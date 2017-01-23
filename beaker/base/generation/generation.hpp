// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_GENERATION_GENERATION_HPP
#define BEAKER_BASE_GENERATION_GENERATION_HPP

#include <beaker/base/lang.hpp>
#include <beaker/base/generation/type.hpp>
#include <beaker/base/generation/value.hpp>
#include <beaker/base/generation/function.hpp>

#include <llvm/IR/IRBuilder.h>

#include <string>
#include <unordered_map>
#include <vector>


namespace llvm {

class LLVMContext;
using Context = LLVMContext;

class Module;
class Type;
class Value;
class Function;
class BasicBlock;

using Builder = IRBuilder<>;

} // namespace llvm


namespace beaker {

struct module;
struct name;
struct type;
struct expr;
struct decl;
struct stmt;

using type_map = std::unordered_map<const type*, cg::type>;
using fn_info_map = std::unordered_map<const type*, cg::fn_info>;

using decl_env = std::unordered_map<const decl*, cg::value>;
using decl_stack = std::vector<std::vector<const decl*>>;

// Provides context and facilities for generating LLVM bitcode.
struct generator
{
  struct decl_context_guard;
  struct loop_guard;
  struct init_guard;

  generator(const char*);
  generator(generator&);
  ~generator();

  llvm::Context& get_context();
  llvm::Module& get_module();

  // Type information
  const type_map& get_types() const;
  type_map& get_types();
  const cg::type& get_type(const type&);

  // Annnotations
  template<typename T, typename... Args>
  cg::type& annotate(cg::type&, Args&&... args);

  template<typename T, typename... Args>
  cg::value& annotate(cg::value&, Args&&... args);

  const cg::fn_info& get_function_info(const type&);
  void put_function_info(const type&, const cg::fn_info&);
  void put_function_info(const type&, cg::fn_info&&);

  // Scoping
  void enter_decl_context();
  void leave_decl_context();
  llvm::Value* get_value(const decl&);
  void put_value(const decl&, llvm::Value*);
  bool seen_decl(const decl&);

  // Current function.
  void define_function(llvm::Function*);
  llvm::Function* get_function();
  llvm::Value* get_return_value();
  void set_return_value(llvm::Value*);
  llvm::BasicBlock* get_entry_block();
  llvm::BasicBlock* get_exit_block();
  void set_exit_block(llvm::BasicBlock*);
  void end_function();

  // Instruction builders.
  cg::value make_alloca(cg::type, const char* = "");
  cg::value make_alloca(cg::type, const std::string&);

  // Current initialization. Managed by init_guard.
  llvm::Value* get_initialized_object();
  
  // Current insertion block.
  llvm::BasicBlock* get_current_block();
  void set_current_block(llvm::BasicBlock*);
  
  // Loop constructs
  llvm::BasicBlock* get_top_block();
  llvm::BasicBlock* get_bottom_block();
  void set_loop_blocks(llvm::BasicBlock*, llvm::BasicBlock*);

  // Source context
  const module& get_source_module();
  void enter_source_module(const module&);
  void leave_source_module();

  // Printing and serialization
  void print();
  void print(std::ostream&);

  generator* parent_;  // The parent generation context.
  llvm::Context* cxt_; // The context
  llvm::Module* mod_;  // The module

  // Mapping of declarations to values.
  decl_env decl_env_;
  decl_stack decl_stack_;

  // Additional information
  type_map types_;
  fn_info_map fn_info_;
  
  // Annotations
  std::vector<cg::note*> notes_;

  // TODO: Coalesce these into a structure.
  llvm::Function* fn_; // The current function
  llvm::Value* return_; // The return value.
  llvm::BasicBlock* entry_; // Function entry block
  llvm::BasicBlock* exit_; // Function exit block
  llvm::BasicBlock* block_; // The current block
  llvm::BasicBlock* top_; // The top of the current loop.
  llvm::BasicBlock* bottom_; // The bottom of the current loop.

  // Initialization context.
  llvm::Value* init_; // The current initialization

  // Source context
  const module* src_mod_;
};

/// Returns the current code generation context.
inline llvm::Context& generator::get_context() { return *cxt_; }

/// Returns the current module.
inline llvm::Module& generator::get_module() { return *mod_; }

/// Returns the set of generated types.
inline const type_map& generator::get_types() const { return types_; }

/// Returns the set of generated types.
inline type_map& generator::get_types() { return types_; }

/// Returns the codegen type corresponding to t.
inline const cg::type& 
generator::get_type(const type& t)
{
  assert(types_.count(&t) != 0);
  return types_.find(&t)->second;
}

/// Annotate the type t with the given value.
///
/// Note that an object should be annotated only once, when it is created.
template<typename T, typename... Args>
inline cg::type&
generator::annotate(cg::type& t, Args&&... args)
{
  cg::note* n = new T(std::forward<Args>(args)...);
  notes_.push_back(n);
  t.annotate(*n);
  return t;
}

/// Annotate the type t with the given value.
///
/// Note that an object should be annotated only once, when it is created.
template<typename T, typename... Args>
inline cg::value&
generator::annotate(cg::value& v, Args&&... args)
{
  cg::note* n = new cg::note(std::forward<Args>(args)...);
  notes_.push_back(n);
  v.annotate(*n);
  return v;
}

/// Returns the current function.
inline llvm::Function* generator::get_function() { return fn_; }

/// Returns the current function's return value.
///
/// For `void` functions, the return value will be null.
inline llvm::Value* generator::get_return_value() { return return_; }

/// Returns the current function's return value.
inline void generator::set_return_value(llvm::Value* v) { return_ = v; }

/// Returns the current function's entry block.
inline llvm::BasicBlock* generator::get_entry_block() { return entry_; }

/// Returns the current function's exit block.
inline llvm::BasicBlock* generator::get_exit_block() { return exit_; }

/// Sets the exit block to b.
inline void generator::set_exit_block(llvm::BasicBlock* b) { exit_ = b; }

/// Returns the current block being built.
inline llvm::BasicBlock* generator::get_current_block() { return block_; }


/// Returns the top block of the current loop.
///
/// This is null if not generating code for a loop.
inline llvm::BasicBlock* generator::get_top_block() { return top_; }

/// Returns the bottom block of the current loop.
///
/// This is null if not generating code for a loop.
inline llvm::BasicBlock* generator::get_bottom_block() { return top_; }

/// Set the top and bottom blocks of the current loop to t and b.
inline void 
generator::set_loop_blocks(llvm::BasicBlock* t, llvm::BasicBlock* b)
{
  top_ = t;
  bottom_ = b;
}

/// Returns the current object being initialized.
inline llvm::Value* generator::get_initialized_object() { return init_; }

/// Returns the module being analyzed.
inline const module& generator::get_source_module() { return *src_mod_; }

/// Sets the source module to m.
inline void generator::enter_source_module(const module& m) { src_mod_ = &m; }

/// Clears the source module.
inline void generator::leave_source_module() { src_mod_ = nullptr; }


// Utilities

// Used to manage the entry and exit of declaration contexts during
// code generation.
struct generator::decl_context_guard
{
  decl_context_guard(generator& g)
    : gen(g)
  {
    gen.enter_decl_context();
  }

  ~decl_context_guard()
  {
    gen.leave_decl_context();
  }

  generator& gen;
};


// An RAII class that manages the top and bottom blocks of loops. These are 
// the current jump targets for the break and continue statements.
struct generator::loop_guard
{
  loop_guard(generator& gen, llvm::BasicBlock* t, llvm::BasicBlock* b)
    : gen(gen), top(gen.top_), bot(gen.bottom_)
  {
    // Set up the current loop.
    gen.set_loop_blocks(t, b);
  }

  ~loop_guard()
  {
    // Restore the previous loop.
    gen.set_loop_blocks(top, bot);
  }

  generator& gen;
  llvm::BasicBlock* top;
  llvm::BasicBlock* bot;
};


// An RAII class that manages the current initialization.
struct generator::init_guard
{
  init_guard(generator& gen, llvm::Value* p)
    : gen(gen), ptr(gen.init_)
  {
    gen.init_ = p;
  }

  ~init_guard()
  {
    gen.init_ = ptr;
  }

  generator& gen;
  llvm::Value* ptr;
};

// -------------------------------------------------------------------------- //
// Operations

/// Defines the dispatch signature for codegen algorithms.
struct generate_algorithm : algorithm
{
  struct tag { };

  virtual std::string operator()(generator&, const name&) const;
  virtual cg::type operator()(generator&, const type&) const;
  virtual cg::value operator()(generator&, const expr&) const;
  virtual cg::value operator()(generator&, const decl&) const;
  virtual void operator()(generator&, const stmt&) const;
};

void generate(generator&, const module&);
std::string generate(generator&, const name&);
cg::type generate(generator&, const type&);
cg::value generate(generator&, const expr&);
cg::value generate(generator&, const decl&);
void generate(generator&, const stmt&);

} // namespace beaker


#endif

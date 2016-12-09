// Copyright (c) 2015-2016 Andrew Sutton
// All rights reserved

#ifndef BEAKER_BASE_SEQ_HPP
#define BEAKER_BASE_SEQ_HPP

#include <vector>


namespace beaker {

// The iterator type for sequences.
template<typename T>
struct seq_iterator
{
  using Iter              = T*;
  using value_type        = T;
  using reference         = T&;
  using pointer           = T*;
  using difference_type   = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  seq_iterator() = default;

  seq_iterator(T** i)
    : iter(i)
  { }

  reference operator*() const { return **iter; }
  pointer  operator->() const { return *iter; }

  seq_iterator& operator++() { ++iter; return *this; }
  seq_iterator  operator++(int) { auto x = *this; ++iter; return x; }

  seq_iterator& operator--() { --iter; return *this; }
  seq_iterator  operator--(int) { auto x = *this; --iter; return x; }

  seq_iterator& operator+=(difference_type n) { iter += n; return *this; }
  seq_iterator& operator-=(difference_type n) { iter -= n; return *this; }

  T** iter;
};

template<typename T>
struct seq_iterator<T const>
{
  using Iter              = T const*;
  using value_type        = T;
  using reference         = T const&;
  using pointer           = T const*;
  using difference_type   = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

  seq_iterator() = default;

  seq_iterator(T* const* i)
    : iter(i)
  { }

  seq_iterator(seq_iterator<T> i)
    : iter(i.iter)
  { }

  reference operator*() const { return **iter; }
  pointer  operator->() const { return *iter; }

  seq_iterator& operator++()    { ++iter; return *this; }
  seq_iterator operator++(int) { auto x = *this; ++iter; return x; }

  seq_iterator& operator+=(std::ptrdiff_t n) { iter += n; return *this; }
  seq_iterator& operator-=(std::ptrdiff_t n) { iter -= n; return *this; }

  T* const* iter;
};


template<typename T>
bool operator==(seq_iterator<T> i, seq_iterator<T> j)
{
  return i.iter == j.iter;
}

template<typename T>
bool operator!=(seq_iterator<T> i, seq_iterator<T> j)
{
  return i.iter != j.iter;
}

template<typename T>
bool operator<(seq_iterator<T> i, seq_iterator<T> j)
{
  return i.iter < j.iter;
}

template<typename T>
bool operator>(seq_iterator<T> i, seq_iterator<T> j)
{
  return i.iter > j.iter;
}

template<typename T>
bool operator<=(seq_iterator<T> i, seq_iterator<T> j)
{
  return i.iter <= j.iter;
}

template<typename T>
bool operator>=(seq_iterator<T> i, seq_iterator<T> j)
{
  return i.iter >= j.iter;
}

template<typename T>
seq_iterator<T> operator+(seq_iterator<T> i, std::ptrdiff_t n)
{
  return i += n;
}

template<typename T>
seq_iterator<T> operator+(std::ptrdiff_t n, seq_iterator<T> i)
{
  return i += n;
}

template<typename T>
seq_iterator<T> operator-(seq_iterator<T> i, std::ptrdiff_t n)
{
  return i -= n;
}

template<typename T>
std::ptrdiff_t operator-(seq_iterator<T> i, seq_iterator<T> j)
{
  return i - j;
}


// A sequence of terms.
//
// This is a vector of node pointers. Iterators and accessors yield nodes
// as objects, however.
template<typename T>
struct seq
{
  using iterator = seq_iterator<T>;
  using const_iterator = seq_iterator<const T>;

  seq() = default;

  seq(std::initializer_list<T*> list)
    : elems_(list)
  { }

  // Returns this object, which can be used to disambiguation conversions.
  seq const& form() const { return *this; }
  seq& form() { return *this; }

  // Returns the underlying pointer vector.
  std::vector<T*> const& base() const { return elems_; }
  std::vector<T*>& base() { return elems_; }

  // Returns true if the sequence is empty.
  bool is_empty() const { return elems_.empty(); }
  
  // Returns the number of elements in the sequence.
  int size() const { return (int)elems_.size(); }

  // Returns the nth term in the sequence.
  T const& operator[](int n) const { return *elems_[n]; }
  T& operator[](int n) { return *elems_[n]; }

  // Returns the first element in the sequence.
  T const& front() const { return *elems_.front(); }
  T& front() { return *elems_.front(); }

  // Returns the last element in the sequence.
  T const& back() const { return *elems_.back(); }
  T& back() { return *elems_.back(); }

  // Insert an element at the back of the sequence.
  void push_back(T& x) { elems_.push_back(&x); }
  void push_back(T* x) { elems_.push_back(x); }

  // Insert a range of iterators at the end of the sequence.
  template<typename I>
  void push_back(I first, I limit) {
    while (first != limit)
      push_back(*first++);
  }

  // Iterators
  const_iterator begin() const { return base().data(); }
  const_iterator end() const { return base().data() + size(); }

  iterator begin() { return base().data(); }
  iterator end() { return base().data() + size(); }

  std::vector<T*> elems_;
};


template<typename T>
bool
equivalent(const seq<T>& a, const seq<T>& b) {
  auto cmp = [](const T& a, const T& b) {
    return equivalent(a, b);
  };
  return std::equal(a.begin(), a.end(), b.begin(), b.end(), cmp);
}


struct hasher;

/// Hash the elements of s into h.
template<typename T>
inline void
hash(hasher& h, const seq<T>& s) {
  for (const T& t : s)
    hash(h, t);
  hash(h, s.size());
}

} // namespace beaker


#endif

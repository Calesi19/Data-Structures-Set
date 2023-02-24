/***********************************************************************
* Header:
*    Set
* Summary:
*      __       ____       ____         __
*     /  |    .'    '.   .'    '.   _  / /
*     `| |   |  .--.  | |  .--.  | (_)/ /
*      | |   | |    | | | |    | |   / / _
*     _| |_  |  `--'  | |  `--'  |  / / (_)
*    |_____|  '.____.'   '.____.'  /_/
*
*    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    <Your name here>
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
   friend class ::TestSet; // give unit tests access to the privates
public:
   
   // 
   // Construct
   //
   set()
   { 
   }
   set(const set &  rhs) : bst(rhs.bst)
   { 
   }
   set(set && rhs) : bst(std::move(rhs.bst))
   { 
   }
   set(const std::initializer_list <T> & il)
   {
       for (auto it = il.begin(); it != il.end(); ++it)
           bst.insert(*it);
   }
   template <class Iterator>
   set(Iterator first, Iterator last)
   {
       for (auto it = first; it != last; ++it)
           bst.insert(*it);
   }
  ~set() { }

   //
   // Assign
   //

   set & operator = (const set & rhs)
   {
       bst = rhs.bst;
       return *this;
   }
   set & operator = (set && rhs)
   {
       bst = std::move(rhs.bst);
       return *this;
   }
   set & operator = (const std::initializer_list <T> & il)
   {
       bst.clear();
       for (auto it = il.begin(); it != il.end(); ++it)
           bst.insert(*it);
       return *this;
   }
   void swap(set& rhs) noexcept
   {
       std::swap(bst, rhs.bst);
   }

   //
   // Iterator
   //

   class iterator;
   iterator begin() const noexcept 
   { 
      return iterator(bst.begin());
   }
   iterator end() const noexcept 
   { 
      return iterator(bst.end()); 
   }

   //
   // Access
   //
   iterator find(const T& t) 
   { 
      return iterator(bst.find(t)); 
   }

   //
   // Status
   //
   bool   empty() const noexcept 
   { 
      return bst.empty();
   }
   size_t size() const noexcept 
   { 
      return bst.size();
   }

   //
   // Insert
   //
   std::pair<iterator, bool> insert(const T& t)
   {
       auto result = bst.insert(t);
       iterator it(result.first);
       std::pair<iterator, bool> p(it, result.second);
       return p;
   }
   std::pair<iterator, bool> insert(T&& t)
   {
       auto result = bst.insert(std::move(t));
       iterator it(result.first);
       std::pair<iterator, bool> p(it, result.second);
       return p;
   }
   void insert(const std::initializer_list <T>& il)
   {
       for (auto it = il.begin(); it != il.end(); ++it)
           bst.insert(*it);
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
       for (auto it = first; it != last; ++it)
           bst.insert(*it);
   }


   //
   // Remove
   //
   void clear() noexcept 
   { 
       bst.clear();
   }
   iterator erase(iterator &it)
   { 
       iterator returnIt(it);
       ++returnIt;
       bst.erase(*it);
       return returnIt;
   }
   size_t erase(const T & t) 
   {
       return bst.size();  //fix
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
       while (itBegin != itEnd)
       {
           itBegin = erase(itBegin);
       }
       return itEnd;
   }

private:
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
   friend class ::TestSet; // give unit tests access to the privates
   friend class custom::set<T>;

public:
   // constructors, destructors, and assignment operator
   iterator() : it() {}
    
   iterator(const typename custom::BST<T>::iterator& itRHS) : it(itRHS) {}
    
   iterator(const iterator & rhs) : it(rhs.it) {}
    
   iterator & operator = (const iterator & rhs)
   {
       it = rhs.it;
       return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
       return it != rhs.it;
   }
   bool operator == (const iterator & rhs) const 
   { 
       return it == rhs.it;
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const 
   { 
      return *it;
   }

   // prefix increment
   iterator & operator ++ ()
   {
       ++it;
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
       iterator tmp(*this);
       ++it;
       return tmp;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
       --it;
       return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
       iterator tmp(*this);
       --it;
       return tmp;
   }
   
private:

   typename custom::BST<T>::iterator it;
};

/***********************************************
 * SET : EQUIVALENCE
 * See if two sets are the same size
 ***********************************************/
template <typename T>
bool operator == (const set <T> & lhs, const set <T> & rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    auto itLhs = lhs.begin();
    auto itRhs = rhs.begin();

    while (itLhs != lhs.end() && itRhs != rhs.end())
    {
        if (*itLhs != *itRhs)
            return false;

        ++itLhs;
        ++itRhs;
    }

    return true;
}

template <typename T>
inline bool operator != (const set <T> & lhs, const set <T> & rhs)
{
    return !(lhs == rhs);
}

/***********************************************
 * SET : RELATIVE COMPARISON
 * See if one set is lexicographically before the second
 ***********************************************/
template <typename T>
bool operator < (const set <T> & lhs, const set <T> & rhs)
{
    auto itLhs = lhs.begin();
    auto itRhs = rhs.begin();

    while (itLhs != lhs.end() && itRhs != rhs.end())
    {
        if (*itLhs < *itRhs)
            return true;

        if (*itRhs < *itLhs)
            return false;

        ++itLhs;
        ++itRhs;
    }

    // if one set is a prefix of the other, then the shorter one is lexicographically first
    return lhs.size() < rhs.size();
}

template <typename T>
inline bool operator > (const set <T> & lhs, const set <T> & rhs)
{
    auto itLhs = lhs.begin();
    auto itRhs = rhs.begin();

    while (itLhs != lhs.end() && itRhs != rhs.end())
    {
        if (*itLhs > *itRhs)
            return true;

        if (*itRhs > *itLhs)
            return false;

        ++itLhs;
        ++itRhs;
    }

    // if one set is a prefix of the other, then the shorter one is lexicographically first
    return lhs.size() < rhs.size();
}

}; // namespace custom




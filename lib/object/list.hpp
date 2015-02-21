/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \section License
 *
 * Copyright (C) 2015 Mattia Basaglia
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef LIST_HPP
#define LIST_HPP

#include "mirror.hpp"
#include <list>

namespace object {

class Array;

/**
 * \brief A list
 * \todo test
 * \todo string conversion
 */
class List : public Mirror
{
    MIRROR(List,Mirror)

public:
    typedef std::list<util::Any> Sequence; ///< Internal container type

    // C++ Container Requirements
    typedef Sequence::value_type      value_type;
    typedef Sequence::reference       reference;
    typedef Sequence::const_reference const_reference;
    typedef Sequence::iterator        iterator;
    typedef Sequence::const_iterator  const_iterator;
    typedef Sequence::difference_type difference_type;
    typedef Sequence::size_type       size_type;

    List() = default;
    List(const List&) = default;
    List& operator=(const List&) = default;

    iterator begin() { return elements.begin(); }
    const_iterator begin() const { return elements.begin(); }
    const_iterator cbegin() const { return elements.begin(); }
    iterator end() { return elements.end(); }
    const_iterator end() const { return elements.end(); }
    const_iterator cend() const { return elements.end(); }

    bool operator== (const List& other) const
    {
        return elements == other.elements;
    }
    bool operator!= (const List& other) const
    {
        return elements == other.elements;
    }

    void swap ( List& other ) { elements.swap(other.elements); }
    friend void swap (List& a, List& b) { a.swap(b); }

    size_type size() const { return elements.size(); }
    size_type max_size() const { return elements.max_size(); }
    bool empty() const { return elements.empty(); }
    void clear()  { elements.clear(); }

    // C++ SequenceContainer
    template<class InputIterator>
        List( const InputIterator& i, const InputIterator& j )
            : elements (i,j) {}

    List ( const std::initializer_list<value_type>& il )
        : elements(il) {}

    template<class ...Args>
        iterator emplace ( const const_iterator& p, Args&&... args )
    {
        return elements.emplace(p, args...);
    }

    iterator insert(const const_iterator& p, const value_type& t)
    {
        return elements.insert(p,t);
    }

    iterator insert(const const_iterator& p, size_type n, const value_type& t)
    {
        return elements.insert(p, n, t);
    }

    template<class InputIterator>
        iterator insert(const const_iterator& p, const InputIterator& i, const InputIterator& j )
        {
            return elements.insert(p, i, j);
        }

    iterator insert(const const_iterator& p, const std::initializer_list<value_type>& il )
    {
        return elements.insert(p,il);
    }

    iterator erase(const const_iterator& q)
    {
        return elements.erase(q);
    }

    iterator erase(const const_iterator& p, const const_iterator& q)
    {
        return elements.erase(p,q);
    }

    void assign(size_type n, const value_type& t)
    {
        elements.assign(n, t);
    }

    template<class InputIterator>
        void assign(const InputIterator& i, const InputIterator& j )
        {
            elements.assign(i, j);
        }

    void assign(const std::initializer_list<value_type>& il )
    {
        elements.assign(il);
    }

    /**
     * \brief Get the equivalent array
     */
    Array array() const;

    /**
     * \brief Whether the list contains the given value
     */
    bool contains(const util::Any& value) const;

    /**
     * \brief Return the number of occurrences of the given value
     */
    size_type count(const_reference value) const;

    /**
     * \brief Sorts the list
     */
    void sort();

    /**
     * \brief Gets the element at the given position
     */
    util::Any at(size_type pos) const;

    /**
     * \brief Removes all the occurences of the given value
     */
    void remove(const_reference value);

    /**
     * \brief Removes the first occurence of the given value
     */
    void remove_first(const_reference value);

    /**
     * \brief Append a value with a numeric key
     */
    void push_back(const_reference value)
    {
        elements.push_back(value);
    }

    /**
     * \brief Remove the last element
     */
    void pop_back()
    {
        elements.pop_back();
    }

    /**
     * \brief Append a value with a numeric key
     */
    void push_front(const_reference value)
    {
        elements.push_front(value);
    }
    /**
     * \brief Remove the first element
     */
    void pop_front()
    {
        elements.pop_front();
    }

private:
    Sequence elements;
};

} // namespace object
#endif // LIST_HPP

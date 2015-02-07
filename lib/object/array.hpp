/**
 \ file            *

 \author Mattia Basaglia

 \section License

 Copyright (C)  Mattia Basaglia

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#ifndef OBJECT_ARRAY_HPP
#define OBJECT_ARRAY_HPP
#include "dynamic_mirror.hpp"
namespace object {

/**
 * \brief An associative array object
 * \todo test
 */
class Array : public Dynamic_Mirror
{
    MIRROR(Array,Dynamic_Mirror)

public:
    // C++ Container Requirements
    typedef Properties::value_type      value_type;
    typedef Properties::reference       reference;
    typedef Properties::const_reference const_reference;
    typedef Properties::iterator        iterator;
    typedef Properties::const_iterator  const_iterator;
    typedef Properties::difference_type difference_type;
    typedef Properties::size_type       size_type;

    Array() = default;
    Array(const Array&) = default;
    Array& operator=(const Array&) = default;

    iterator begin() { return properties.begin(); }
    const_iterator begin() const { return properties.begin(); }
    const_iterator cbegin() const { return properties.begin(); }
    iterator end() { return properties.end(); }
    const_iterator end() const { return properties.end(); }
    const_iterator cend() const { return properties.end(); }

    bool operator== (const Array& other) const
    {
        return properties == other.properties;
    }
    bool operator!= (const Array& other) const
    {
        return properties == other.properties;
    }

    void swap ( Array& other ) { properties.swap(other.properties); }
    friend void swap (Array& a, Array& b) { a.swap(b); }

    size_type size() const { return properties.size(); }
    size_type max_size() const { return properties.max_size(); }
    bool empty() const { return properties.empty(); }
    void clear()  { properties.clear(); }

    // C++ AssociativeContainer
    typedef Properties::key_type        key_type;
    typedef Properties::key_compare     key_compare;
    typedef Properties::value_compare   value_compare;
    typedef Properties::mapped_type     mapped_type;

    Array ( const key_compare& c) { properties = Properties(c); }
    template<class InputIterator>
        Array( const InputIterator& i, const InputIterator& j,
               const key_compare& c = key_compare() )
        {
            properties = Properties(i,j,c);
        }

    Array ( const std::initializer_list<value_type>& il )
    {
        properties = Properties(il);
    }

    key_compare key_comp() const { return properties.key_comp(); }
    value_compare value_comp() const { return properties.value_comp(); }

    // Extra scripting methods
    /**
     * \brief Returns a numeric array of the keys
     */
    Array keys() const;
    /**
     * \brief Returns a numeric array of the values
     */
    Array values() const;
    /**
     * \brief Transforms the Array so that keys are consecutive integer starting from 0
     */
    void normalize_keys();
    /**
     * \brief Whether the array contains the given key
     */
    bool key_exists(const std::string& key) const;
    /**
     * \brief Whether the array contains the given value
     */
    bool contains(const util::Any& value) const;
    /**
     * \brief Get a subarray
     *
     * The subarray consists of the elements starting from
     * the key \c a and ending on the key \c b (inclusive)
     */
    Array subarray(const std::string& a, const std::string& b);
    /**
     * \brief Removes the element indexed by the given key
     */
    void remove(const std::string& key);
    /**
     * \brief Append a value with a numeric key
     */
    void push_back(const util::Any& value);
    /**
     * \brief Remove the last element
     */
    void pop_back();
    /**
     * \brief Append a value with a numeric key
     */
    void push_front(const util::Any& value);
    /**
     * \brief Remove the first element
     */
    void pop_front();
};



} // namespace object
#endif // OBJECT_ARRAY_HPP

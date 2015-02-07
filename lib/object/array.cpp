/**
 \file

 \author Mattia Basaglia

 \section License

 Copyright (C) 2015 Mattia Basaglia

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
#include "array.hpp"

namespace object {

DECLARE_METHOD(Array,size)
DECLARE_METHOD(Array,clear)
DECLARE_METHOD(Array,empty)
DECLARE_METHOD(Array,keys)
DECLARE_METHOD(Array,values)
DECLARE_METHOD(Array,normalize_keys)
DECLARE_METHOD(Array,key_exists)
DECLARE_METHOD(Array,contains)
DECLARE_METHOD_FUNCTOR(Array,subarray,
    [](Array& arr, const Mirror::Arguments& props)
    {
        return arr.subarray(
            props.empty() ? "" : props.at(0).to_string(),
            props.size()<2? "" : props.at(1).to_string() );
    })
DECLARE_METHOD(Array,remove)
DECLARE_METHOD(Array,push_back)
DECLARE_METHOD(Array,pop_back)
DECLARE_METHOD(Array,push_front)
DECLARE_METHOD(Array,pop_front)

Array Array::keys() const
{
    Array k;
    for ( const value_type& p : *this )
        k.push_back(p.first);
    return k;
}

Array Array::values() const
{
    Array k;
    for ( const value_type& p : *this )
        k.push_back(p.second);
    return k;
}

void Array::normalize_keys()
{
    *this = values();
}

bool Array::key_exists(const std::string& key) const
{
    return properties.count(key);
}
bool Array::contains(const util::Any& value) const
{
    for ( const value_type& p : *this )
        if ( p.second == value )
            return true;
    return false;
}
Array Array::subarray(const std::string& a, const std::string& b)
{
   Array ret;
   auto it_a = properties.find(a);
   if ( it_a == properties.end() )
       it_a = properties.begin();
   auto it_b = properties.find(b);
   if ( it_b != properties.end() )
       ++it_b; // inclusive
   for ( ; it_a != it_b; ++it_a )
       ret.properties.insert(*it_a);
   return ret;
}

void Array::remove(const std::string& key)
{
    properties.erase(key);
}


void Array::push_back(const util::Any& value)
{
    int i = size();
    while ( key_exists(std::to_string(i)) )
        i++;
    properties.insert({std::to_string(i), value});
}

void Array::pop_back()
{
    properties.erase(end());
}

void Array::push_front(const util::Any& value)
{
    /// \note very inefficient...
    Properties p;
    p.insert({"0",value});
    int i = 0;
    for ( const auto& pair : *this )
        p.insert({std::to_string(++i), pair.second});
}

void Array::pop_front()
{
}

} // namespace object

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

#include "list.hpp"
#include "array.hpp"

#include <algorithm>
#include <iterator>

namespace object {

DECLARE_METHOD(List,size)
DECLARE_METHOD(List,clear)
DECLARE_METHOD(List,empty)
DECLARE_METHOD(List,contains)
DECLARE_METHOD(List,count)
DECLARE_METHOD(List,remove)
DECLARE_METHOD(List,remove_first)
DECLARE_METHOD(List,push_back)
DECLARE_METHOD(List,pop_back)
DECLARE_METHOD(List,push_front)
DECLARE_METHOD(List,pop_front)
DECLARE_METHOD(List,array)
DECLARE_METHOD(List,sort)
DECLARE_METHOD(List,at)

Array List::array() const
{
    Array arr;
    for ( const auto& v : elements )
        arr.push_back(v);
    return arr;
}

bool List::contains(const util::Any& value) const
{
    return std::find(elements.begin(), elements.end(), value) != elements.end();
}

List::size_type List::count(const_reference value) const
{
    return std::count(elements.begin(), elements.end(), value);
}

void List::sort()
{
    /// \todo requires Any::operator< elements.sort();
}

util::Any List::at(size_type pos) const
{
    if ( pos >= elements.size() )
        return util::Any();
    auto iter = elements.begin();
    std::advance(iter, pos);
    return *iter;
}

void List::remove(const_reference value)
{
    auto iter = elements.begin();
    while ( iter != elements.end() )
    {
        iter = std::find(iter,elements.end(),value);
        elements.erase(iter);
    }
}

void List::remove_first(const_reference value)
{
    elements.erase(std::find(elements.begin(),elements.end(),value));
}


} // namespace object

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
#include "any.hpp"

namespace util {


Any& Any::swap(Any& other)
{
    std::swap(content, other.content);
    return *this;
}

Any& Any::operator=(const Any& other)
{
    delete content;
    content = other.content->clone();
    return *this;
}

Any& Any::operator=(Any&& other)
{
    delete content;
    content = other.content;
    other.content = nullptr;
    return *this;
}

std::string Any::to_string() const
{
    if (empty())
        return std::string();
    if (type_info() == typeid(std::string))
        return cast<std::string>();
    std::ostringstream ss;
    content->stream_output(ss);
    return ss.str();
}

std::ostream& operator<< ( std::ostream& stream, const Any& any)
{
    return any.content ? any.content->stream_output(stream) : stream;
}

} // namespace util

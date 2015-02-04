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
#include "dynamic_mirror.hpp"
using namespace object;

util::Any Dynamic_Mirror::get_extra(const std::string& name) const
{
    auto it = properties.find(name);
    return it != properties.end() ? it->second : util::Any();
}

bool Dynamic_Mirror::can_get_extra(const std::string& name) const
{
    return properties.count(name);
}

void Dynamic_Mirror::set_extra(const std::string& name, const util::Any& value)
{
    properties[name] = value;
}

bool Dynamic_Mirror::can_set_extra(const std::string& name) const
{
    return properties.count(name);
}

void Dynamic_Mirror::get_all_extra(Properties& out, const std::string& prefix) const
{
    for ( const auto& p : properties )
        out[prefix+p.first] = p.second;
}

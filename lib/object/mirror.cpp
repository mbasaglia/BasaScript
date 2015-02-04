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
#include "mirror.hpp"

using namespace object;

Mirror::Magic_Mirror& Mirror::Magic_Mirror::instance()
{
    static Magic_Mirror s;
    return s;
}
bool Mirror::Magic_Mirror::can_get( const std::string&) const
{
    return false;
}
util::Any Mirror::Magic_Mirror::get(const Mirror&, const std::string&) const
{
    return util::Any();
}
void Mirror::Magic_Mirror::get_all(const Mirror&, Properties&, const std::string&) const
{
}
bool Mirror::Magic_Mirror::can_set(const std::string&) const
{
    return false;
}
void Mirror::Magic_Mirror::set(Mirror&, const std::string&, const util::Any&) const
{
}
bool Mirror::Magic_Mirror::can_call(const std::string&) const
{
    return false;
}
util::Any Mirror::Magic_Mirror::call(Mirror&, const std::string&, const Arguments&) const
{
    return util::Any();
}


std::string Mirror::get_string (const std::string& name) const
{
    return get_any(name).to_string();
}

util::Any Mirror::get_any (const std::string& name) const
{
    return get_magic().can_get(name) ?
        get_magic().get(*this, name) :
        get_extra(name);
}

void Mirror::set(const std::string& name, const util::Any& value)
{
    if ( get_magic().can_set(name) )
        get_magic().set(*this, name, value);
    else
        set_extra(name, value);
}

bool Mirror::can_set(const std::string& name) const
{
    return get_magic().can_set(name) || can_set_extra(name);
}

bool Mirror::can_get(const std::string& name) const
{
    return get_magic().can_get(name) || can_get_extra(name);
}

bool Mirror::can_call(const std::string& name) const
{
    return get_magic().can_call(name) || can_call_extra(name);
}

Mirror::Properties Mirror::properties(const std::string& prefix) const
{
    Mirror::Properties props;
    properties(props, prefix);
    return props;
}

void Mirror::properties(Properties& out, const std::string& prefix) const
{
    get_all_extra(out, prefix);
    get_magic().get_all(*this, out, prefix);
}

util::Any Mirror::call(const std::string& name, const Arguments& args)
{
    return get_magic().can_call(name) ?
        get_magic().call(*this, name, args) :
        call_extra(name, args);
}

util::Any Mirror::get_extra(const std::string&) const
{
    return util::Any();
}

bool Mirror::can_get_extra(const std::string&) const
{
    return false;
}

void Mirror::set_extra(const std::string&, const util::Any&)
{
}

bool Mirror::can_set_extra(const std::string&) const
{
    return false;
}

void Mirror::get_all_extra(Properties&, const std::string&) const
{
}

util::Any Mirror::call_extra(const std::string&, const Arguments&)
{
    return util::Any();
}

bool Mirror::can_call_extra(const std::string&) const
{
    return false;
}

int Mirror::has_type_id(int id) const
{
    return id == static_type_id();
}

int Mirror::static_type_id()
{
    return 0;
}

int Mirror::new_type_id()
{
    static int id = static_type_id();
    return ++id;
}


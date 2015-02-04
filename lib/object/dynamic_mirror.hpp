/**
 \file

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
#ifndef OBJECT_DYNAMIC_MIRROR_HPP
#define OBJECT_DYNAMIC_MIRROR_HPP

#include "mirror.hpp"

namespace object {

/**
 * \brief Mirror allowing extra dynamic properties
 */
class Dynamic_Mirror : public Mirror
{
    MIRROR(Dynamic_Mirror,Mirror)

protected:
    util::Any get_extra(const std::string& name) const override;
    bool can_get_extra(const std::string& name) const override;
    void set_extra(const std::string& name, const util::Any& value) override;
    bool can_set_extra(const std::string& name) const override;
    void get_all_extra(Properties& out, const std::string& prefix = "") const override;

private:
    Properties properties;
};

} // namespace object
#endif // OBJECT_DYNAMIC_MIRROR_HPP

/**
\file

\author Mattia Basaglia

\section License

Copyright (C) 2015  Mattia Basaglia

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "real.hpp"

#include <sstream>
#include <boost/math/constants/constants.hpp>

namespace math {

const Real pi = boost::math::constants::pi<Real>();

Real from_float(long double d, int precision)
{
    std::stringstream ss;
    ss.precision(precision);
    ss << d;
    return Real(ss.str());
}


} // namespace math


std::ostream& operator<< (std::ostream& output, const math::Real& number)
{
    auto precision = output.precision();
    output.precision(std::numeric_limits<math::Real>::digits10);
    boost::multiprecision::operator<<(output,number);
    output.precision(precision);
    return output;
}

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

#ifndef MATH_REAL_HPP
#define MATH_REAL_HPP

#include <boost/multiprecision/cpp_dec_float.hpp>

/**
 * \brief Namespace for mathematical utilities
 */
namespace math {

/**
 * \brief Fixed precision real numbers
 */
typedef boost::multiprecision::cpp_dec_float_100 Real;

/**
 * \brief Converts from a double with the given precision
 * \note If you simply use Real(double) you might get the imprecisions
 * of floating point numbers to be reflected into the real number
 */
Real from_float(long double d, int precision);

/**
 * \brief The number of radians to get half a pie
 */
extern const Real pi;

#define UNARY_OP_FUNCTOR(func) inline Real func ( Real n ) { return Real(boost::multiprecision::func(n)); }
#define BINARY_OP_FUNCTOR(func) inline Real func ( Real a, Real b ) { return Real(boost::multiprecision::func(a,b)); }

UNARY_OP_FUNCTOR(fabs)
UNARY_OP_FUNCTOR(sqrt)
UNARY_OP_FUNCTOR(floor)
UNARY_OP_FUNCTOR(ceil)
UNARY_OP_FUNCTOR(trunc)
UNARY_OP_FUNCTOR(round)
UNARY_OP_FUNCTOR(exp)
UNARY_OP_FUNCTOR(log)
UNARY_OP_FUNCTOR(log10)
UNARY_OP_FUNCTOR(cos)
UNARY_OP_FUNCTOR(sin)
UNARY_OP_FUNCTOR(tan)
UNARY_OP_FUNCTOR(asin)
UNARY_OP_FUNCTOR(acos)
UNARY_OP_FUNCTOR(atan)
UNARY_OP_FUNCTOR(cosh)
UNARY_OP_FUNCTOR(sinh)
UNARY_OP_FUNCTOR(tanh)

BINARY_OP_FUNCTOR(pow)
BINARY_OP_FUNCTOR(fmod)
BINARY_OP_FUNCTOR(atan2)

#undef UNARY_OP_FUNCTOR
#undef BINARY_OP_FUNCTOR

} // namespace math

/**
 * \note Global scope so that this is selected instead of the one in boost::multiprecision
 */
std::ostream& operator<< (std::ostream& output, const math::Real& number);

#endif // MATH_REAL_HPP

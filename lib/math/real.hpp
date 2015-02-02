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

#include <string>
#include <iostream>
#include <limits>

#include <boost/multiprecision/cpp_dec_float.hpp>

/**
 * \brief Namespace for mathematical utilities
 */
namespace math {

class Real;
namespace detail {
    typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<20>> RealBase;

    const detail::RealBase& real_to_base(const Real& number);
    Real base_to_real(detail::RealBase number);
} // namespace (math::)detail

/**
 * \brief Fixed precision real numbers
 */
class Real
{
public:
    Real (int value = 0);
    Real (long long value);
    Real (const char* value_string);
    Real (const std::string& value_string);
    Real (long double value, int precision);

    Real& operator+= (const Real& other);
    Real& operator-= (const Real& other);
    Real& operator*= (const Real& other);
    Real& operator/= (const Real& other);
    Real& operator++();
    Real  operator++(int);
    Real& operator--();
    Real  operator--(int);

private:
    detail::RealBase value;

    friend const detail::RealBase& detail::real_to_base(const Real& number);
    friend Real detail::base_to_real(detail::RealBase number);
    friend std::istream& operator>> (std::istream& input, Real& number);
};

Real operator+ (const Real& a, const Real& b);
Real operator- (const Real& a, const Real& b);
Real operator* (const Real& a, const Real& b);
Real operator/ (const Real& a, const Real& b);
Real operator- (const Real& a);
Real operator+ (const Real& a);

bool operator< (const Real& a, const Real& b);
bool operator<= (const Real& a, const Real& b);
bool operator== (const Real& a, const Real& b);
bool operator!= (const Real& a, const Real& b);
bool operator>= (const Real& a, const Real& b);
bool operator> (const Real& a, const Real& b);

Real fabs (const Real& a);
Real abs (const Real& a);
Real sqrt (const Real& a);
Real floor (const Real& a);
Real ceil (const Real& a);
Real trunc (const Real& a);
Real round (const Real& a);
Real exp (const Real& a);
Real log (const Real& a);
Real log10 (const Real& a);
Real cos (const Real& a);
Real sin (const Real& a);
Real tan (const Real& a);
Real asin (const Real& a);
Real acos (const Real& a);
Real atan (const Real& a);
Real cosh (const Real& a);
Real sinh (const Real& a);
Real tanh (const Real& a);

Real pow (const Real& a, const Real& b);
Real fmod (const Real& a, const Real& b);
Real atan2 (const Real& a, const Real& b);

std::ostream& operator<< (std::ostream& output, const Real& number);
std::istream& operator>> (std::istream& input, Real& number);

/**
 * \brief The number of radians to get half a pie
 */
extern const Real pi;

} // namespace math

namespace std {
template <>
    class numeric_limits<math::Real> : public numeric_limits<math::detail::RealBase>
    {};
} // namespace std

/**
 * \brief Namespace for custom literals
 */
namespace literals {
    inline math::Real operator"" _real (const char* string) { return math::Real(string); }
} // namespace literals

#endif // MATH_REAL_HPP

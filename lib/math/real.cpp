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

namespace detail {
    const detail::RealBase& real_to_base(const Real& number)
    {
        return number.value;
    }
    Real base_to_real(detail::RealBase number)
    {
        return Real(number.str(std::numeric_limits<Real>::digits10));
    }
} // namespace (math::)detail

const Real pi = detail::base_to_real(boost::math::constants::pi<detail::RealBase>());
const Real e = detail::base_to_real(boost::math::constants::e<detail::RealBase>());

Real::Real (int value) : value (value) {}
Real::Real (long long value) : value (value) {}
Real::Real (const char* value_string) : value (value_string) {}
Real::Real (const std::string& value_string) : value (value_string) {}
Real::Real (long double value, int precision)
{
    std::stringstream ss;
    ss.precision(precision);
    ss << value;
    this->value = detail::RealBase(ss.str());
}

Real& Real::operator+= (const Real& other)
{
    value += other.value;
    return *this;
}
Real& Real::operator-= (const Real& other)
{
    value -= other.value;
    return *this;
}
Real& Real::operator*= (const Real& other)
{
    value *= other.value;
    return *this;
}
Real& Real::operator/= (const Real& other)
{
    /// \todo division by zero?
    value /= other.value;
    return *this;
}
Real& Real::operator++()
{
    ++value;
    return *this;
}
Real  Real::operator++(int)
{
    Real copy = *this;
    ++*this;
    return copy;
}
Real& Real::operator--()
{
    --value;
    return *this;
}
Real  Real::operator--(int)
{
    Real copy = *this;
    --*this;
    return copy;
}

Real operator+ (const Real& a, const Real& b)
{
    return detail::base_to_real(detail::real_to_base(a) + detail::real_to_base(b));
}
Real operator- (const Real& a, const Real& b)
{
    return detail::base_to_real(detail::real_to_base(a) - detail::real_to_base(b));
}
Real operator* (const Real& a, const Real& b)
{
    return detail::base_to_real(detail::real_to_base(a) * detail::real_to_base(b));
}
Real operator/ (const Real& a, const Real& b)
{
    return detail::base_to_real(detail::real_to_base(a) / detail::real_to_base(b));
}
Real operator- (const Real& a)
{
    return detail::base_to_real(-detail::real_to_base(a));
}
Real operator+ (const Real& a)
{
    return a;
}

bool operator< (const Real& a, const Real& b)
{
    return detail::real_to_base(a) < detail::real_to_base(b);
}
bool operator<= (const Real& a, const Real& b)
{
    return detail::real_to_base(a) <= detail::real_to_base(b);
}
bool operator== (const Real& a, const Real& b)
{
    return detail::real_to_base(a) == detail::real_to_base(b);
}
bool operator!= (const Real& a, const Real& b)
{
    return detail::real_to_base(a) != detail::real_to_base(b);
}
bool operator>= (const Real& a, const Real& b)
{
    return detail::real_to_base(a) >= detail::real_to_base(b);
}
bool operator> (const Real& a, const Real& b)
{
    return detail::real_to_base(a) > detail::real_to_base(b);
}

Real fabs (const Real& a)
{
    return detail::base_to_real(fabs(detail::real_to_base(a)));
}
Real abs (const Real& a)
{
    return fabs(a);
}
Real sqrt (const Real& a)
{
    return detail::base_to_real(sqrt(detail::real_to_base(a)));
}
Real floor (const Real& a)
{
    return detail::base_to_real(floor(detail::real_to_base(a)));
}
Real ceil (const Real& a)
{
    return detail::base_to_real(ceil(detail::real_to_base(a)));
}
Real trunc (const Real& a)
{
    return detail::base_to_real(trunc(detail::real_to_base(a)));
}
Real round (const Real& a)
{
    return detail::base_to_real(round(detail::real_to_base(a)));
}
Real exp (const Real& a)
{
    return detail::base_to_real(exp(detail::real_to_base(a)));
}
Real log (const Real& a)
{
    return detail::base_to_real(log(detail::real_to_base(a)));
}
Real log10 (const Real& a)
{
    return detail::base_to_real(log10(detail::real_to_base(a)));
}
Real cos (const Real& a)
{
    return detail::base_to_real(cos(detail::real_to_base(a)));
}
Real sin (const Real& a)
{
    return detail::base_to_real(sin(detail::real_to_base(a)));
}
Real tan (const Real& a)
{
    return detail::base_to_real(tan(detail::real_to_base(a)));
}
Real asin (const Real& a)
{
    return detail::base_to_real(asin(detail::real_to_base(a)));
}
Real acos (const Real& a)
{
    return detail::base_to_real(acos(detail::real_to_base(a)));
}
Real atan (const Real& a)
{
    return detail::base_to_real(atan(detail::real_to_base(a)));
}
Real cosh (const Real& a)
{
    return detail::base_to_real(cosh(detail::real_to_base(a)));
}
Real sinh (const Real& a)
{
    return detail::base_to_real(sinh(detail::real_to_base(a)));
}
Real tanh (const Real& a)
{
    return detail::base_to_real(tanh(detail::real_to_base(a)));
}

Real pow (const Real& a, const Real& b)
{
    return detail::base_to_real(pow(detail::real_to_base(a),detail::real_to_base(b)));
}
Real fmod (const Real& a, const Real& b)
{
    return detail::base_to_real(fmod(detail::real_to_base(a),detail::real_to_base(b)));
}
Real atan2 (const Real& a, const Real& b)
{
    return detail::base_to_real(atan2(detail::real_to_base(a),detail::real_to_base(b)));
}

std::ostream& operator<< (std::ostream& output, const Real& number)
{
    return output << detail::real_to_base(number).str(
        std::numeric_limits<Real>::digits10, output.flags()
    );
}

std::istream& operator>> (std::istream& input, Real& number)
{
    return input >> number.value;
}

} // namespace math


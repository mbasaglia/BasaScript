/**
\file

\author Mattia Basaglia

\section License

Copyright (C) 2015  Mattia Basaglia

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
#define BOOST_TEST_MODULE Test_Math

#include "math.hpp"

#include <sstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_real )
{
    using math::Real;

    BOOST_CHECK( Real(1) == 1 );
    BOOST_CHECK( Real("2") == 2 );
    BOOST_CHECK( Real(1ll) == 1 );
    BOOST_CHECK( Real(0.001,3) == Real("0.001") );
    BOOST_CHECK( Real("1.23456789") == Real(123456789) / math::pow(Real(10),8) );
    BOOST_CHECK( Real("0.99999999") < 1 );
    BOOST_CHECK( +Real(1) == 1 );
    BOOST_CHECK( Real("0.123456789") != Real("0.123456789000000000000000001") );

    Real sum_hundredth = 0;
    for (int i = 0; i < 100; i++)
	sum_hundredth += Real("0.01");
    BOOST_CHECK( sum_hundredth == 1 );

    Real string_output("1234.5678901234");
    std::stringstream ss;
    ss << string_output;
    Real string_input(ss.str());
    BOOST_CHECK( string_input == string_output );
    Real stream_input;
    ss >> stream_input;
    BOOST_CHECK( stream_input == string_output );

    Real r = 5;
    BOOST_CHECK(r++ == 5 && r == 6);
    BOOST_CHECK(r-- == 6 && r == 5);
    BOOST_CHECK(++r == 6 && r == 6);
    BOOST_CHECK(--r == 5 && r == 5);
    BOOST_CHECK((r -= 35, r == -30));
    BOOST_CHECK((r /= 6, r == -5)); /// \todo division by zero
    BOOST_CHECK((r *= -1, r == 5));

    BOOST_CHECK(math::ceil(Real("3.4")) == 4);
    BOOST_CHECK(math::ceil(Real("3.6")) == 4);
    BOOST_CHECK(math::ceil(-Real("3.4")) == -3);
    BOOST_CHECK(math::ceil(-Real("3.6")) == -3);
    BOOST_CHECK(math::floor(Real("3.4")) == 3);
    BOOST_CHECK(math::floor(Real("3.6")) == 3);
    BOOST_CHECK(math::floor(-Real("3.4")) == -4);
    BOOST_CHECK(math::floor(-Real("3.6")) == -4);
    BOOST_CHECK(math::round(Real("3.4")) == 3);
    BOOST_CHECK(math::round(Real("3.6")) == 4);
    BOOST_CHECK(math::round(Real("3.5")) == 4);
    BOOST_CHECK(math::round(-Real("3.4")) == -3);
    BOOST_CHECK(math::round(-Real("3.6")) == -4);
    BOOST_CHECK(math::round(-Real("3.5")) == -4);
    BOOST_CHECK(math::trunc(Real("3.4")) == 3);
    BOOST_CHECK(math::trunc(Real("3.6")) == 3);
    BOOST_CHECK(math::trunc(-Real("3.4")) == -3);
    BOOST_CHECK(math::trunc(-Real("3.6")) == -3);

    BOOST_CHECK( math::sin(math::pi/2) == 1 );
    BOOST_CHECK( math::cos(math::pi) == -1 );
    BOOST_CHECK( math::cos(math::pi/4) == math::sin(math::pi/4) );
    BOOST_CHECK( math::tan(math::pi/4) == 1 );
    BOOST_CHECK( math::acos(math::cos(1)) == 1 );
    BOOST_CHECK( math::asin(math::sin(1)) == 1 );
    BOOST_CHECK( math::atan(math::tan(1)) == 1 );
}

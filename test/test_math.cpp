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

#include "math/real.hpp"

#include <sstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test_real )
{
    using math::Real;

    // Constructors
    BOOST_CHECK( Real(1) == 1 );
    BOOST_CHECK( Real("2") == 2 );
    BOOST_CHECK( Real("1.23456789") == Real(123456789) / math::pow(Real(10),8) );
    BOOST_CHECK( math::from_float(0.001,3) == Real("0.001") );

    // Math/Precision
    BOOST_CHECK( Real("0.99999999") < 1 );
    BOOST_CHECK( math::sin(math::pi/2) == 1 );

    Real sum_hundredth = 0;
    for (int i = 0; i < 100; i++)
	sum_hundredth += Real("0.01");
    BOOST_CHECK( sum_hundredth == 1 );

    Real string_output("1234.5678901234");
    std::stringstream ss;
    //ss.precision(std::numeric_limits<Real>::digits10);
    ss << string_output;
    Real string_input(ss.str());
    BOOST_CHECK( string_input == string_output );


}

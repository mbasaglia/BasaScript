/**
\file

\author Mattia Basaglia

\section License

Copyright (C) 2014-2015  Mattia Basaglia

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

#ifndef GEO_POINT_HPP
#define GEO_POINT_HPP

#include "../math/real.hpp"

/**
 * \brief Namespace for geometric objects and operations
 */
namespace geo {

typedef math::Real Scalar; ///< Scalar in the vector space

/**
 * \brief A point in a 2D space
 */
struct Point
{
    Scalar x = 0; ///< X coordinate
    Scalar y = 0; ///< Y coordinate

    Point(Scalar x, Scalar y) : x(x), y(y) {}
    Point() {}

    Point& operator+= ( const Point& p )
    {
        x += p.x;
        y += p.y;
        return *this;
    }
    Point& operator-= ( const Point& p )
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    Point& operator*= ( Scalar factor )
    {
        x *= factor;
        y *= factor;
        return *this;
    }
    Point& operator/= ( Scalar factor )
    {
        x /= factor;
        y /= factor;
        return *this;
    }

    Point operator- () const
    {
        return Point(-x,-y);
    }
    Point operator+ () const
    {
        return *this;
    }

    Point operator+ (const Point&p) const
    {
        return Point(p.x+x,p.y+y);
    }
    Point operator- (const Point&p) const
    {
        return Point(x-p.x,y-p.y);
    }
    Point operator* (Scalar factor) const
    {
        return Point(x*factor,y*factor);
    }
    Point operator/ (Scalar factor) const
    {
        return Point(x/factor,y/factor);
    }

    bool operator== ( const Point& p ) const
    {
        return x == p.x && y == p.y;
    }

    bool operator!= ( const Point& p ) const
    {
        return x != p.x || y != p.y;
    }

    /**
     * \brief Distance from the origin
     */
    Scalar magnitude () const
    {
        return math::sqrt(x*x+y*y);
    }

    /**
     * \brief Distance to another Point
     */
    Scalar distance_to (const Point& o) const
    {
        return (*this-o).magnitude();
    }
};

Point operator* (Scalar factor, const Point&p)
{
    return Point(p.x*factor,p.y*factor);
}
/**
 * \brief 2-norm distance between two points
 */
inline Scalar distance (const Point& a, const Point& b)
{
    return a.distance_to(b);
}


/**
 * \brief A size ( width/height )
 */
struct Size
{
    Scalar width = 0;
    Scalar height= 0;

    Size ( Scalar width, Scalar height ) : width(width), height(height) {}
    Size(){}
};

} // namespace geo

#endif // GEO_POINT_HPP

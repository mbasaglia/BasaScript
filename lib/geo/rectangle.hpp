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

#ifndef GEO_RECTANGLE_HPP
#define GEO_RECTANGLE_HPP

#include <algorithm>
#include "point.hpp"


namespace geo {


/**
 * \brief A rectangle defined by its position (top left corner) and size
 */
struct Rectangle
{
    Scalar x = 0;
    Scalar y = 0;
    Scalar width = 0; ///< Width \note If not positive, renders the rectangle invalid
    Scalar height = 0;///< Height \note If not positive, renders the rectangle invalid

    /**
     * \brief Construct an empty rectangle
     */
    Rectangle() {}

    Rectangle(Scalar x, Scalar y, Scalar width, Scalar height )
        : x(x),y(y),width(width),height(height) {}

    Rectangle( const Point& pos, const Size& size )
        : x(pos.x), y(pos.y), width(size.width), height(size.height) {}

    Rectangle( const Point& top_left, const Point& bottom_right )
        : x(top_left.x), y(top_left.y), width(bottom_right.x-top_left.x), height(bottom_right.y-top_left.y) {}

    Scalar top() const { return y; }
    Scalar left() const { return x; }
    Scalar right() const { return x+width; }
    Scalar bottom() const { return y+height; }

    Point top_left() const { return Point(left(),top()); }
    Point bottom_right() const { return Point(right(),bottom()); }
    Point top_right() const { return Point(right(),top()); }
    Point bottom_left() const { return Point(left(),bottom()); }
    Point center() const { return Point(x+width/2,y+height/2); }

    Scalar area() const { return width*height; }
    Size size() const { return Size(width,height); }

    /**
     * \brief Get whether a rectangle contains the given point
     */
    bool contains(const Point& p) const
    {
        return p.x >= x && p.x <= x+width && p.y >= y && p.y <= y+height;
    }
    /**
     * \brief Get whether a rectangle contains the given point
     */
    bool contains(Scalar x, Scalar y) const
    {
        return contains(Point(x,y));
    }

    /**
     * \brief Whether the intersection between this and \c rect is not empty
     */
    bool intersects(const Rectangle& rect) const
    {
        return x < rect.right() && right() > rect.x && y < rect.bottom() && bottom() > rect.y;
    }

    /**
     * \brief Move the rectangle by the given offset
     */
    void translate(const Point& offset)
    {
        x += offset.x;
        y += offset.y;
    }
    /**
     * \brief Move the rectangle by the given offset
     */
    void translate(Scalar dx, Scalar dy)
    {
        translate(Point(dx,dy));
    }

    /**
     * \brief Get a rectangle moved by the given amount
     */
    Rectangle translated(const Point& offset) const
    {
        return Rectangle(top_left()+offset,size());
    }
    /**
     * \brief Get a rectangle moved by the given amount
     */
    Rectangle translated(Scalar dx, Scalar dy) const
    {
        return translated(Point(dx,dy));
    }

    /**
     * \brief Get the rectangle corresponding to the overlapping area between the two rectangles
     * \return The overlapping area or an invalid rectangle
     */
    Rectangle intersection(const Rectangle&rect) const
    {
        if ( !is_valid() || ! rect.is_valid() )
            return Rectangle();
        return Rectangle (
                Point(
                    std::max(x,rect.x),
                    std::max(y,rect.y)
                ),
                Point(
                    std::min(x+width,rect.x+rect.width),
                    std::min(y+height,rect.y+rect.height)
                )
            );
    }

    /**
     * \brief Get a rectangle large enough to contain both rectangles
     * \note If any rectangle is invalid, the other one is returned
     */
    Rectangle united(const Rectangle&rect) const
    {
        if ( !rect.is_valid() )
            return *this;
        if ( !is_valid() )
            return rect;
        return Rectangle(
                Point(
                    std::min(x,rect.x),
                    std::min(y,rect.y)
                ),
                Point(
                    std::max(x+width,rect.x+rect.width),
                    std::max(y+height,rect.y+rect.height)
                 )
            );
    }

    /**
     * \brief Grow this rectangle enough to contain rect
     */
    void unite(const Rectangle&rect)
    {
        *this = united(rect);
    }

    /**
     * \brief Unite
     */
    Rectangle& operator|= ( const Rectangle& rect )
    {
        return *this = united(rect);
    }

    /**
     * \brief Unite
     */
    Rectangle operator| ( const Rectangle& rect )
    {
        return united(rect);
    }

    /**
     * \brief Intersect
     */
    Rectangle& operator&= ( const Rectangle& rect )
    {
        return *this = intersection(rect);
    }

    /**
     * \brief Intersect
     */
    Rectangle operator& ( const Rectangle& rect )
    {
        return intersection(rect);
    }

    /**
     * \brief Whether the rectangle is valid (ie both \c width and \c height are positive)
     */
    bool is_valid() const
    {
        return width > 0 && height > 0;
    }

    bool operator==(const Rectangle& rect) const
    {
        return x == rect.x && y == rect.y && width == rect.width && height == rect.height;
    }
    bool operator!=(const Rectangle& rect) const
    {
        return !(*this == rect);
    }

    /**
     * \brief Get the point within the rectangle that's closest to the given point
     */
    Point nearest(const Point& p) const
    {
        return Point(
            p.x > right()  ? right()  : ( p.x < left()? left(): p.x ),
            p.y > bottom() ? bottom() : ( p.y < top() ? top() : p.y )
        );
    }

    /**
     * \brief Add (or subtract) the given margin from each edge
     */
    void expand(Scalar margin)
    {
        x -= margin;
        y -= margin;
        width += 2*margin;
        height += 2*margin;
    }

    /**
     * \brief Get a rectangle larger than the current one by the given margin
     */
    Rectangle expanded(Scalar margin) const
    {
        return Rectangle(x-margin,y-margin,width+2*margin,height+2*margin);
    }
};

} // namespace geo

#endif // GEO_RECTANGLE_HPP

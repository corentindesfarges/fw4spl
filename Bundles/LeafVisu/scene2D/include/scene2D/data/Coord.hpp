/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __SCENE2D_DATA_COORD_HPP__
#define __SCENE2D_DATA_COORD_HPP__

#include "scene2D/config.hpp"

#include <fwData/Object.hpp>


namespace scene2D
{
namespace data
{

class SCENE2D_CLASS_API Coord
{
public:

    Coord()
    {
        m_x = 0; m_y = 0;
    }
    Coord( double x, double y )
    {
        m_x = x; m_y = y;
    }
    double getX() const
    {
        return m_x;
    }
    void setX ( double x )
    {
        m_x = x;
    }
    double getY() const
    {
        return m_y;
    }
    void setY ( double y )
    {
        m_y = y;
    }

private:

    double m_x;
    double m_y;
};

} // namespace data
} // namespace scene2D


#endif // __SCENE2D_DATA_COORD_HPP__


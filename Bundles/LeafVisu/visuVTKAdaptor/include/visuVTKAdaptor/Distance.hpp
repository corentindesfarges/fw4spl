/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_DISTANCE_HPP__
#define __VISUVTKADAPTOR_DISTANCE_HPP__

#ifndef ANDROID

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/MeshFactory.hpp"

#include <fwData/Point.hpp>
#include <fwData/Color.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include <vector>

class vtkDistanceRepresentation2D;
class vtkActor;
class vtkLineSource;

namespace visuVTKAdaptor
{

class VISUVTKADAPTOR_CLASS_API Distance : public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Distance)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API Distance() throw();

    VISUVTKADAPTOR_API virtual ~Distance() throw();

    /// set Distance Axis color AND alpha
    VISUVTKADAPTOR_API void setAxisColor( ::fwData::Color::sptr newColor) throw();

protected:

    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStart();
    VISUVTKADAPTOR_API void doStop();

private:

    vtkDistanceRepresentation2D * m_distanceRepresentation;

    vtkActor          *m_lineActor;
    vtkLineSource     *m_lineSource;

    ::fwData::Point::wptr m_point1;
    ::fwData::Point::wptr m_point2;

    /// Connection between point 1 modified and this service reveive
    ::fwCom::Connection m_point1Connection;

    /// Connection between point 2 modified and this service reveive
    ::fwCom::Connection m_point2Connection;
};




} //namespace visuVTKAdaptor

#endif // ANDROID

#endif // __VISUVTKADAPTOR_DISTANCE_HPP__

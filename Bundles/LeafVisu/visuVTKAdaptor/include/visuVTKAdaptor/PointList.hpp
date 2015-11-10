/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_POINTLIST_HPP__
#define __VISUVTKADAPTOR_POINTLIST_HPP__

#ifndef ANDROID

#ifndef ANDROID

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/MeshFactory.hpp"

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include <fwData/Point.hpp>

#include <set>
#include <vector>

namespace visuVTKAdaptor
{

class VISUVTKADAPTOR_CLASS_API PointList : public ::fwRenderVTK::IVtkAdaptorService
{

public:
    typedef std::vector< WPTR(::fwData::Point) > WeakPointListType;
    typedef std::set< WPTR(::fwData::Point) > WeakPointSetType;

    fwCoreServiceClassDefinitionsMacro ( (PointList)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API PointList() throw();

    VISUVTKADAPTOR_API virtual ~PointList() throw();

protected:

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doReceive(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);

    VISUVTKADAPTOR_API void createServices(WeakPointListType &wPtList);
    VISUVTKADAPTOR_API WeakPointListType getWeakPointList();
    VISUVTKADAPTOR_API WeakPointListType getNewPoints();

    WeakPointListType m_oldWeakPointList;
    WeakPointListType m_weakPointList;

};
} //namespace visuVTKAdaptor

#endif // ANDROID

#endif // ANDROID

#endif // __VISUVTKADAPTOR_POINTLIST_HPP__

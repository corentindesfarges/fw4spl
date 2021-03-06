/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef ANDROID

#include "visuVTKAdaptor/PointList.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwData/Material.hpp>
#include <fwData/PointList.hpp>
#include <fwData/Reconstruction.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <algorithm>
#include <iterator>
#include <functional>

#include <vtkActor.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>

#include <boost/function.hpp>

fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::PointList, ::fwData::PointList );

namespace visuVTKAdaptor
{

static const ::fwCom::Slots::SlotKeyType s_ADD_POINT_SLOT     = "addPoint";
static const ::fwCom::Slots::SlotKeyType s_UPDATE_SPLINE_SLOT = "updateSpline";

//------------------------------------------------------------------------------

PointList::PointList() throw()
{
    newSlot(s_ADD_POINT_SLOT, &PointList::addPoint, this);
    newSlot(s_UPDATE_SPLINE_SLOT, &PointList::updateSpline, this);
}

//------------------------------------------------------------------------------

PointList::~PointList() throw()
{
}

//------------------------------------------------------------------------------

void PointList::configuring() throw(fwTools::Failed)
{
    assert(m_configuration->getName() == "config");
    this->setPickerId( m_configuration->getAttributeValue("picker") );
    this->setRenderId( m_configuration->getAttributeValue("renderer") );
}

//------------------------------------------------------------------------------

void PointList::doStart() throw(fwTools::Failed)
{
    m_oldWeakPointList.clear();

    m_weakPointList = this->getWeakPointList();

    this->doUpdate();
}

//------------------------------------------------------------------------------

void PointList::doUpdate() throw(fwTools::Failed)
{
    WeakPointListType points = this->getNewPoints();
    this->createServices( points );
}

//----------------------------------------------------------------------------------------------------------------

void PointList::addPoint(::fwData::Point::sptr /*point*/)
{
    m_oldWeakPointList = m_weakPointList;
    m_weakPointList    = this->getWeakPointList();
    this->doUpdate();
    this->setVtkPipelineModified();
}

//----------------------------------------------------------------------------------------------------------------

void PointList::updateSpline()
{
    this->doStop();
    this->doUpdate();
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void PointList::doSwap() throw(fwTools::Failed)
{
    this->doStop();
    this->doUpdate();
}

//------------------------------------------------------------------------------

void PointList::doStop() throw(fwTools::Failed)
{
    m_oldWeakPointList.clear();
    m_weakPointList.clear();
    this->unregisterServices();
}

//------------------------------------------------------------------------------

void PointList::createServices(WeakPointListType &wPtList)
{
    for( ::fwData::Point::wptr wpt :  wPtList )
    {
        SLM_ASSERT("Point Expired", !wpt.expired());

        ::fwData::Point::sptr pt                        = wpt.lock();
        ::fwRenderVTK::IVtkAdaptorService::sptr service =
            ::fwServices::add< ::fwRenderVTK::IVtkAdaptorService >
                ( pt, "::visuVTKAdaptor::Point" );
        SLM_ASSERT("service not instanced", service);

        service->setRenderService(this->getRenderService());
        service->setRenderId( this->getRenderId() );
        service->setPickerId( this->getPickerId() );
        service->setAutoRender( this->getAutoRender() );
        service->start();

        this->registerService(service);
    }
}

//------------------------------------------------------------------------------

PointList::WeakPointListType PointList::getWeakPointList()
{
    ::fwData::PointList::sptr ptList = this->getObject< ::fwData::PointList >();
    WeakPointListType weakList;

    std::copy(ptList->getRefPoints().begin(), ptList->getRefPoints().end(), std::back_inserter(weakList));

    return weakList;
}

//------------------------------------------------------------------------------

PointList::WeakPointListType PointList::getNewPoints()
{
    WeakPointListType newPoints;

//    std::set_difference (
//            m_weakPointList.begin(), m_weakPointList.end(),
//            m_oldWeakPointList.begin(), m_oldWeakPointList.end(),
//            std::back_inserter(newPoints)
//            );
    bool isFound;
    for(::fwData::Point::wptr point :  m_weakPointList)
    {
        isFound = false;
        for(::fwData::Point::wptr oldPoint :  m_oldWeakPointList)
        {
            isFound = (point.lock() == oldPoint.lock());
            if(isFound)
            {
                break;
            }
        }
        if(!isFound)
        {
            newPoints.push_back(point);
        }
    }
    return newPoints;
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsType PointList::getObjSrvConnections() const
{
    KeyConnectionsType connections;
    connections.push_back( std::make_pair( ::fwData::PointList::s_MODIFIED_SIG, s_UPDATE_SPLINE_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::PointList::s_POINT_ADDED_SIG, s_ADD_POINT_SLOT ) );

    return connections;
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor

#endif // ANDROID

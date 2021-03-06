/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef ANDROID

#include "visuVTKAdaptor/Distance.hpp"

#include <fwComEd/Dictionary.hpp>

#include <fwData/PointList.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwData/Material.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkAxisActor2D.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkHandleRepresentation.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>


fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::Distance, ::fwData::PointList );

namespace visuVTKAdaptor
{


Distance::Distance() throw() :
    m_distanceRepresentation( vtkDistanceRepresentation2D::New()),
    m_lineActor(vtkActor::New()),
    m_lineSource(vtkLineSource::New())
{
    m_distanceRepresentation->InstantiateHandleRepresentation();
    m_distanceRepresentation->SetLabelFormat("%-#6.3gmm");

    m_lineSource->SetResolution(1);
    vtkPolyDataMapper* lineMapper = vtkPolyDataMapper::New();
    lineMapper->SetInputConnection(m_lineSource->GetOutputPort());

    m_lineActor->SetMapper(lineMapper);
    m_lineActor->GetProperty()->SetLineWidth(5.);
    m_lineActor->GetProperty()->SetColor(1.,1.,0.);
    m_lineActor->GetProperty()->SetOpacity(0.4);

    lineMapper->Delete();
    m_distanceRepresentation->GetAxis()->GetProperty()->SetColor(1.0,0.0,1.);
    m_distanceRepresentation->GetAxis()->SetTitlePosition(0.9);

    //fixed font size for displaying distance
    //m_distanceRepresentation->GetAxis()->SizeFontRelativeToAxisOn();
    //m_distanceRepresentation->GetAxis()->GetMapper();
    //m_distanceRepresentation->GetAxis()->SetFontFactor(0.8);
}

//------------------------------------------------------------------------------

Distance::~Distance() throw()
{
    m_distanceRepresentation->Delete();
}

//------------------------------------------------------------------------------

void Distance::setAxisColor( ::fwData::Color::sptr newColor) throw()
{
    SLM_ASSERT("newColor not instanced", newColor);
    m_distanceRepresentation->GetAxis()->GetProperty()->SetColor(
        newColor->red(),newColor->green(),newColor->blue() );
    m_distanceRepresentation->GetAxis()->GetProperty()->SetOpacity(newColor->alpha() );
    m_lineActor->GetProperty()->SetColor(newColor->red(),newColor->green(),newColor->blue());
    m_lineActor->GetProperty()->SetOpacity( newColor->alpha() * 0.4);

    m_distanceRepresentation->GetAxis()->GetTitleTextProperty()->SetColor(
        newColor->red(), newColor->green(), newColor->blue() );
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void Distance::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    assert(m_configuration->getName() == "config");
    this->setRenderId( m_configuration->getAttributeValue("renderer") );
}

//------------------------------------------------------------------------------

void Distance::doStart()
{
    ::fwData::PointList::sptr ptList = this->getObject< ::fwData::PointList >();

    m_point1 = ptList->getPoints().front();
    m_point2 = ptList->getPoints().back();

    m_point1Connection = m_point1.lock()->signal(::fwData::Object::s_MODIFIED_SIG)->connect(
        this->slot(::fwServices::IService::s_UPDATE_SLOT));
    m_point2Connection = m_point2.lock()->signal(::fwData::Object::s_MODIFIED_SIG)->connect(
        this->slot(::fwServices::IService::s_UPDATE_SLOT));

    // set color to distance if Point List have Color Field
    if ( ptList->getField( ::fwComEd::Dictionary::m_colorId ) )
    {
        ::fwData::Color::sptr color;
        color = ptList->getField< ::fwData::Color >( ::fwComEd::Dictionary::m_colorId );
        this->setAxisColor( color );
    }

    this->addToRenderer(m_lineActor);
    this->addToRenderer(m_distanceRepresentation);

    this->doUpdate();
}

//------------------------------------------------------------------------------

void Distance::doUpdate() throw(fwTools::Failed)
{
    ::fwData::Point::sptr p1 = m_point1.lock();
    ::fwData::Point::sptr p2 = m_point2.lock();

    double ps1[3];
    std::copy(p1->getCRefCoord().begin(),(p1)->getCRefCoord().end(), ps1 );

    double ps2[3];
    std::copy(p2->getCRefCoord().begin(),(p2)->getCRefCoord().end(), ps2 );

    m_distanceRepresentation->GetPoint1Representation()->SetWorldPosition(ps1);
    m_distanceRepresentation->GetPoint2Representation()->SetWorldPosition(ps2);

    m_distanceRepresentation->SetPoint1DisplayPosition(ps1);
    m_distanceRepresentation->SetPoint2DisplayPosition(ps2);

    m_distanceRepresentation->BuildRepresentation();

    m_lineSource->SetPoint1(ps1);
    m_lineSource->SetPoint2(ps2);
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void Distance::doSwap() throw(fwTools::Failed)
{
    this->doStop();
    this->doStart();
    this->doUpdate();
}

//------------------------------------------------------------------------------

void Distance::doStop()
{
    m_point1Connection.disconnect();
    m_point2Connection.disconnect();

    m_point1.reset();
    m_point2.reset();

    this->removeAllPropFromRenderer();
}

//------------------------------------------------------------------------------


} //namespace visuVTKAdaptor

#endif // ANDROID

/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/InteractorStyle.hpp"

#include <fwData/Object.hpp>

#include <fwRenderVTK/IInteractorStyle.hpp>
#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>

#include <fwServices/registry/ObjectService.hpp>

#include <vtkInstantiator.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>

fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::InteractorStyle, ::fwData::Object );

namespace visuVTKAdaptor
{

//------------------------------------------------------------------------------

InteractorStyle::InteractorStyle() throw() : m_interactorStyle(nullptr)
{
}

//------------------------------------------------------------------------------

InteractorStyle::~InteractorStyle() throw()
{
    SLM_ASSERT("InteractorStyle should be NULL", !m_interactorStyle);
}

//------------------------------------------------------------------------------

void InteractorStyle::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    assert(m_configuration->getName() == "config");
    assert(m_configuration->hasAttribute("style"));
    this->setRenderId( m_configuration->getAttributeValue("renderer") );
    m_configuredStyle = m_configuration->getAttributeValue("style");
}

//------------------------------------------------------------------------------

void InteractorStyle::doStart() throw(fwTools::Failed)
{
    vtkObject* objectStyle         = vtkInstantiator::CreateInstance(m_configuredStyle.c_str());
    vtkInteractorStyle *interactor = vtkInteractorStyle::SafeDownCast(objectStyle);
    OSLM_ASSERT(
        "InsteractorStyle adaptor is waiting "
        "for a vtkInteractorStyle object, but '"
        << m_configuredStyle <<
        "' has been given.", interactor);
    this->setInteractorStyle(interactor);

}

//------------------------------------------------------------------------------

void InteractorStyle::doUpdate() throw(fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void InteractorStyle::doSwap() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void InteractorStyle::doStop() throw(fwTools::Failed)
{
    this->setInteractorStyle(nullptr);
}

//------------------------------------------------------------------------------

void InteractorStyle::setInteractorStyle(vtkInteractorStyle *interactor)
{
    if ( m_interactorStyle != nullptr )
    {
        m_interactorStyle->Delete();
        m_interactorStyle = nullptr;
    }

    ::fwRenderVTK::IInteractorStyle* fwInteractor = dynamic_cast< ::fwRenderVTK::IInteractorStyle* >(interactor);
    if(fwInteractor)
    {
        fwInteractor->setAutoRender(this->getAutoRender());
    }

    m_interactorStyle = interactor;


    this->getInteractor()->SetInteractorStyle(nullptr);
    this->getInteractor()->SetInteractorStyle(m_interactorStyle);
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------


} //namespace visuVTKAdaptor

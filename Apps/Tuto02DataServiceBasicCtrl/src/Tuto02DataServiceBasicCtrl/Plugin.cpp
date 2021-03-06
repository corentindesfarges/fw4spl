/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include <fwRuntime/EConfigurationElement.hpp>

#include <fwServices/op/Add.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwServices/registry/AppConfig.hpp>

#include "Tuto02DataServiceBasicCtrl/Plugin.hpp"


namespace Tuto02DataServiceBasicCtrl
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::Tuto02DataServiceBasicCtrl::Plugin");

//------------------------------------------------------------------------------

Plugin::Plugin() throw()
{
}

//------------------------------------------------------------------------------

Plugin::~Plugin() throw()
{
}

//------------------------------------------------------------------------------

void Plugin::start() throw( ::fwRuntime::RuntimeException )
{
}

//------------------------------------------------------------------------------

void Plugin::initialize() throw( ::fwRuntime::RuntimeException )
{
    m_image = ::fwData::Image::New();

    // Reader service
    m_readerSrv = ::fwServices::add(m_image, "::io::IReader", "::ioVTK::SImageReader");
    ::fwServices::IService::ConfigType readerCfg;
    readerCfg.put("service.file", "./TutoData/patient1.vtk");
    m_readerSrv->setConfiguration( readerCfg );
    m_readerSrv->configure();

    // Render service
    m_renderSrv = ::fwServices::add(m_image, "::fwRender::IRender", "::vtkSimpleNegato::SRenderer",
                                    "myRenderingTuto");
    m_renderSrv->configure();

    // Frame service
    m_frameSrv = ::fwServices::add(m_image, "::fwGui::IFrameSrv", "::gui::frame::SDefaultFrame");

    ::fwServices::IService::ConfigType frameConfig;

    frameConfig.put("service.gui.frame.name", "tutoDataServiceBasic");
    frameConfig.put("service.gui.frame.icon", "Bundles/Tuto02DataServiceBasicCtrl_0-1/tuto.ico");
    frameConfig.put("service.gui.frame.minSize.<xmlattr>.width", "800");
    frameConfig.put("service.gui.frame.minSize.<xmlattr>.height", "600");

    frameConfig.put("service.registry.view.<xmlattr>.sid", "myRenderingTuto");

    m_frameSrv->setConfiguration( frameConfig );
    m_frameSrv->configure();

    // Start app
    m_frameSrv->start();
    m_readerSrv->start();
    m_renderSrv->start();

    // Update
    m_readerSrv->update();
    m_renderSrv->update();
}

//------------------------------------------------------------------------------

void Plugin::stop() throw()
{
}

//------------------------------------------------------------------------------

void Plugin::uninitialize() throw()
{
    m_renderSrv->stop();
    m_readerSrv->stop();
    m_frameSrv->stop();
    ::fwServices::OSR::unregisterService( m_readerSrv );
    ::fwServices::OSR::unregisterService( m_frameSrv );
    ::fwServices::OSR::unregisterService( m_renderSrv );
    m_image.reset();
}

//------------------------------------------------------------------------------

} // namespace Tuto02DataServiceBasicCtrl

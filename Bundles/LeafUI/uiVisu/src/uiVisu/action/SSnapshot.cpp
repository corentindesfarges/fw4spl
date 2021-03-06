/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

#include <fwCore/base.hpp>

#include <fwData/location/SingleFile.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/Base.hpp>

#include <fwGui/dialog/LocationDialog.hpp>

#include "uiVisu/action/SSnapshot.hpp"

namespace uiVisu
{
namespace action
{

const ::fwCom::Signals::SignalKeyType SSnapshot::s_SNAPPED_SIG = "snapped";

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::uiVisu::action::SSnapshot, ::fwData::Object );


SSnapshot::SSnapshot() throw()
{
    m_sigSnapped = newSignal< SnappedSignalType >(s_SNAPPED_SIG);
}

//------------------------------------------------------------------------------

SSnapshot::~SSnapshot() throw()
{
}

//------------------------------------------------------------------------------

void SSnapshot::starting() throw(::fwTools::Failed)
{
    ::fwGui::IActionSrv::actionServiceStarting();
}

//------------------------------------------------------------------------------

void SSnapshot::stopping() throw(::fwTools::Failed)
{
    ::fwGui::IActionSrv::actionServiceStopping();
}

//------------------------------------------------------------------------------

void SSnapshot::configuring() throw(fwTools::Failed)
{
    ::fwGui::IActionSrv::initialize();
}

//------------------------------------------------------------------------------

void SSnapshot::updating() throw(::fwTools::Failed)
{
    std::string filename = this->requestFileName();

    if(!filename.empty())
    {
        m_sigSnapped->asyncEmit(filename);
    }
}

//------------------------------------------------------------------------------

void SSnapshot::swapping() throw(::fwTools::Failed)
{

}

//------------------------------------------------------------------------------

void SSnapshot::info( std::ostream &_sstream )
{
}

//------------------------------------------------------------------------------

std::string SSnapshot::requestFileName()
{
    std::string fileName = "";

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle("Save snapshot as");
    dialogFile.addFilter("Image file","*.jpg *.jpeg *.bmp *.png *.tiff");
    dialogFile.addFilter("jpeg","*.jpg *.jpeg");
    dialogFile.addFilter("bmp","*.bmp");
    dialogFile.addFilter("png","*.png");
    dialogFile.addFilter("tiff","*.tiff");
    dialogFile.addFilter("all","*.*");
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::WRITE);

    ::fwData::location::SingleFile::sptr result;
    result = ::fwData::location::SingleFile::dynamicCast( dialogFile.show() );
    if (result)
    {
        fileName = result->getPath().string();
    }

    return fileName;
}

//------------------------------------------------------------------------------

} // action
} // uiVisu


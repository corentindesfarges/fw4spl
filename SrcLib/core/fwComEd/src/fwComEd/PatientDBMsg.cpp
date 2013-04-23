/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwServices/registry/message/macros.hpp>

#include "fwComEd/PatientDBMsg.hpp"

fwServicesMessageRegisterMacro(  ::fwComEd::PatientDBMsg );

namespace fwComEd
{

//------------------------------------------------------------------------------

std::string PatientDBMsg::NEW_PATIENT = "NEW_PATIENT";
std::string PatientDBMsg::ADD_PATIENT = "ADD_PATIENT";
std::string PatientDBMsg::CLEAR_PATIENT = "CLEAR_PATIENT";
std::string PatientDBMsg::NEW_IMAGE_SELECTED = "NEW_IMAGE_SELECTED";
std::string PatientDBMsg::NEW_LOADED_PATIENT = "NEW_LOADED_PATIENT";
std::string PatientDBMsg::NEW_RESECTION_SELECTED = "NEW_RESECTION_SELECTED";

//------------------------------------------------------------------------------

PatientDBMsg::PatientDBMsg(::fwServices::ObjectMsg::Key key)
{}

//------------------------------------------------------------------------------

PatientDBMsg::~PatientDBMsg() throw()
{}

//------------------------------------------------------------------------------

} // namespace fwComEd

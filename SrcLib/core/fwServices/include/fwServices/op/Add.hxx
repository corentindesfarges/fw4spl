/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWSERVICES_OP_ADD_HXX__
#define __FWSERVICES_OP_ADD_HXX__

#include <fwData/Object.hpp>

#include "fwServices/registry/ObjectService.hpp"
#include "fwServices/IService.hpp"

namespace fwServices
{

//------------------------------------------------------------------------------

template<class SERVICE>
SPTR(SERVICE) add( ::fwData::Object::sptr obj, std::string _implementationId, std::string _id)
{
    std::string serviceType = ::fwCore::TypeDemangler< SERVICE >().getClassname();
    ::fwServices::IService::sptr service = ::fwServices::add( obj, serviceType, _implementationId, _id );
    OSLM_ASSERT("Failed to add "<<_implementationId, service );
    SPTR(SERVICE) castedService = std::dynamic_pointer_cast< SERVICE >( service );
    SLM_ASSERT("DynamicCast failed", castedService );
    return castedService;
}

//------------------------------------------------------------------------------

}

#endif /* __FWSERVICES_OP_ADD_HXX__ */

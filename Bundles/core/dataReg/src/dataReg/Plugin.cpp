/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>
#include <fwData/Float.hpp>
#include <fwMedData/Patient.hpp>

#include "dataReg/Plugin.hpp"

namespace dataReg
{

static ::fwRuntime::utils::GenericExecutableFactoryRegistrar<Plugin> registrar("::dataReg::Plugin");

Plugin::~Plugin() throw()
{
}

void Plugin::start() throw(::fwRuntime::RuntimeException)
{
    //Hack: force link with fwData
    ::fwData::Float::sptr obj = ::fwData::Float::New();
    obj->getClassname();

    //Hack: force link with fwMedData
    ::fwMedData::Patient::sptr pat = ::fwMedData::Patient::New();
    pat->getClassname();
}

void Plugin::stop() throw()
{
}

} // namespace dataReg

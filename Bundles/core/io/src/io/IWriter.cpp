/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
#include "io/IWriter.hpp"

#include <fwCore/base.hpp>
#include <fwServices/Base.hpp>

namespace io
{

//-----------------------------------------------------------------------------

IWriter::IWriter() throw()
{
}

//-----------------------------------------------------------------------------

IWriter::~IWriter() throw()
{
}

//-----------------------------------------------------------------------------

const ::boost::filesystem::path &IWriter::getFile() const
{
    FW_RAISE_IF("This reader doesn't manage file", !(this->getIOPathType() & ::io::FILE));
    FW_RAISE_IF("Sorry, one file must be define in location", m_locations.size() != 1);
    return m_locations.front();
}

//-----------------------------------------------------------------------------

void IWriter::setFile( const ::boost::filesystem::path &file)
{
    FW_RAISE_IF("This reader doesn't manage file", !(this->getIOPathType() & ::io::FILE));
    m_locations.clear();
    m_locations.push_back(file);
}

//-----------------------------------------------------------------------------

const ::io::LocationsType &IWriter::getFiles() const
{
    FW_RAISE_IF("This reader doesn't manage files", !(this->getIOPathType() & ::io::FILES));
    FW_RAISE_IF("Sorry, at least one file must be define in location", m_locations.empty() );
    return m_locations;
}

//-----------------------------------------------------------------------------

void IWriter::setFiles(const ::io::LocationsType &files)
{
    FW_RAISE_IF("This reader doesn't manage files", !(this->getIOPathType() & ::io::FILES));
    m_locations = files;
}

//-----------------------------------------------------------------------------

const ::boost::filesystem::path &IWriter::getFolder() const
{
    FW_RAISE_IF("This reader doesn't manage folder", !(this->getIOPathType() & ::io::FOLDER));
    FW_RAISE_IF("Sorry, one folder must be define in location", m_locations.size() !=1 );
    return m_locations.front();
}

//-----------------------------------------------------------------------------

void IWriter::setFolder(const ::boost::filesystem::path &folder)
{
    FW_RAISE_IF("This reader doesn't manage folder", !(this->getIOPathType() & ::io::FOLDER));
    m_locations.clear();
    m_locations.push_back(folder);
}

//-----------------------------------------------------------------------------

const ::io::LocationsType &IWriter::getLocations() const
{
    FW_RAISE_IF("Sorry, at least one pâth must be define in location", m_locations.empty() );
    return m_locations;
}

//-----------------------------------------------------------------------------

void IWriter::clearLocations()
{
    m_locations.clear();
}

//-----------------------------------------------------------------------------

void IWriter::configuring() throw (fwTools::Failed)
{
    SLM_ASSERT("Generic configuring method is just available for io service that uses pathes.",
               !( this->getIOPathType() & ::io::TYPE_NOT_DEFINED ) );

    SLM_ASSERT("Sorry, you not manage folder and a folder path is given in the configuration",
               ( this->getIOPathType() & ::io::FOLDER ) ||
               ((!(this->getIOPathType() & ::io::FOLDER)) && (m_configuration->find("folder").size() == 0)) );

    SLM_ASSERT("Sorry, you not manage file and a file path is given in the configuration",
               ( this->getIOPathType() & ::io::FILE || this->getIOPathType() & ::io::FILES ) ||
               ((!( this->getIOPathType() & ::io::FILE || this->getIOPathType() & ::io::FILES )) &&
                (m_configuration->find("file").size() == 0)) );

    if ( this->getIOPathType() & ::io::FILE )
    {
        FW_RAISE_IF("This reader cannot manages FILE and FILES.", this->getIOPathType() & ::io::FILES );
        std::vector< ::fwRuntime::ConfigurationElement::sptr > config = m_configuration->find("file");
        FW_RAISE_IF("Sorry, only one file must be defined in configuration", config.size() > 1 );
        if (config.size() == 1)
        {
            std::string file = config.at(0)->getValue();
            this->setFile(::boost::filesystem::path(file));
        }
    }


    if ( this->getIOPathType() & ::io::FILES )
    {
        FW_RAISE_IF("This reader cannot manages FILE and FILES.", this->getIOPathType() & ::io::FILE );
        std::vector< ::fwRuntime::ConfigurationElement::sptr > config = m_configuration->find("file");
        ::io::LocationsType locations;
        for(::fwRuntime::ConfigurationElement::sptr elt :  config)
        {
            std::string location = elt->getValue();
            locations.push_back(::boost::filesystem::path(location));
        }
        this->setFiles(locations);
    }


    if ( this->getIOPathType() & ::io::FOLDER )
    {
        std::vector< ::fwRuntime::ConfigurationElement::sptr > config = m_configuration->find("folder");
        FW_RAISE_IF("Sorry, only one folder must be defined in configuration", config.size() > 1 );
        if (config.size() == 1)
        {
            std::string folder = config.at(0)->getValue();
            this->setFolder(::boost::filesystem::path(folder));
        }
    }

}

//-----------------------------------------------------------------------------

::io::IOPathType IWriter::getIOPathType() const
{
    return ::io::TYPE_NOT_DEFINED;
}

//-----------------------------------------------------------------------------

bool IWriter::hasLocationDefined() const
{
    return m_locations.size() > 0;
}

//-----------------------------------------------------------------------------

}


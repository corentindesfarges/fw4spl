/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <sstream>

#include "fwRuntime/RuntimeException.hpp"
#include "fwRuntime/Version.hpp"


namespace fwRuntime
{

Version::Version() : m_major(-1),
                     m_minor(-1),
                     m_defined(false)
{
}

//------------------------------------------------------------------------------

Version::Version(const std::string & version) : m_major(-1),
                                                m_minor(-1),
                                                m_defined(false)
{
    if( version.empty() == false )
    {
        ::boost::regex regex ("(\\d+)[-.](\\d+){0,1}");
        ::boost::smatch what;
        if( ::boost::regex_match(version, what, regex) )
        {
            m_major   = ::boost::lexical_cast<int>( what[1].str() );
            m_minor   = ::boost::lexical_cast<int>( what[2].str() );
            m_defined = true;
        }
        else
        {
            throw RuntimeException(version + " : invalid version.");
        }
    }
}

//------------------------------------------------------------------------------

Version::Version(const int major, const int minor) :
    m_major(major),
    m_minor(minor),
    m_defined(true)
{
}

//------------------------------------------------------------------------------

const std::string Version::string() const
{
    std::ostringstream os;
    os << *this;
    return os.str();
}

//------------------------------------------------------------------------------

bool Version::operator==(const Version & version) const
{
    if( m_defined && version.m_defined )
    {
        return m_major == version.m_major && m_minor == version.m_minor;
    }
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------------

std::ostream & operator<<(std::ostream & os, const Version & version)
{
    if(version.m_defined)
    {
        return os << version.m_major << "-" << version.m_minor;
    }
    else
    {
        return os << "version-not-defined";
    }
}


} // namespace fwRuntime

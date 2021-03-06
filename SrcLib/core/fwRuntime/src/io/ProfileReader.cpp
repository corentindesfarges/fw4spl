/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwRuntime/io/ProfileReader.hpp"

#include <string>
#include <sstream>
#include <boost/filesystem/operations.hpp>
#include <libxml/parser.h>

#include "fwRuntime/io/Validator.hpp"
#include "fwRuntime/profile/Activater.hpp"
#include "fwRuntime/profile/Profile.hpp"
#include "fwRuntime/profile/Starter.hpp"
#include "fwRuntime/Runtime.hpp"
#include "fwRuntime/RuntimeException.hpp"

namespace fwRuntime
{

namespace io
{

std::string ProfileReader::ID                    ("id");
std::string ProfileReader::NAME                  ("name");
std::string ProfileReader::VALUE                 ("value");
std::string ProfileReader::VERSION               ("version");
std::string ProfileReader::CHECK_SINGLE_INSTANCE ("check-single-instance");
std::string ProfileReader::ACTIVATE              ("activate");
std::string ProfileReader::START                 ("start");
std::string ProfileReader::PARAM                 ("param");
std::string ProfileReader::DIS_EXT_PT            ("disable-extension-point");
std::string ProfileReader::DIS_EXT               ("disable-extension");

//------------------------------------------------------------------------------

std::shared_ptr< ::fwRuntime::profile::Profile > ProfileReader::createProfile( const boost::filesystem::path & path )
{
    // Normalizes the path.
    boost::filesystem::path normalizedPath(path);
    normalizedPath.normalize();


    // Asserts that the repository is a valid directory path.
    if(boost::filesystem::exists(normalizedPath) == false || boost::filesystem::is_directory(normalizedPath) == true)
    {
        throw RuntimeException("'" + normalizedPath.string() + "': not a a file.");
    }

    // Validation
    std::ostringstream fileLocation;
    fileLocation << "share/fwRuntime_" <<  FWRUNTIME_VER << "/profile.xsd";
    const ::boost::filesystem::path profileXSDLocation(
        ::fwRuntime::Runtime::getDefault()->getWorkingPath() / fileLocation.str() );

    Validator validator(profileXSDLocation);

    if( validator.validate(normalizedPath) == false )
    {
        throw RuntimeException(validator.getErrorLog());
    }

    // Get the document.
    xmlDocPtr document = xmlParseFile(normalizedPath.string().c_str());
    if(document == 0)
    {
        throw RuntimeException("Unable to read the profile file.");
    }

    try
    {
        // Get the root node.
        xmlNodePtr rootNode = xmlDocGetRootElement(document);

        char* pName    = (char *) xmlGetProp(rootNode, (const xmlChar*) NAME.c_str());
        char* pVersion = (char *) xmlGetProp(rootNode, (const xmlChar*) VERSION.c_str());
        char* pChkInst = (char *) xmlGetProp(rootNode, (const xmlChar*) CHECK_SINGLE_INSTANCE.c_str());

        SLM_ASSERT("Application profile MUST have a name attribute", pName);
        SLM_ASSERT("Application profile MUST have a version attribute", pVersion);

        std::string sName( pName );
        std::string sVersion( pVersion );
        bool checkSingleInstance = pChkInst && std::string(pChkInst) == "true";

        xmlFree(pName);
        xmlFree(pVersion);
        xmlFree(pChkInst);

        // Creates and process the profile element.
        std::shared_ptr< ::fwRuntime::profile::Profile > profile = processProfile(rootNode);

        profile->setFilePath(normalizedPath);
        profile->setName(sName);
        profile->setVersion(sVersion);
        profile->setCheckSingleInstance(checkSingleInstance);

        // Job's done!
        xmlFreeDoc(document);
        return profile;
    }
    catch(std::exception& exception)
    {
        xmlFreeDoc(document);
        throw;
    }
}

//------------------------------------------------------------------------------

std::shared_ptr< ::fwRuntime::profile::Profile > ProfileReader::processProfile(xmlNodePtr node)
{
    using namespace ::fwRuntime::profile;


    // Process child nodes.
    SPTR(Profile) profile = std::make_shared<Profile>();
    xmlNodePtr curChild = node->children;
    for(curChild = node->children; curChild != 0; curChild = curChild->next)
    {
        if(xmlStrcmp(curChild->name, (const xmlChar*) ACTIVATE.c_str()) == 0)
        {
            profile->add( processActivater(curChild) );
            continue;
        }

        if(xmlStrcmp(curChild->name, (const xmlChar*) START.c_str()) == 0)
        {
            profile->add( processStarter(curChild) );
            continue;
        }
    }
    return profile;
}

//------------------------------------------------------------------------------

std::shared_ptr< ::fwRuntime::profile::Activater > ProfileReader::processActivater(xmlNodePtr node)
{
    // Processes all attributes.
    xmlAttrPtr curAttr;
    std::string identifier;
    std::string version;
    for(curAttr = node->properties; curAttr != 0; curAttr = curAttr->next)
    {
        if(xmlStrcmp(curAttr->name, (const xmlChar*) ID.c_str()) == 0)
        {
            identifier = (const char*) curAttr->children->content;
            continue;
        }

        if(xmlStrcmp(curAttr->name, (const xmlChar*) VERSION.c_str()) == 0)
        {
            version = (const char*) curAttr->children->content;
            continue;
        }
    }

    // Creates the activater object.
    using ::fwRuntime::profile::Activater;
    std::shared_ptr< Activater >   activater( new Activater(identifier, version) );

    // Processes child node that are the parameters
    xmlNodePtr curChild = node->children;
    for(curChild = node->children; curChild != 0; curChild = curChild->next)
    {
        if(xmlStrcmp(curChild->name, (const xmlChar*) PARAM.c_str()) == 0)
        {
            processActivaterParam( curChild, activater );
            continue;
        }

        if(xmlStrcmp(curChild->name, (const xmlChar*) DIS_EXT_PT.c_str()) == 0)
        {
            processActivaterDisableExtensionPoint( curChild, activater );
            continue;
        }

        if(xmlStrcmp(curChild->name, (const xmlChar*) DIS_EXT.c_str()) == 0)
        {
            processActivaterDisableExtension( curChild, activater );
            continue;
        }
    }
    // Job's done.
    return activater;
}

//------------------------------------------------------------------------------

void ProfileReader::processActivaterParam(xmlNodePtr node, std::shared_ptr< ::fwRuntime::profile::Activater > activater)
{
    // Processes all attributes.
    xmlAttrPtr curAttr;
    std::string identifier;
    std::string value;
    for(curAttr = node->properties; curAttr != 0; curAttr = curAttr->next)
    {
        if(xmlStrcmp(curAttr->name, (const xmlChar*) ID.c_str()) == 0)
        {
            identifier = (const char*) curAttr->children->content;
            continue;
        }

        if(xmlStrcmp(curAttr->name, (const xmlChar*) VALUE.c_str()) == 0)
        {
            value = (const char*) curAttr->children->content;
            continue;
        }
    }
    // Stores the parameter into the activater.
    activater->addParameter( identifier, value );
}

//------------------------------------------------------------------------------

void ProfileReader::processActivaterDisableExtensionPoint(xmlNodePtr node,
                                                          std::shared_ptr< ::fwRuntime::profile::Activater > activater)
{
    // Processes all attributes.
    xmlAttrPtr curAttr;
    std::string identifier;
    for(curAttr = node->properties; curAttr != 0; curAttr = curAttr->next)
    {
        if(xmlStrcmp(curAttr->name, (const xmlChar*) ID.c_str()) == 0)
        {
            identifier = (const char*) curAttr->children->content;
            continue;
        }
    }

    // Stores the parameter into the activater.
    activater->addDisableExtensionPoint( identifier );
}

//------------------------------------------------------------------------------

void ProfileReader::processActivaterDisableExtension(xmlNodePtr node,
                                                     std::shared_ptr< ::fwRuntime::profile::Activater > activater)
{
    // Processes all attributes.
    xmlAttrPtr curAttr;
    std::string identifier;
    for(curAttr = node->properties; curAttr != 0; curAttr = curAttr->next)
    {
        if(xmlStrcmp(curAttr->name, (const xmlChar*) ID.c_str()) == 0)
        {
            identifier = (const char*) curAttr->children->content;
            continue;
        }
    }

    // Stores the parameter into the activater.
    activater->addDisableExtension( identifier );
}

//------------------------------------------------------------------------------

std::shared_ptr< ::fwRuntime::profile::Starter > ProfileReader::processStarter(xmlNodePtr node)
{
    // Processes all attributes.
    xmlAttrPtr curAttr;
    std::string identifier;
    for(curAttr = node->properties; curAttr != 0; curAttr = curAttr->next)
    {
        if(xmlStrcmp(curAttr->name, (const xmlChar*) ID.c_str()) == 0)
        {
            identifier = (const char*) curAttr->children->content;
            continue;
        }
    }

    // Creates the activater object.
    using ::fwRuntime::profile::Starter;
    std::shared_ptr< Starter > starter( new Starter(identifier) );
    return starter;
}

//------------------------------------------------------------------------------

} // namespace io

} // namespace fwRuntime

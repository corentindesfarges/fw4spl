/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWRUNTIME_ECONFIGURATIONELEMENT_HPP__
#define __FWRUNTIME_ECONFIGURATIONELEMENT_HPP__

#include "fwRuntime/ConfigurationElement.hpp"
#include "fwRuntime/config.hpp"

namespace fwRuntime
{


/**
 * @brief   Editable configuration element : allow the self configuration element building, which reading XML structure provided by bundle descriptions
 * @class   EConfigurationElement
 * @date    2006-2009
 *
 */
class FWRUNTIME_CLASS_API EConfigurationElement : public ::fwRuntime::ConfigurationElement
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (EConfigurationElement)(ConfigurationElement),
                                            ((( const std::string& ))),
                                            new EConfigurationElement
                                            );

    /**
     * @brief       Constructor.
     * @param[in]   name name of the configuration element
     */
    FWRUNTIME_API EConfigurationElement(const std::string& name);

    /**
     * @brief   Destructor : does nothing.
     */
    FWRUNTIME_API virtual ~EConfigurationElement();

    /**
     * @brief       Create and add a configuration element.
     * @param[in]   name name of the configuration element
     * @return      the created configuration element
     */
    FWRUNTIME_API EConfigurationElement::sptr addConfigurationElement( const std::string& name );

    /**
     * @brief       Adds a new configuration element to the extension.
     * @param[in]   element     a shared pointer to the configuration element to add
     */
    FWRUNTIME_API void addConfigurationElement( EConfigurationElement::sptr element );

    FWRUNTIME_API void setAttributeValue(const std::string& name, const std::string& value) throw();

    FWRUNTIME_API void setValue(const std::string& value) throw();
};

}

#endif /*__FWRUNTIME_ECONFIGURATIONELEMENT_HPP__*/

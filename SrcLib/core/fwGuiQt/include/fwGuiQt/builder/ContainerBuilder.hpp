/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQT_BUILDER_CONTAINERBUILDER_HPP__
#define __FWGUIQT_BUILDER_CONTAINERBUILDER_HPP__

#include <fwCore/base.hpp>
#include <fwRuntime/ConfigurationElement.hpp>

#include <fwGui/builder/IContainerBuilder.hpp>
#include <fwGui/container/fwContainer.hpp>

#include "fwGuiQt/config.hpp"
#include "fwGuiQt/container/QtContainer.hpp"

namespace fwGui
{
namespace builder
{

/**
 * @brief   Defines the generic layout manager for IHM.
 * @class   ContainerBuilder
 *
 * @date    2009-2010.
 *
 */
class FWGUIQT_CLASS_API ContainerBuilder : public ::fwGui::builder::IContainerBuilder
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (ContainerBuilder)(::fwGui::builder::IContainerBuilder),
                                            (()),
                                            ::fwGui::factory::New< ContainerBuilder > );

    FWGUIQT_API ContainerBuilder(::fwGui::GuiBaseObject::Key key);

    FWGUIQT_API virtual ~ContainerBuilder();

    /**
     * @brief Instantiate layout with parent container.
     * @pre LayoutManager must be initialized before.
     * @pre parent container must be instanced.
     */
    FWGUIQT_API virtual void createContainer( ::fwGui::container::fwContainer::sptr parent );

    /**
     * @brief Destroy local layout with sub containers.
     * @pre services using this sub containers must be stopped before.
     */
    FWGUIQT_API virtual void destroyContainer();

    FWGUIQT_API virtual void setParent(::fwGui::container::fwContainer::sptr parent);

protected:
    ::fwGuiQt::container::QtContainer::sptr m_parent;
};

} // namespace builder
} // namespace fwGui

#endif /*__FWGUIQT_BUILDER_CONTAINERBUILDER_HPP__*/



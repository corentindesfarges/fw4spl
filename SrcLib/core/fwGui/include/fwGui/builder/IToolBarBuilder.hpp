/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUI_BUILDER_ITOOLBARBUILDER_HPP__
#define __FWGUI_BUILDER_ITOOLBARBUILDER_HPP__

#include <fwRuntime/ConfigurationElement.hpp>

#include "fwGui/GuiBaseObject.hpp"
#include "fwGui/container/fwToolBar.hpp"
#include "fwGui/container/fwContainer.hpp"
#include "fwGui/config.hpp"

namespace fwGui
{

namespace builder
{

/**
 * @brief   Defines the interface class  for the toolbar builder.
 * @class   IToolBarBuilder
 *
 * @date    2009-2010.
 *
 */
class FWGUI_CLASS_API IToolBarBuilder : public ::fwGui::GuiBaseObject
{
public:
    fwCoreNonInstanciableClassDefinitionsMacro( (IToolBarBuilder)(::fwGui::GuiBaseObject) )

    typedef std::string RegistryKeyType;

    FWGUI_API const static RegistryKeyType REGISTRY_KEY;

    typedef enum
    {
        TOP,
        BOTTOM,
        RIGHT,
        LEFT
    } Aligment;


    /// Constructor. Do nothing.
    FWGUI_API IToolBarBuilder();

    /// Destructor. Do nothing.
    FWGUI_API virtual ~IToolBarBuilder();

    /**
     * @brief Returns the builded tool bar.
     */
    FWGUI_API virtual ::fwGui::container::fwToolBar::sptr getToolBar();

    /**
     * @brief Initialize the tool bar.
     *
     * Example of configuration
     * @verbatim
        <toolBar align="top">
            <toolBitmapSize height= "50" width="50" />
        </toolBar>
       @endverbatim
        - \b align : toolbar alignment (top, bottom, left, right)
     *  - \<toolBitmapSize height= "50" width="50"/\> : give the size of the icon.
     */

    FWGUI_API virtual void initialize( ::fwRuntime::ConfigurationElement::sptr configuration);

    /**
     * @brief Instantiate layout with parent toolBar.
     * @pre LayoutManager must be initialized before.
     * @pre parent toolBar must be instanced.
     */
    FWGUI_API virtual void createToolBar( ::fwGui::container::fwContainer::sptr parent ) = 0;

    /**
     * @brief Destroy local layout with sub containers.
     * @pre services using this sub containers must be stopped before.
     */
    FWGUI_API virtual void destroyToolBar() = 0;

protected:

    /// ToolBar.
    ::fwGui::container::fwToolBar::sptr m_toolBar;

    std::pair< int, int > m_toolBitmapSize;

    Aligment m_aligment;

};

} // namespace builder
} // namespace fwGui

#endif /*__FWGUI_BUILDER_ITOOLBARBUILDER_HPP__*/



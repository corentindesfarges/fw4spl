/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GUIQT_EDITOR_DYNAMICVIEW_HPP__
#define __GUIQT_EDITOR_DYNAMICVIEW_HPP__

#include "guiQt/config.hpp"

#include <fwActivities/registry/Activities.hpp>
#include <fwActivities/registry/ActivityMsg.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include <fwServices/AppConfigManager.hpp>

#include <fwTools/Failed.hpp>

#include <gui/view/IView.hpp>

#include <QObject>
#include <QPointer>

#include <map>
#include <set>


class QTabWidget;
namespace fwData
{
class Composite;
}
namespace guiQt
{
namespace editor
{

typedef const ::fwServices::IService::ConfigType ConfigType;

struct AppConfig
{
    typedef ::fwActivities::registry::ActivityAppConfigParam ParameterType;
    typedef std::vector< ParameterType > ParametersType;

    AppConfig()
    {
    }
    AppConfig(const ConfigType &config);

    std::string id;
    std::string title;
    std::string tabInfo;
    bool closable;
    ParametersType parameters;
};


/**
 * @class   DynamicView
 * @brief   This editor manages tabs. It receive message with NEW_CONFIGURATION_HELPER event containing the view config id.
 *
 * @note The ::activities::action::SActivityLauncher action sends message to be receive by the editor.
 */
class GUIQT_CLASS_API DynamicView : public QObject,
                                    public ::gui::view::IView
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro ( (DynamicView)(::gui::view::IView) );

    /// Constructor. Do nothing.
    GUIQT_API DynamicView() throw();

    /// Destructor. Do nothing.
    GUIQT_API virtual ~DynamicView() throw();

protected:

    /**
     * @brief Install the container.
     */
    virtual void starting() throw(::fwTools::Failed);

    /**
     * @brief Destroy the container.
     */
    virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief Update
     *
     */
    virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief Swap
     *
     * Update the editors.
     */
    virtual void swapping() throw(::fwTools::Failed);

    /**
     * @brief Configure the view
     * @see fwGui::IGuiContainerSrv::initialize()
     *
     * @verbatim
       <service type="::gui::view::IView" impl="::guiQt::editor::DynamicView" autoConnect="yes" >
        <config dynamicConfigStartStop="false">
            <appConfig id="Visu2DID" title="Visu2D" >
                <parameters>
                    <parameter replace="SERIESDB" by="medicalData"  />
                    <parameter replace="IMAGE" by="@values.image"  />
                </parameters>
            </appConfig>
        </config>
       </service>
       @endverbatim
     */
    virtual void configuring() throw(fwTools::Failed);

    virtual void info( std::ostream &_sstream );


    typedef std::map< std::string, std::string> ReplaceMapType;
    struct DynamicViewInfo
    {
        ::fwGuiQt::container::QtContainer::sptr container;
        ::fwServices::AppConfigManager::sptr helper;
        std::string wid;
        std::string title;
        bool closable;
        std::string icon;
        std::string tooltip;
        std::string tabID;
        std::string viewConfigID;
        ReplaceMapType replaceMap;
    };

    typedef std::map< QWidget*, DynamicViewInfo > DynamicViewInfoMapType;

    /**
     * @brief Launch tab
     */
    void launchTab(DynamicViewInfo& info);

    /// Slot: launch a new tab according to the receiving msg
    void createTab(::fwActivities::registry::ActivityMsg info);

    /**
     * @brief Build a DynamicViewInfo from an AppConfig
     */
    DynamicViewInfo buildDynamicViewInfo(const AppConfig& appConfig);

    QPointer<QTabWidget> m_tabWidget;
    QPointer<QWidget> m_currentWidget;

    std::map< std::string, unsigned int > m_titleToCount;
    std::set< std::string > m_tabIDList;

    DynamicViewInfoMapType m_dynamicInfoMap;
    bool m_dynamicConfigStartStop;

    AppConfig m_appConfig;

protected Q_SLOTS:

    void closeTab( int index, bool forceClose );

    void closeTabSignal(int index);

    void changedTab(int index);

};

}//namespace editor
} // guiQt

#endif /*__GUIQT_EDITOR_DYNAMICVIEW_HPP__*/



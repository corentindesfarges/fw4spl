/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "guiQt/editor/SSelectionMenuButton.hpp"


#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Signals.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwComEd/Dictionary.hpp>

#include <fwCore/base.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <fwTools/fwID.hpp>

#include <QWidget>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QVBoxLayout>

#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>

namespace guiQt
{
namespace editor
{

fwServicesRegisterMacro( ::gui::editor::IEditor, ::guiQt::editor::SSelectionMenuButton, ::fwData::Object );

static const ::fwCom::Signals::SignalKeyType s_SELECTED_SIG = "selected";

static const ::fwCom::Slots::SlotKeyType s_SET_ENABLED_SIG = "setEnabled";
static const ::fwCom::Slots::SlotKeyType s_SENABLE_SIG     = "enable";
static const ::fwCom::Slots::SlotKeyType s_DISABLE_SIG     = "disable";

SSelectionMenuButton::SSelectionMenuButton() throw() :
    m_text(">"),
    m_selection(0)
{
    m_sigSelected = newSignal < SelectedSignalType >(s_SELECTED_SIG);

    newSlot(s_SET_ENABLED_SIG, &SSelectionMenuButton::setEnabled, this);
    newSlot(s_SENABLE_SIG, &SSelectionMenuButton::enable, this);
    newSlot(s_DISABLE_SIG, &SSelectionMenuButton::disable, this);
}

//------------------------------------------------------------------------------

SSelectionMenuButton::~SSelectionMenuButton() throw()
{
}

//------------------------------------------------------------------------------

void SSelectionMenuButton::configuring() throw(fwTools::Failed)
{
    this->initialize();

    Configuration txtCfg = m_configuration->findConfigurationElement("text");
    if(txtCfg)
    {
        m_text = txtCfg->getValue();
    }
    Configuration toolTipCfg = m_configuration->findConfigurationElement("toolTip");
    if(toolTipCfg)
    {
        m_toolTip = toolTipCfg->getValue();
    }

    Configuration selectedCfg = m_configuration->findConfigurationElement("selected");
    if(selectedCfg)
    {
        m_selection = std::stoi(selectedCfg->getValue());
    }

    Configuration itemsCfg = m_configuration->findConfigurationElement("items");
    SLM_ASSERT("Missing 'items' config", itemsCfg);

    std::vector < Configuration > itemCfgs = itemsCfg->find("item");
    SLM_ASSERT("At least two items must be defined", itemCfgs.size() >= 2);
    for (auto itemCfg : itemCfgs)
    {
        SLM_ASSERT("Missing 'text' attribute", itemCfg->hasAttribute("text"));
        SLM_ASSERT("Missing 'value' attribute", itemCfg->hasAttribute("value"));
        std::string txt = itemCfg->getExistingAttributeValue("text");
        std::string val = itemCfg->getExistingAttributeValue("value");
        int value       = std::stoi(val);
        m_items.push_back(std::make_pair(value, txt));
    }
}

//------------------------------------------------------------------------------

void SSelectionMenuButton::starting() throw(::fwTools::Failed)
{
    this->create();

    ::fwGuiQt::container::QtContainer::sptr qtContainer = ::fwGuiQt::container::QtContainer::dynamicCast(
        this->getContainer() );
    QWidget* const container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);

    m_dropDownButton = new QPushButton( QString::fromStdString(m_text), container );
    m_dropDownButton->setToolTip( QString::fromStdString(m_toolTip));
//    m_dropDownButton->setMaximumWidth(40);

    m_pDropDownMenu = new QMenu(container);
    m_actionGroup   = new QActionGroup(m_pDropDownMenu);

    for (auto item : m_items)
    {
        QAction * action = new QAction(QString::fromStdString(item.second), m_pDropDownMenu);
        action->setCheckable(true);
        action->setData(QVariant(item.first));
        m_actionGroup->addAction(action);
        m_pDropDownMenu->addAction(action);

        if (item.first == m_selection)
        {
            action->setChecked(true);
        }
    }
    QObject::connect(m_actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(onSelection(QAction*)));
    m_dropDownButton->setMenu(m_pDropDownMenu);

    QVBoxLayout * vLayout = new QVBoxLayout(container);
    vLayout->addWidget( m_dropDownButton);
    vLayout->setContentsMargins(0,0,0,0);

    container->setLayout( vLayout );
}

//------------------------------------------------------------------------------

void SSelectionMenuButton::stopping() throw(::fwTools::Failed)
{
    QObject::connect(m_actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(onSelection(QAction*)));
    for (QAction* action : m_actionGroup->actions())
    {
        m_actionGroup->removeAction(action);
    }

    this->getContainer()->clean();
    this->destroy();
}

//------------------------------------------------------------------------------

void SSelectionMenuButton::updating() throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void SSelectionMenuButton::swapping() throw(::fwTools::Failed)
{

}

//------------------------------------------------------------------------------

void SSelectionMenuButton::info( std::ostream &_sstream )
{
}

//------------------------------------------------------------------------------

void SSelectionMenuButton::onSelection(QAction* action)
{
    if (action->isChecked())
    {
        int value = action->data().toInt();
        m_sigSelected->asyncEmit(value);
        return;
    }
}

//------------------------------------------------------------------------------

void SSelectionMenuButton::setEnabled(bool enabled)
{
    m_dropDownButton->setEnabled(enabled);
}

//------------------------------------------------------------------------------

void SSelectionMenuButton::enable()
{
    this->setEnabled(true);
}

//------------------------------------------------------------------------------

void SSelectionMenuButton::disable()
{
    this->setEnabled(false);
}

//------------------------------------------------------------------------------

} // namespace editor
} // namespace guiQt


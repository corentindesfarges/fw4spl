/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIMEASUREMENTQT_EDITOR_DISTANCE_HPP__
#define __UIMEASUREMENTQT_EDITOR_DISTANCE_HPP__

#include <QObject>
#include <QPointer>
#include <QPushButton>

#include <fwTools/Failed.hpp>
#include <gui/editor/IEditor.hpp>

#include "uiMeasurementQt/config.hpp"

namespace uiMeasurement
{
namespace editor
{
/**
 * @brief   Distance service is represented by a button. It allows to show distances in a generic scene.
 * @class   Distance
 *
 * @note Sends a signal to request a distance. It should be conected to an ImageMultiDistance adaptor to create the
 * distance.
 */
class UIMEASUREMENTQT_CLASS_API Distance : public QObject,
                                           public ::gui::editor::IEditor
{
Q_OBJECT
public:

    fwCoreServiceClassDefinitionsMacro ( (Distance)(::gui::editor::IEditor) );

    /// Constructor. Do nothing.
    UIMEASUREMENTQT_API Distance() throw();

    /// Destructor. Do nothing.
    UIMEASUREMENTQT_API virtual ~Distance() throw();

    /**
     * @name Signal
     * @{
     */
    UIMEASUREMENTQT_API static const ::fwCom::Signals::SignalKeyType s_DISTANCE_REQUESTED_SIG;
    typedef ::fwCom::Signal< void () > DistanceRequestedSignalType;
    /**
     * @}
     */
protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    /**
     * @brief Install the layout.
     *
     * This method launches the IEditor::starting method.
     */
    virtual void starting() throw(::fwTools::Failed);

    /**
     * @brief Destroy the layout.
     *
     * This method launches the IEditor::stopping method.
     */
    virtual void stopping() throw(::fwTools::Failed);

    /// Do nothing
    virtual void updating() throw(::fwTools::Failed);

    /// Do nothing
    virtual void swapping() throw(::fwTools::Failed);

    /**
     * @brief Configure the editor.
     *
     * Example of configuration
     * @verbatim
       <service uid="distanceEditor" type="::gui::editor::IEditor" impl="::uiMeasurement::editor::Distance" autoConnect="no">
        <placeInScene uid="genericScene" />
       </service>
       @endverbatim
       \b genericScene is the uid of the ::fwRenderVTK::VtkRenderService representing the generic scene which will be printed.
     */
    virtual void configuring() throw(fwTools::Failed);

    /// Overrides
    virtual void info( std::ostream &_sstream );

protected Q_SLOTS:
    /**
     * @brief Notify the scene that a distance is added.
     */
    void onDistanceButton();

private:

    std::string m_scenesUID;
    QPointer< QPushButton > m_distButton;

    DistanceRequestedSignalType::sptr m_sigDistanceRequested; /// signal emitted when a distance is requested



};
} // namespace editor
} // uiMeasurement

#endif /*__UIMEASUREMENTQT_EDITOR_DISTANCE_HPP__*/



/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIVISUQT_POINTEDITOR_HPP__
#define __UIVISUQT_POINTEDITOR_HPP__



#include "uiVisuQt/config.hpp"

#include <fwComEd/PickingInfo.hpp>

#include <fwData/Point.hpp>

#include <fwTools/Failed.hpp>

#include <gui/editor/IEditor.hpp>

#include <QLineEdit>
#include <QObject>
#include <QPointer>


namespace uiVisu
{

/**
 * @brief   PointEditor service allows to display point information.
 * @class   PointEditor
 */
class UIVISUQT_CLASS_API PointEditor : public QObject,
                                       public ::gui::editor::IEditor
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro ( (PointEditor)(::gui::editor::IEditor) );

    /// Constructor. Do nothing.
    UIVISUQT_API PointEditor() throw();

    /// Destructor. Do nothing.
    UIVISUQT_API virtual ~PointEditor() throw();

protected:


    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    ///This method launches the IEditor::starting method.
    virtual void starting() throw(::fwTools::Failed);

    ///This method launches the IEditor::stopping method.
    virtual void stopping() throw(::fwTools::Failed);

    void updating() throw(::fwTools::Failed);

    void swapping() throw(::fwTools::Failed);

    void configuring() throw( ::fwTools::Failed);

    /// Overrides
    virtual void info( std::ostream &_sstream );


private:

    /// Slot: get the interaction information
    void getInteraction(::fwComEd::PickingInfo info);

    QPointer< QLineEdit >   m_textCtrl_x;
    QPointer< QLineEdit >   m_textCtrl_y;
    QPointer< QLineEdit >   m_textCtrl_z;

};

} // uiData

#endif /*__UIVISUQT_POINTEDITOR_HPP___*/

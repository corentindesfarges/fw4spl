/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIMEDDATAQT_EDITOR_SORGANTRANSFORMATION_HPP__
#define __UIMEDDATAQT_EDITOR_SORGANTRANSFORMATION_HPP__

#include <QPointer>
#include <QObject>

#include <map>
#include <string>

#include <fwData/Reconstruction.hpp>
#include <fwData/TransformationMatrix3D.hpp>
#include <gui/editor/IEditor.hpp>

#include "uiMedDataQt/config.hpp"


class QListWidget;
class QPushButton;
class QListWidgetItem;
class QComboBox;
class QCheckBox;

namespace uiMedData
{
namespace editor
{

/**
 * @brief Display the organs list and allow an interactive selection to set the corresponding meshes in a composite
 */
class UIMEDDATAQT_CLASS_API SOrganTransformation : public QObject,
                                                   public ::gui::editor::IEditor
{
Q_OBJECT
public:

    fwCoreServiceClassDefinitionsMacro( (SOrganTransformation)(::gui::editor::IEditor) );

    /// constructor
    UIMEDDATAQT_API SOrganTransformation() throw();
    /// destructor
    UIMEDDATAQT_API virtual ~SOrganTransformation() throw();


    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect ModelSeries::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     * Connect ModelSeries::s_RECONSTRUCTIONS_ADDED_SIG to this::s_UPDATE_SLOT
     * Connect ModelSeries::s_RECONSTRUCTIONS_REMOVED_SIG to this::s_UPDATE_SLOT
     */
    UIMEDDATAQT_API virtual KeyConnectionsType getObjSrvConnections() const;

protected:

    /**
     * @brief configures the service
     * @verbatim
       <service impl="::uiMedData::editor::SOrganTransformation" type="::gui::editor::IEditor" autoConnect="yes">
           <TMSUid>MESHDB_UID</TMSUid>
       </service>
       @endverbatim
     * - \b TMSUid = composite uid
     */
    UIMEDDATAQT_API virtual void configuring() throw( ::fwTools::Failed );
    UIMEDDATAQT_API virtual void starting()    throw( ::fwTools::Failed );
    UIMEDDATAQT_API virtual void stopping()    throw( ::fwTools::Failed );
    UIMEDDATAQT_API virtual void swapping()    throw( ::fwTools::Failed );
    UIMEDDATAQT_API virtual void updating()    throw( ::fwTools::Failed );
    UIMEDDATAQT_API virtual void info    ( ::std::ostream& stream );

private Q_SLOTS:
    void onReconstructionCheck(QListWidgetItem *currentItem);
    void onResetClick();
    void onSaveClick();
    void onLoadClick();

    /// Slot to check all the organs of the list
    void onSelectAllChanged(int state);

private:

    void refresh();
    void notitfyTransformationMatrix(::fwData::TransformationMatrix3D::sptr aTransMat);

    // ReconstructionMapType
    typedef ::std::map< ::std::string, ::fwData::Reconstruction::sptr > ReconstructionMapType;
    typedef ::std::map< ::std::string, ::fwData::TransformationMatrix3D::sptr> InnerMatMappingType;
    typedef ::std::map< ::std::string, InnerMatMappingType> SaveMappingType;

    ReconstructionMapType m_reconstructionMap;
    ::std::string m_TMSUid;
    QPointer< QPushButton > m_saveButton;
    QPointer< QPushButton > m_loadButton;
    QPointer< QPushButton > m_resetButton;
    QPointer< QListWidget > m_reconstructionListBox;
    QPointer< QComboBox > m_saveSelectionComboBox;
    QPointer< QCheckBox > m_selectAllCheckBox;

    //variables for the functionalities of saving & loading
    SaveMappingType m_saveListing;
    unsigned int m_saveCount;
};

} // namespace editor
} // namespace uiMedData


#endif // __UIMEDDATAQT_EDITOR_SORGANTRANSFORMATION_HPP__

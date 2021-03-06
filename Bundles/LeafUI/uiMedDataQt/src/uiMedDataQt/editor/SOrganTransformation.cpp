/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "uiMedDataQt/editor/SOrganTransformation.hpp"

#include <fwComEd/helper/Composite.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Material.hpp>
#include <fwData/Mesh.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwDataTools/TransformationMatrix3D.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include <fwMedData/ModelSeries.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>
#include <fwTools/fwID.hpp>

#include <QListWidget>
#include <QListWidgetItem>
#include <QStringList>
#include <QString>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>

#include <map>

namespace uiMedData
{
namespace editor
{

fwServicesRegisterMacro( ::gui::editor::IEditor, ::uiMedData::editor::SOrganTransformation, ::fwMedData::ModelSeries );

SOrganTransformation::SOrganTransformation() throw() :
    m_saveButton( 0 ),
    m_loadButton( 0 ),
    m_resetButton( 0 ),
    m_reconstructionListBox( 0 ),
    m_saveCount( 0 )
{
}

//------------------------------------------------------------------------------

SOrganTransformation::~SOrganTransformation() throw()
{
}

//------------------------------------------------------------------------------

void SOrganTransformation::configuring() throw( ::fwTools::Failed )
{
    this->initialize();
    if( m_configuration->findConfigurationElement( "TMSUid" ) )
    {
        m_TMSUid = m_configuration->findConfigurationElement( "TMSUid" )->getValue();
    }
}

//------------------------------------------------------------------------------

void SOrganTransformation::starting() throw( ::fwTools::Failed )
{
    this->create();
    ::fwGuiQt::container::QtContainer::sptr qtContainer =
        ::fwGuiQt::container::QtContainer::dynamicCast( this->getContainer() );
    QWidget* const container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);

    QVBoxLayout* layout = new QVBoxLayout(container);

    QGroupBox* groupBox = new QGroupBox(tr("Organs"), container );
    layout->addWidget( groupBox);

    QVBoxLayout* layoutGroupBox = new QVBoxLayout(container);
    groupBox->setLayout(layoutGroupBox);

    m_selectAllCheckBox     = new QCheckBox(tr("Select All"), container );
    m_reconstructionListBox = new QListWidget( groupBox);
    m_resetButton           = new QPushButton(tr("Reset"), container );
    m_saveButton            = new QPushButton(tr("Save"), container );
    m_loadButton            = new QPushButton(tr("Load"), container );
    m_saveSelectionComboBox = new QComboBox(container);


    QObject::connect(m_reconstructionListBox,
                     SIGNAL(itemClicked (QListWidgetItem * )),
                     this,
                     SLOT(onReconstructionCheck(QListWidgetItem *)));
    QObject::connect(m_resetButton, SIGNAL(clicked( )), this, SLOT(onResetClick()));
    QObject::connect(m_saveButton, SIGNAL(clicked( )), this, SLOT(onSaveClick()));
    QObject::connect(m_loadButton, SIGNAL(clicked( )), this, SLOT(onLoadClick()));
    QObject::connect(m_selectAllCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onSelectAllChanged(int)));

    layoutGroupBox->addWidget( m_selectAllCheckBox, 0);
    layoutGroupBox->addWidget( m_reconstructionListBox, 1);
    layoutGroupBox->addWidget( m_resetButton, 0);
    layoutGroupBox->addWidget( m_saveButton, 0);
    layoutGroupBox->addWidget( m_saveSelectionComboBox,0);
    layoutGroupBox->addWidget( m_loadButton, 0);

    container->setLayout( layout );

    this->updating();
}

//------------------------------------------------------------------------------

void SOrganTransformation::stopping() throw( ::fwTools::Failed )
{
    QObject::disconnect(m_reconstructionListBox,
                        SIGNAL(itemClicked (QListWidgetItem * )),
                        this,
                        SLOT(onReconstructionCheck(QListWidgetItem *)));
    QObject::disconnect(m_resetButton, SIGNAL(clicked( )), this, SLOT(onResetClick()));
    QObject::disconnect(m_saveButton, SIGNAL(clicked( )), this, SLOT(onSaveClick()));
    QObject::disconnect(m_loadButton, SIGNAL(clicked( )), this, SLOT(onLoadClick()));
    QObject::disconnect(m_selectAllCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onSelectAllChanged(int)));

    this->getContainer()->clean();
    this->destroy();
}

//------------------------------------------------------------------------------

void SOrganTransformation::swapping() throw( ::fwTools::Failed )
{
    this->updating();
}

//------------------------------------------------------------------------------

void SOrganTransformation::updating() throw( ::fwTools::Failed )
{
    this->refresh();
}

//------------------------------------------------------------------------------

void SOrganTransformation::info( ::std::ostream& sstream )
{
}

//------------------------------------------------------------------------------

void SOrganTransformation::refresh()
{
    m_reconstructionMap.clear();
    m_reconstructionListBox->clear();

    ::fwMedData::ModelSeries::sptr series = this->getObject< ::fwMedData::ModelSeries >();

    ::fwGuiQt::container::QtContainer::sptr qtContainer =
        ::fwGuiQt::container::QtContainer::dynamicCast( this->getContainer() );
    QWidget* const container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);

    bool hasReconstructions = !series->getReconstructionDB().empty();
    container->setEnabled( hasReconstructions );

    if(hasReconstructions)
    {
        ::fwData::Composite::sptr pComposite;
        if (::fwTools::fwID::exist(m_TMSUid))
        {
            pComposite = ::fwData::Composite::dynamicCast(::fwTools::fwID::getObject(m_TMSUid));
            SLM_ASSERT("Sorry, '"<< m_TMSUid <<"' object is not a composite", pComposite);
        }

        for(::fwData::Reconstruction::sptr rec :  series->getReconstructionDB())
        {
            m_reconstructionMap[ rec->getOrganName() ] = rec;
        }

        for( ReconstructionMapType::iterator it = m_reconstructionMap.begin(); it != m_reconstructionMap.end(); ++it )
        {
            std::string organName = it->first;
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(organName), m_reconstructionListBox);
            if( pComposite && pComposite->find(organName) != pComposite->end())
            {
                item->setCheckState(Qt::Checked);
            }
            else
            {
                item->setCheckState(Qt::Unchecked);
            }
            m_reconstructionListBox->addItem (item);
        }
    }
}

//------------------------------------------------------------------------------

void SOrganTransformation::notitfyTransformationMatrix(::fwData::TransformationMatrix3D::sptr aTransMat)
{
    auto sig = aTransMat->signal< ::fwData::Object::ModifiedSignalType >(::fwData::Object::s_MODIFIED_SIG);
    sig->asyncEmit();
}

//------------------------------------------------------------------------------

void SOrganTransformation::onReconstructionCheck(QListWidgetItem *currentItem)
{
    ::fwData::Composite::sptr pComposite;
    if (::fwTools::fwID::exist(m_TMSUid))
    {
        pComposite = ::fwData::Composite::dynamicCast(::fwTools::fwID::getObject(m_TMSUid));
        SLM_ASSERT("Sorry, '"<< m_TMSUid <<"' object is not a composite", pComposite);

        ::std::string item_name                        = currentItem->text().toStdString();
        ::fwData::Reconstruction::sptr pReconstruction = m_reconstructionMap[item_name];
        ::fwData::Mesh::sptr pMesh                     = pReconstruction->getMesh();

        ::fwComEd::helper::Composite aCompositeHelper(pComposite);
        if ((currentItem->checkState()) == Qt::Checked)
        {
            if (pComposite->find(item_name) == pComposite->end())
            {
                aCompositeHelper.add(item_name, pMesh);
            }
            else
            {
                aCompositeHelper.swap(item_name, pMesh);
            }
        }
        else
        {
            if (pComposite->find(item_name) != pComposite->end())
            {
                aCompositeHelper.remove(item_name);
            }
        }
        aCompositeHelper.notify();
    }
}

//------------------------------------------------------------------------------

void SOrganTransformation::onResetClick()
{
    ::fwMedData::ModelSeries::sptr series = this->getObject< ::fwMedData::ModelSeries >();

    //search the corresponding triangular mesh
    for(::fwData::Reconstruction::sptr rec :  series->getReconstructionDB())
    {
        ::fwData::Mesh::sptr pTmpTrMesh = rec->getMesh();

        ::fwData::TransformationMatrix3D::sptr pTmpMat =
            pTmpTrMesh->getField< ::fwData::TransformationMatrix3D>( "TransformMatrix" );
        if (pTmpMat)
        {
            ::fwDataTools::TransformationMatrix3D::identity(pTmpMat);
            this->notitfyTransformationMatrix(pTmpMat);
        }
    }
}

//------------------------------------------------------------------------------

void SOrganTransformation::onSaveClick()
{
    InnerMatMappingType matMap;

    ::fwMedData::ModelSeries::sptr series = this->getObject< ::fwMedData::ModelSeries >();

    if(!series->getReconstructionDB().empty())
    {
        for(::fwData::Reconstruction::sptr rec :  series->getReconstructionDB())
        {
            ::fwData::Mesh::sptr pTmpTrMesh                = rec->getMesh();
            ::fwData::TransformationMatrix3D::sptr pTmpMat =
                pTmpTrMesh->getField< ::fwData::TransformationMatrix3D>( "TransformMatrix" );
            if (pTmpMat)
            {
                ::fwData::TransformationMatrix3D::sptr pCpyTmpMat;
                pCpyTmpMat                  = ::fwData::Object::copy(pTmpMat);
                matMap[pTmpTrMesh->getID()] = pCpyTmpMat;
            }
        }

        ::std::stringstream tmpSaveName;
        tmpSaveName << "save_" << m_saveCount;
        m_saveListing[tmpSaveName.str()] = matMap;
        m_saveSelectionComboBox->addItem(QString::fromStdString(tmpSaveName.str()));
        m_saveCount++;
    }
}

//------------------------------------------------------------------------------

void SOrganTransformation::onLoadClick()
{
    if (m_saveSelectionComboBox->count() != 0)
    {
        InnerMatMappingType matMap = m_saveListing[m_saveSelectionComboBox->currentText().toStdString()];

        ::fwMedData::ModelSeries::sptr series = this->getObject< ::fwMedData::ModelSeries >();

        //search the corresponding triangular mesh
        for(::fwData::Reconstruction::sptr rec :  series->getReconstructionDB())
        {
            ::fwData::Mesh::sptr pTmpTrMesh = rec->getMesh();
            if (matMap.find(pTmpTrMesh->getID()) != matMap.end())
            {
                ::fwData::TransformationMatrix3D::sptr pTmpMat =
                    pTmpTrMesh->getField< ::fwData::TransformationMatrix3D>( "TransformMatrix" );
                if (pTmpMat)
                {
                    pTmpMat->shallowCopy(matMap[pTmpTrMesh->getID()]);
                    this->notitfyTransformationMatrix(pTmpMat);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------

void SOrganTransformation::onSelectAllChanged(int state)
{

    if (::fwTools::fwID::exist(m_TMSUid))
    {
        ::fwData::Composite::sptr composite = ::fwData::Composite::dynamicCast(::fwTools::fwID::getObject(m_TMSUid));
        ::fwComEd::helper::Composite compositeHelper(composite);

        if(state == Qt::Checked)
        {
            m_reconstructionListBox->setEnabled(false);

            ::fwMedData::ModelSeries::sptr series = this->getObject< ::fwMedData::ModelSeries >();

            for(::fwData::Reconstruction::sptr rec :  series->getReconstructionDB())
            {
                if(composite->find(rec->getOrganName()) == composite->end())
                {
                    compositeHelper.add(rec->getOrganName(), rec->getMesh());
                }
            }

        }
        else if(state == Qt::Unchecked)
        {
            m_reconstructionListBox->setEnabled(true);

            QList<QListWidgetItem*> itemList = m_reconstructionListBox->findItems("", Qt::MatchContains);
            for(QListWidgetItem* item :  itemList)
            {
                if(item->checkState() == Qt::Unchecked)
                {
                    compositeHelper.remove(item->text().toStdString());
                }
            }

            this->refresh();
        }
        compositeHelper.notify();
    }
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsType SOrganTransformation::getObjSrvConnections() const
{
    KeyConnectionsType connections;
    connections.push_back( std::make_pair( ::fwMedData::ModelSeries::s_MODIFIED_SIG, s_UPDATE_SLOT ) );
    connections.push_back( std::make_pair( ::fwMedData::ModelSeries::s_RECONSTRUCTIONS_ADDED_SIG, s_UPDATE_SLOT ) );
    connections.push_back( std::make_pair( ::fwMedData::ModelSeries::s_RECONSTRUCTIONS_REMOVED_SIG, s_UPDATE_SLOT ) );

    return connections;
}

//------------------------------------------------------------------------------

} // namespace editor
} // namespace uiMedData


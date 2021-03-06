/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "opVTKMesh/action/SMeshCreation.hpp"


#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Signals.hpp>

#include <fwData/Image.hpp>
#include <fwData/Mesh.hpp>

#include <fwServices/macros.hpp>

#include <fwTools/fwID.hpp>

#include <fwVtkIO/helper/Mesh.hpp>
#include <fwVtkIO/vtk.hpp>

#include <vtkDiscreteMarchingCubes.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkThreshold.h>
#include <vtkPolyDataMapper.h>
#include <vtkDecimatePro.h>
#include <vtkGeometryFilter.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>


namespace opVTKMesh
{

namespace action
{

//-----------------------------------------------------------------------------

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::opVTKMesh::action::SMeshCreation, ::fwData::Object );

//-----------------------------------------------------------------------------

SMeshCreation::SMeshCreation() throw() :
    m_imageUID(""),
    m_meshUID(""),
    m_reduction(0)
{
}

//-----------------------------------------------------------------------------

SMeshCreation::~SMeshCreation() throw()
{
}

//-----------------------------------------------------------------------------

void SMeshCreation::starting() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    this->actionServiceStarting();
}

//-----------------------------------------------------------------------------

void SMeshCreation::stopping() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    this->actionServiceStopping();
}

//-----------------------------------------------------------------------------

void SMeshCreation::configuring() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();
    this->initialize();

    SLM_ASSERT( "Mesh UID andImage UID must be defined in the service configuration",  m_configuration->findConfigurationElement(
                    "image") && m_configuration->findConfigurationElement("mesh") );


    m_imageUID = m_configuration->findConfigurationElement("image")->getExistingAttributeValue("uid");

    m_meshUID = m_configuration->findConfigurationElement("mesh")->getExistingAttributeValue("uid");

    if (m_configuration->findConfigurationElement("percentReduction") &&
        m_configuration->findConfigurationElement("percentReduction")->hasAttribute("value"))
    {
        std::string reduce = m_configuration->findConfigurationElement("percentReduction")->getExistingAttributeValue(
            "value");
        m_reduction = boost::lexical_cast<unsigned int>(reduce);
    }

    OSLM_INFO( "Image UID = " << m_imageUID);
    OSLM_INFO( "Mesh UID = " << m_meshUID);
    OSLM_INFO( "Reduction value = " << m_reduction);
}

//-----------------------------------------------------------------------------

void SMeshCreation::updating() throw ( ::fwTools::Failed )
{
    SLM_TRACE_FUNC();

    /// Retreive object
    OSLM_ASSERT("Not found the image defined by uid : " << m_imageUID, ::fwTools::fwID::exist(m_imageUID));
    ::fwData::Image::sptr pImage = ::fwData::Image::dynamicCast( ::fwTools::fwID::getObject(m_imageUID) );
    OSLM_ASSERT("Not found the mesh defined by uid : " << m_meshUID, ::fwTools::fwID::exist(m_meshUID));
    ::fwData::Mesh::sptr pMesh = ::fwData::Mesh::dynamicCast( ::fwTools::fwID::getObject(m_meshUID) );

    ///VTK Mesher

    // vtk img
    vtkSmartPointer< vtkImageData > vtkImage = vtkSmartPointer< vtkImageData >::New();
    ::fwVtkIO::toVTKImage( pImage, vtkImage );

    // contour filter
    vtkSmartPointer< vtkDiscreteMarchingCubes > contourFilter = vtkSmartPointer< vtkDiscreteMarchingCubes >::New();
    contourFilter->SetInputData(vtkImage);
    contourFilter->SetValue(0, 255);
    contourFilter->ComputeScalarsOn();
    contourFilter->ComputeNormalsOn();
    contourFilter->Update();

    // smooth filter
    vtkSmartPointer< vtkWindowedSincPolyDataFilter > smoothFilter =
        vtkSmartPointer< vtkWindowedSincPolyDataFilter >::New();
    smoothFilter->SetInputConnection(contourFilter->GetOutputPort());
    smoothFilter->SetNumberOfIterations( 50 );
    smoothFilter->BoundarySmoothingOn();
    smoothFilter->SetPassBand ( 0.1 );
    smoothFilter->SetFeatureAngle(120.0);
    smoothFilter->SetEdgeAngle(90);
    smoothFilter->FeatureEdgeSmoothingOn();
    smoothFilter->Update();


    // Get polyData
    vtkSmartPointer< vtkPolyData > polyData;

    // decimate filter
    unsigned int reduction = m_reduction;
    if( reduction > 0 )
    {
        vtkSmartPointer< vtkDecimatePro > decimate = vtkSmartPointer< vtkDecimatePro >::New();
        decimate->SetInputConnection( smoothFilter->GetOutputPort() );
        decimate->SetTargetReduction( reduction/100.0 );
        decimate->PreserveTopologyOff();
        decimate->SplittingOn();
        decimate->BoundaryVertexDeletionOn();
        decimate->SetSplitAngle( 120 );
        decimate->Update();
        polyData = decimate->GetOutput();
        OSLM_TRACE("final GetNumberOfCells = " << polyData->GetNumberOfCells());
        ::fwVtkIO::helper::Mesh::fromVTKMesh( polyData, pMesh);
    }
    else
    {
        polyData = smoothFilter->GetOutput();
        OSLM_TRACE("final GetNumberOfCells = " << polyData->GetNumberOfCells());
        ::fwVtkIO::helper::Mesh::fromVTKMesh( polyData, pMesh);
    }

    /// Notification
    ::fwData::Object::ModifiedSignalType::sptr sig;
    sig = pMesh->signal< ::fwData::Object::ModifiedSignalType >(::fwData::Object::s_MODIFIED_SIG);
    {
        ::fwCom::Connection::Blocker block(sig->getConnection(m_slotUpdate));
        sig->asyncEmit();
    }
}

//-----------------------------------------------------------------------------

} // namespace action
} // namespace opVTKMesh


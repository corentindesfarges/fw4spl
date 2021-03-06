/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/NegatoSlicingInteractor.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Signals.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwComEd/Dictionary.hpp>
#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>

#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>
#include <fwData/String.hpp>
#include <fwData/TransferFunction.hpp>

#include <fwRenderVTK/vtk/fwVtkCellPicker.hpp>
#include <fwRenderVTK/vtk/Helpers.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <vtkActor.h>
#include <vtkActorCollection.h>
#include <vtkAssemblyNode.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkInteractorStyleImage.h>
#include <vtkProp3DCollection.h>
#include <vtkRenderWindowInteractor.h>



fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::NegatoSlicingInteractor,
                         ::fwData::Image );


namespace visuVTKAdaptor
{

#define START_SLICING_EVENT vtkCommand::MiddleButtonPressEvent
#define STOP_SLICING_EVENT  vtkCommand::MiddleButtonReleaseEvent

class NegatoSlicingCallback : public vtkCommand
{
public:
    static NegatoSlicingCallback *New()
    {
        return new NegatoSlicingCallback();
    }

    NegatoSlicingCallback() :
        m_picker(nullptr),
        m_localPicker(nullptr),
        m_pickedProp(nullptr),
        m_mouseMoveObserved(false)
    {
        this->PassiveObserverOff();
    }

    ~NegatoSlicingCallback()
    {
    }

    bool Pick(double pickPoint[3], double position[3])
    {
        SLM_ASSERT("m_picker should be set before picking.", m_picker);

        if ( m_picker->Pick( pickPoint, m_adaptor->getRenderer() ) )
        {
            m_picker->GetPickPosition(position);
            return true;
        }
        return false;
    }

    bool localPick(double pickPoint[3], double position[3])
    {
        SLM_ASSERT("m_localPicker should be set before picking.", m_localPicker);

        if ( m_localPicker->Pick( pickPoint, m_adaptor->getRenderer() ) )
        {
            m_localPicker->GetPickPosition(position);
            return true;
        }
        return false;
    }

    void startSlicing()
    {
        SLM_TRACE("vtkEvent: MiddleButtonPressEvent");
        SLM_ASSERT("m_adaptor not instanced", m_adaptor);
        SLM_ASSERT("m_picker not instanced", m_picker);

        double pickPoint[3];
        double pickedPoint[3];

        int x,y;
        m_adaptor->getInteractor()->GetEventPosition(x, y);
        pickPoint[0] = x;
        pickPoint[1] = y;
        pickPoint[2] = 0;

        OSLM_TRACE(
            "vtkEvent: MiddleButtonPressEvent: picking " << pickPoint[0] << ", " << pickPoint[1] << ", " <<
            pickPoint[2]);

        if ( this->Pick(pickPoint, pickedPoint) )
        {
            SLM_TRACE("vtkEvent: MiddleButtonPressEvent:picked point");
            SLM_ASSERT("Slicing has already begun", !m_mouseMoveObserved);
            m_adaptor->getInteractor()->AddObserver(vtkCommand::MouseMoveEvent, this, 1.);
            m_mouseMoveObserved = true;
            SetAbortFlag(1);

            //m_pickedProp = m_picker->GetProp3D();
            m_pickedProp  = ::fwRenderVTK::vtk::getNearestPickedProp(m_picker, m_adaptor->getRenderer());
            m_localPicker = fwVtkCellPicker::New();
            m_localPicker->InitializePickList();
            m_localPicker->PickFromListOn();
            m_localPicker->AddPickList(m_pickedProp);

            double localPickedPoint[3];
            this->localPick(pickPoint, localPickedPoint);

            m_adaptor->startSlicing(localPickedPoint);
        }
    }

    void stopSlicing()
    {
        SLM_TRACE("vtkEvent: MiddleButtonReleaseEvent");
        SLM_ASSERT("m_adaptor not instanced", m_adaptor);
        SLM_ASSERT("m_picker not instanced", m_picker);
        SLM_ASSERT("Slicing doesn't begun", m_mouseMoveObserved);

        m_adaptor->getInteractor()->RemoveObservers(vtkCommand::MouseMoveEvent, this);
        m_mouseMoveObserved = false;
        m_adaptor->stopSlicing();
        m_localPicker->Delete();
        m_localPicker = nullptr;
        m_pickedProp  = nullptr;
    }

    virtual void Execute( vtkObject *caller, unsigned long eventId, void *)
    {
        if(m_mouseMoveObserved && eventId == START_SLICING_EVENT)
        {
            SetAbortFlag(1); // To handle space bar pressed and middle click slicing at the same time
        }

        if(m_mouseMoveObserved || !m_adaptor->getInteractor()->GetShiftKey())
        {
            if (eventId == START_SLICING_EVENT && !m_mouseMoveObserved)
            {
                startSlicing();
            }

            else if(eventId == STOP_SLICING_EVENT && m_mouseMoveObserved)
            {
                stopSlicing();
            }
            else if (eventId == vtkCommand::MouseMoveEvent)
            {
                SLM_TRACE("vtkEvent: MouseMoveEvent");
                SLM_ASSERT("m_mouseMoveObserved not instanced", m_mouseMoveObserved);

                int x,y;
                double pickPoint[3];
                double pickedPoint[3];

                m_adaptor->getInteractor()->GetEventPosition(x,y);
                pickPoint[0] = x;
                pickPoint[1] = y;
                pickPoint[2] = 0;

                if ( this->localPick(pickPoint, pickedPoint))
                {
                    m_adaptor->updateSlicing(pickedPoint);
                }
            }
            else if (eventId == vtkCommand::KeyPressEvent && !m_adaptor->getInteractor()->GetControlKey())
            {
                vtkRenderWindowInteractor *rwi = vtkRenderWindowInteractor::SafeDownCast(caller);
                char *keySym                   = rwi->GetKeySym();

                if ( std::string(keySym) == "A" || std::string(keySym) == "a" )
                {
                    m_adaptor->pushSlice(-1, m_adaptor->getOrientation());
                }
                else if (std::string(keySym) == "Z" || std::string(keySym) == "z" )
                {
                    m_adaptor->pushSlice(1, m_adaptor->getOrientation());
                }
                if ( std::string(keySym) == "T" || std::string(keySym) == "t" )
                {
                    m_adaptor->pushSlice(-1, ::fwComEd::helper::MedicalImageAdaptor::Z_AXIS);
                }
                else if (std::string(keySym) == "Y" || std::string(keySym) == "y" )
                {
                    m_adaptor->pushSlice(1, ::fwComEd::helper::MedicalImageAdaptor::Z_AXIS);
                }
                else if (std::string(keySym) == "G" || std::string(keySym) == "g" )
                {
                    m_adaptor->pushSlice(-1, ::fwComEd::helper::MedicalImageAdaptor::Y_AXIS);
                }
                else if (std::string(keySym) == "H" || std::string(keySym) == "h" )
                {
                    m_adaptor->pushSlice(1, ::fwComEd::helper::MedicalImageAdaptor::Y_AXIS);
                }
                else if (std::string(keySym) == "B" || std::string(keySym) == "b" )
                {
                    m_adaptor->pushSlice(-1, ::fwComEd::helper::MedicalImageAdaptor::X_AXIS);
                }
                else if (std::string(keySym) == "N" || std::string(keySym) == "n" )
                {
                    m_adaptor->pushSlice(1, ::fwComEd::helper::MedicalImageAdaptor::X_AXIS);
                }
                else if (std::string(keySym) == "space" && !m_mouseMoveObserved)
                {
                    this->startSlicing();
                }
            }
            else if(eventId == vtkCommand::KeyReleaseEvent)
            {
                vtkRenderWindowInteractor *rwi = vtkRenderWindowInteractor::SafeDownCast(caller);
                char *keySym                   = rwi->GetKeySym();

                if (std::string(keySym) == "space" && m_mouseMoveObserved)
                {
                    this->stopSlicing();
                }
            }
        }
        else if (m_adaptor->getInteractor()->GetShiftKey())
        {
            if (eventId == MouseWheelForwardEvent)
            {
                m_adaptor->pushSlice(1, m_adaptor->getOrientation());
            }
            else if (eventId == vtkCommand::MouseWheelBackwardEvent)
            {
                m_adaptor->pushSlice(-1, m_adaptor->getOrientation());
            }
        }
    }

    void setAdaptor( NegatoSlicingInteractor::sptr adaptor)
    {
        m_adaptor = adaptor;
    }

    void setPicker( vtkAbstractPropPicker *picker)
    {
        m_picker = picker;
    }

protected:
    NegatoSlicingInteractor::sptr m_adaptor;
    vtkAbstractPropPicker *m_picker;
    vtkAbstractPropPicker *m_localPicker;
    vtkProp *m_pickedProp;
    bool m_mouseMoveObserved;
};

static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_INDEX_SLOT = "updateSliceIndex";
static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_TYPE_SLOT  = "updateSliceType";

const ::fwCom::Signals::SignalKeyType NegatoSlicingInteractor::s_SLICING_STARTED_SIG = "slicingStarted";
const ::fwCom::Signals::SignalKeyType NegatoSlicingInteractor::s_SLICING_STOPPED_SIG = "slicingStopped";

//-----------------------------------------------------------------------------

NegatoSlicingInteractor::NegatoSlicingInteractor() throw() : m_vtkObserver(nullptr), m_priority(.6)
{
    m_sigSlicingStarted = newSignal< SlicingStartedSignalType >(s_SLICING_STARTED_SIG);
    m_sigSlicingStopped = newSignal< SlicingStoppedSignalType >(s_SLICING_STOPPED_SIG);

    newSlot(s_UPDATE_SLICE_INDEX_SLOT, &NegatoSlicingInteractor::updateSliceIndex, this);
    newSlot(s_UPDATE_SLICE_TYPE_SLOT, &NegatoSlicingInteractor::updateSliceType, this);
}

//-----------------------------------------------------------------------------

NegatoSlicingInteractor::~NegatoSlicingInteractor() throw()
{
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    SLM_ASSERT("Tag config is required", m_configuration->getName() == "config");
    this->setRenderId( m_configuration->getAttributeValue("renderer") );
    this->setPickerId( m_configuration->getAttributeValue("picker") );
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::doStart() throw(fwTools::Failed)
{
    NegatoSlicingCallback *observer = NegatoSlicingCallback::New();
    observer->setAdaptor( NegatoSlicingInteractor::dynamicCast(this->getSptr()) );
    observer->setPicker(this->getPicker());

    m_vtkObserver = observer;

    this->getInteractor()->AddObserver(START_SLICING_EVENT, m_vtkObserver, m_priority);
    this->getInteractor()->AddObserver(STOP_SLICING_EVENT, m_vtkObserver, m_priority);
    this->getInteractor()->AddObserver(vtkCommand::KeyPressEvent, m_vtkObserver, m_priority);
    this->getInteractor()->AddObserver(vtkCommand::KeyReleaseEvent, m_vtkObserver, m_priority);
    this->getInteractor()->AddObserver(vtkCommand::MouseWheelForwardEvent, m_vtkObserver, m_priority);
    this->getInteractor()->AddObserver(vtkCommand::MouseWheelBackwardEvent, m_vtkObserver, m_priority);

    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    this->updateImageInfos(image);
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::doUpdate() throw(fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    this->updateImageInfos(image);
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::doSwap() throw(fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    this->updateImageInfos(image);
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::doStop() throw(fwTools::Failed)
{
    this->getInteractor()->RemoveObservers(START_SLICING_EVENT, m_vtkObserver);
    this->getInteractor()->RemoveObservers(STOP_SLICING_EVENT, m_vtkObserver);
    this->getInteractor()->RemoveObservers(vtkCommand::KeyPressEvent, m_vtkObserver);
    this->getInteractor()->RemoveObservers(vtkCommand::KeyReleaseEvent, m_vtkObserver);
    this->getInteractor()->RemoveObservers(vtkCommand::MouseWheelForwardEvent, m_vtkObserver);
    this->getInteractor()->RemoveObservers(vtkCommand::MouseWheelBackwardEvent, m_vtkObserver);
    m_vtkObserver->Delete();
    m_vtkObserver = nullptr;
    this->removeAllPropFromRenderer();
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::updateSliceIndex(int axial, int frontal, int sagittal)
{
    m_axialIndex->value()    = axial;
    m_frontalIndex->value()  = frontal;
    m_sagittalIndex->value() = sagittal;
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::updateSliceType(int from, int to)
{
    if( to == static_cast<int>(m_orientation) )
    {
        setOrientation( static_cast< Orientation >( from ));
    }
    else if(from == static_cast<int>(m_orientation))
    {
        setOrientation( static_cast< Orientation >( to ));
    }
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::startSlicing( double pickedPoint[3] )
{
    ::fwData::Integer::sptr sliceIndex[3];
    this->getSliceIndex(sliceIndex);

    int index[3];
    this->worldToImageSliceIndex(pickedPoint, index);
    m_sigSlicingStarted->asyncEmit();

    int i;
    for (i = 0; i<3; i++)
    {
        if (index[i] == sliceIndex[i]->value())
        {
            this->setOrientation((Orientation)i);
            break;
        }
    }
    SLM_INFO_IF( "unknown orientation", i == 3 );
    if(i != 3)
    {
        this->updateSlicing(pickedPoint);
    }
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::stopSlicing( )
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    m_sigSlicingStopped->asyncEmit();

    auto sig = image->signal< ::fwData::Image::SliceIndexModifiedSignalType >(
        ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG);
    {
        ::fwCom::Connection::Blocker block(sig->getConnection(this->slot(s_UPDATE_SLICE_INDEX_SLOT)));
        sig->asyncEmit(m_axialIndex->value(), m_frontalIndex->value(), m_sagittalIndex->value());
    }
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::updateSlicing( double pickedPoint[3] )
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

    ::fwData::Integer::sptr sliceIndex[3];
    this->getSliceIndex(sliceIndex);

    int index[3];
    int originalIndex = sliceIndex[m_orientation]->value();

    this->worldToImageSliceIndex(pickedPoint, index);
    index[m_orientation] = originalIndex;

    OSLM_TRACE("sliceIndex[0] "<< sliceIndex[0]->value()<< " index[0] " << index[0] << " pickedPt "<<pickedPoint[0]);
    OSLM_TRACE("sliceIndex[1] "<< sliceIndex[1]->value()<< " index[1] " << index[1] << " pickedPt "<<pickedPoint[1]);
    OSLM_TRACE("sliceIndex[2] "<< sliceIndex[2]->value()<< " index[2] " << index[2] << " pickedPt "<<pickedPoint[2]);

#ifdef DEBUG
    for ( int i = 0; i < image->getNumberOfDimensions(); i++ )
    {
        OSLM_ASSERT("index["<< i <<"] = " << index[i]
                            << " and image->getSize()[" << i << "] = " << image->getSize()[i],
                    index[i] >=0 && index[i] < image->getSize()[i]);
    }
#endif

    if(setSliceIndex(index))
    {
        auto sig = image->signal< ::fwData::Image::SliceIndexModifiedSignalType >(
            ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG);
        {
            ::fwCom::Connection::Blocker block(sig->getConnection(this->slot(s_UPDATE_SLICE_INDEX_SLOT)));
            sig->asyncEmit(m_axialIndex->value(), m_frontalIndex->value(), m_sagittalIndex->value());
        }
    }
}

//-----------------------------------------------------------------------------

void NegatoSlicingInteractor::pushSlice( int factor, Orientation axis)
{
    ::fwData::Integer::sptr sliceIndex[3];
    this->getSliceIndex(sliceIndex);

    int index[3];
    index[0]     = sliceIndex[0]->value();
    index[1]     = sliceIndex[1]->value();
    index[2]     = sliceIndex[2]->value();
    index[axis] += factor;

    int size[3];
    this->getImageDataSize(size);

    if (index[axis] < 0)
    {
        index[axis] = 0;
    }
    else if (index[axis] >= size[axis])
    {
        index[axis] = size[axis]-1;
    }

    if(setSliceIndex(index))
    {
        ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

        m_sigSlicingStopped->asyncEmit();

        auto sig = image->signal< ::fwData::Image::SliceIndexModifiedSignalType >(
            ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG);
        {
            ::fwCom::Connection::Blocker block(sig->getConnection(this->slot(s_UPDATE_SLICE_INDEX_SLOT)));
            sig->asyncEmit(m_axialIndex->value(), m_frontalIndex->value(), m_sagittalIndex->value());
        }
    }
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsType NegatoSlicingInteractor::getObjSrvConnections() const
{
    KeyConnectionsType connections;
    connections.push_back( std::make_pair( ::fwData::Image::s_MODIFIED_SIG, s_UPDATE_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG, s_UPDATE_SLICE_INDEX_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::Image::s_SLICE_TYPE_MODIFIED_SIG, s_UPDATE_SLICE_TYPE_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::Image::s_BUFFER_MODIFIED_SIG, s_UPDATE_SLOT ) );

    return connections;
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor

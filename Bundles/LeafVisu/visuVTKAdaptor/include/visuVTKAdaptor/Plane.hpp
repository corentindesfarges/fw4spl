/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_PLANE_HPP__
#define __VISUVTKADAPTOR_PLANE_HPP__

#ifndef ANDROID

#include "visuVTKAdaptor/config.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>

#include <fwData/Plane.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>


class vtkPlaneSource;
class vtkPlaneCollection;
class vtkPlane;
class vtkObject;
class vtkActor;

namespace fwData
{
class Plane;
}

namespace visuVTKAdaptor
{



class VISUVTKADAPTOR_CLASS_API Plane : public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (Plane)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API Plane() throw();

    VISUVTKADAPTOR_API virtual ~Plane() throw();

    VISUVTKADAPTOR_API void setVtkPlaneCollection( vtkObject * col );

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Plane::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     * Connect Plane::s_SELECTED_SIG to this::s_SELECT_PLANE_SLOT
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsType getObjSrvConnections() const;

    /**
     * @Signals
     * @{
     */
    /// Type of signal when plane interaction is started (store current plane)
    typedef ::fwCom::Signal< void (::fwData::Plane::sptr) > InteractionStartedSignalType;
    VISUVTKADAPTOR_API static const ::fwCom::Signals::SignalKeyType s_INTERACTION_STARTED_SIG;
    /**
     * @}
     */

protected:

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    /**
     * @name Slots
     * @{
     */
    /// Update the plane from the points and notify planes is modified
    void updatePoints();

    /// Re-notify "startInteraction" for this service
    void startInteraction();

    /// Changes points color (red is selected, else white)
    void selectPlane(bool select);
    /**
     * @}
     */

private:

    vtkPlaneSource* m_vtkPlane;
    vtkActor *m_actorPlan;
    ::fwData::Plane::wptr m_pPlane;

    vtkPlane* m_vtkImplicitPlane;
    vtkPlaneCollection* m_vtkPlaneCollection;

    /// register connections between plane's points and this service
    ::fwServices::helper::SigSlotConnection::sptr m_connections;
};


} //namespace visuVTKAdaptor

#endif // ANDROID

#endif // __VISUVTKADAPTOR_PLANE_HPP__

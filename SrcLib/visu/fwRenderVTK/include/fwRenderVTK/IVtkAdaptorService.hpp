/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWRENDERVTK_IVTKADAPTORSERVICE_HPP__
#define __FWRENDERVTK_IVTKADAPTORSERVICE_HPP__

#include <string>


#include <fwServices/IService.hpp>
#include <fwServices/helper/SigSlotConnection.hpp>

#include "fwRenderVTK/VtkRenderService.hpp"
#include "fwRenderVTK/config.hpp"

class vtkProp;
class vtkPropCollection;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkTransform;

namespace fwRenderVTK
{

class FWRENDERVTK_CLASS_API IVtkAdaptorService : public fwServices::IService
{
friend class VtkRenderService;
public:
    fwCoreServiceClassDefinitionsMacro ( (IVtkAdaptorService)(::fwServices::IService) );

    typedef fwServices::IService SuperClass;


    /// To set a representation
    virtual void show(bool b = true)
    {
    }
    virtual void hide()
    {
        this->show(false);
    }


    FWRENDERVTK_API void setRenderService( VtkRenderService::sptr service );
    FWRENDERVTK_API void setRenderId(VtkRenderService::RendererIdType newID);
    FWRENDERVTK_API VtkRenderService::sptr getRenderService();
    FWRENDERVTK_API VtkRenderService::RendererIdType getRenderId();
    FWRENDERVTK_API vtkRenderer* getRenderer();


    FWRENDERVTK_API void setPickerId(VtkRenderService::PickerIdType newID);
    FWRENDERVTK_API VtkRenderService::PickerIdType getPickerId();
    FWRENDERVTK_API vtkAbstractPropPicker* getPicker(std::string pickerId = "");

    FWRENDERVTK_API void setTransformId(VtkRenderService::VtkObjectIdType newID);
    FWRENDERVTK_API VtkRenderService::VtkObjectIdType getTransformId();
    FWRENDERVTK_API vtkTransform* getTransform();

    FWRENDERVTK_API vtkObject * getVtkObject(const VtkRenderService::VtkObjectIdType& objectId) const;

    FWRENDERVTK_API vtkRenderWindowInteractor* getInteractor();


    FWRENDERVTK_API virtual ::fwData::Object::sptr getAssociatedObject(vtkProp *prop, int depth = 0);

    FWRENDERVTK_API void getAllSubProps(vtkPropCollection *propc, int depth = -1);

    /// set the pipeline modified so a render request can be send. end-user have to call this
    /// method when it have modified a  vtk structure
    FWRENDERVTK_API void setVtkPipelineModified();

    /// Returns true if the service automatically triggers the rendering.
    bool getAutoRender() const
    {
        return m_autoRender;
    }

    /// Sets if the service automatically triggers the rendering.
    void setAutoRender(bool autoRender)
    {
        m_autoRender = autoRender;
    }

protected:

    /**
     * @brief   constructor
     */
    FWRENDERVTK_API IVtkAdaptorService() throw();

    /**
     * @brief   destructor
     */
    FWRENDERVTK_API virtual ~IVtkAdaptorService() throw();

    /**
     * @name    Standard service methods
     */
    //@{
    /// Overrides
    FWRENDERVTK_API virtual void info(std::ostream &_sstream );
    FWRENDERVTK_API void starting() throw(fwTools::Failed);
    FWRENDERVTK_API void stopping() throw(fwTools::Failed);
    FWRENDERVTK_API void swapping() throw(fwTools::Failed);
    FWRENDERVTK_API void updating() throw(fwTools::Failed);
    //@}


    /// priority of comChannel observing related object (specified with objectId)
    double m_comChannelPriority;

    /// state of the pipeline
    bool m_vtkPipelineModified;
    VtkRenderService::RendererIdType m_rendererId;
    VtkRenderService::PickerIdType m_pickerId;
    VtkRenderService::VtkObjectIdType m_transformId;
    VtkRenderService::wptr m_renderService;

    ::fwServices::helper::SigSlotConnection::sptr m_connections;


    typedef std::vector < ::fwRenderVTK::IVtkAdaptorService::wptr > ServiceVector;
    ServiceVector m_subServices;

    vtkPropCollection * m_propCollection;

    bool m_autoRender;

    FWRENDERVTK_API virtual void doStart()  = 0;
    FWRENDERVTK_API virtual void doStop()   = 0;
    FWRENDERVTK_API virtual void doSwap()   = 0;
    FWRENDERVTK_API virtual void doUpdate() = 0;

    ServiceVector & getRegisteredServices()
    {
        return m_subServices;
    }
    FWRENDERVTK_API void registerService( ::fwRenderVTK::IVtkAdaptorService::sptr service );
    FWRENDERVTK_API void unregisterServices();

    FWRENDERVTK_API void registerProp(vtkProp *prop);
    FWRENDERVTK_API void unregisterProps();

    FWRENDERVTK_API void addToRenderer(vtkProp *prop);
    FWRENDERVTK_API void addToPicker(vtkProp *prop, std::string pickerId = "");
    FWRENDERVTK_API void removeFromPicker(vtkProp *prop, std::string pickerId = "");

    FWRENDERVTK_API void removeAllPropFromRenderer();

    FWRENDERVTK_API static void getProps(vtkPropCollection *propc, vtkProp *prop);

    /// notify a render request iff vtkPipeline is modified
    FWRENDERVTK_API void requestRender();
};

}

#endif /*__FWRENDERVTK_IVTKADAPTORSERVICE_HPP__*/

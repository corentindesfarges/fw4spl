/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_POINTLABEL_HPP__
#define __VISUVTKADAPTOR_POINTLABEL_HPP__

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/Text.hpp"

#include <string>

#include <fwRenderVTK/IVtkAdaptorService.hpp>


class VISUVTKADAPTOR_CLASS_API vtkTextActor;

namespace visuVTKAdaptor
{

class VISUVTKADAPTOR_CLASS_API PointLabel : public Text
{

public:
    fwCoreServiceClassDefinitionsMacro ( (PointLabel)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API PointLabel();

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Point::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsType getObjSrvConnections() const;

protected:

    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);

    void starting() throw(::fwTools::Failed);
    void stopping() throw(::fwTools::Failed);
};

} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_POINTLABEL_HPP__


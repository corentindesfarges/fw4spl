#include "fwDataCamp/Factory.hpp"
#include "fwDataCamp/Video.hpp"

fwCampImplementDataMacro((fwData)(Video))
{
    builder
        .tag("object_version", "1")
        .tag("lib_name", "fwData")
        .base< ::fwData::Object >()
        .property("data_available",&fwData::Video::m_dataAvailable)
        .property("last_modified",&fwData::Video::m_lastModified)
        .property("camera",&fwData::Video::m_camera)
        ;
}
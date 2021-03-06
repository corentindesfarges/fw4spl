/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWMEDDATA_SERIES_HPP__
#define __FWMEDDATA_SERIES_HPP__

#include "fwMedData/config.hpp"
#include "fwMedData/types.hpp"

#include <fwData/factory/new.hpp>
#include <fwData/Object.hpp>


fwCampAutoDeclareDataMacro((fwMedData)(Series), FWMEDDATA_API);

namespace fwMedData
{

class Patient;
class Study;
class Equipment;

/**
 * @brief Holds series information.
 */
class FWMEDDATA_CLASS_API Series : public ::fwData::Object
{

public:
    fwCoreNonInstanciableClassDefinitionsMacro( (Series)(::fwData::Object) );

    fwCampMakeFriendDataMacro((fwMedData)(Series));

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWMEDDATA_API Series(::fwData::Object::Key key);

    /// Destructor
    FWMEDDATA_API virtual ~Series();

    /// Defines shallow copy
    FWMEDDATA_API void shallowCopy( const ::fwData::Object::csptr &_source );

    /// Defines deep copy
    FWMEDDATA_API void cachedDeepCopy( const ::fwData::Object::csptr &_source, DeepCopyCacheType &cache );

    /**
     * @name Getters / Setters
     * @{ */

    /**
     * @brief Referring Patient
     * @{ */
    SPTR(::fwMedData::Patient) getPatient() const;
    void setPatient(const SPTR(::fwMedData::Patient)& val);
    /**  @} */

    /**
     * @brief Referring Study
     * @{ */
    SPTR(::fwMedData::Study) getStudy () const;
    void setStudy(const SPTR(::fwMedData::Study)& val);
    /**  @} */

    /**
     * @brief Related Equipment
     * @{ */
    SPTR(::fwMedData::Equipment) getEquipment () const;
    void setEquipment(const SPTR(::fwMedData::Equipment)& val);
    /**  @} */

    /**
     * @brief Unique identifier of the Series (0020,000E)
     * @{ */
    const DicomValueType &getInstanceUID () const;
    void setInstanceUID (const DicomValueType &val);
    /**  @} */

    /**
     * @brief Type of equipment that originally acquired the data used to create this Series (0008,0060)
     * @{ */
    const DicomValueType &getModality () const;
    void setModality (const DicomValueType &val);
    /**  @} */

    /**
     * @brief Date the Series started (0008,0021)
     * @{ */
    const DicomValueType &getDate () const;
    void setDate (const DicomValueType &val);
    /**  @} */

    /**
     * @brief Time the Series started (0008,0031)
     * @{ */
    const DicomValueType &getTime () const;
    void setTime (const DicomValueType &val);
    /**  @} */

    /**
     * @brief Description of the Series (0008,103E)
     * @{ */
    const DicomValueType &getDescription () const;
    void setDescription (const DicomValueType &val);
    /**  @} */

    /**
     * @brief Name of the physician(s) administering the Series (0008,1050)
     * @{ */
    const DicomValuesType &getPerformingPhysiciansName () const;
    void setPerformingPhysiciansName (const DicomValuesType &val);
    /**  @} */

    /**  @} */

protected:

    /// Referring Patient
    SPTR(Patient) m_patient;

    /// Referring Study
    SPTR(Study) m_study;

    /// Related Equipment
    SPTR(Equipment) m_equipment;

    /// Series unique identifier
    DicomValueType m_instanceUID;

    /// Modality
    DicomValueType m_modality;

    /// Date
    DicomValueType m_date;

    /// Time
    DicomValueType m_time;

    /// Description
    DicomValueType m_description;

    /// Performing physicians name
    DicomValuesType m_performingPhysiciansName;
};

//-----------------------------------------------------------------------------

inline SPTR(::fwMedData::Patient) Series::getPatient() const
{
    return m_patient;
}

//-----------------------------------------------------------------------------

inline void Series::setPatient(const SPTR(::fwMedData::Patient)& val)
{
    m_patient = val;
}

//-----------------------------------------------------------------------------

inline SPTR(::fwMedData::Study) Series::getStudy () const
{
    return m_study;
}

//-----------------------------------------------------------------------------

inline void Series::setStudy(const SPTR(::fwMedData::Study)& val)
{
    m_study = val;
}

//-----------------------------------------------------------------------------

inline SPTR(::fwMedData::Equipment) Series::getEquipment () const
{
    return m_equipment;
}

//-----------------------------------------------------------------------------

inline void Series::setEquipment(const SPTR(::fwMedData::Equipment)& val)
{
    m_equipment = val;
}

//-----------------------------------------------------------------------------

inline const DicomValueType &Series::getInstanceUID () const
{
    return m_instanceUID;
}

//-----------------------------------------------------------------------------

inline void Series::setInstanceUID (const DicomValueType &val)
{
    m_instanceUID = val;
}

//-----------------------------------------------------------------------------

inline const DicomValueType &Series::getModality () const
{
    return m_modality;
}

//-----------------------------------------------------------------------------

inline void Series::setModality (const DicomValueType &val)
{
    m_modality = val;
}

//-----------------------------------------------------------------------------

inline const DicomValueType &Series::getDate () const
{
    return m_date;
}

//-----------------------------------------------------------------------------

inline void Series::setDate (const DicomValueType &val)
{
    m_date = val;
}

//-----------------------------------------------------------------------------

inline const DicomValueType &Series::getTime () const
{
    return m_time;
}

//-----------------------------------------------------------------------------

inline void Series::setTime (const DicomValueType &val)
{
    m_time = val;
}

//-----------------------------------------------------------------------------

inline const DicomValueType &Series::getDescription () const
{
    return m_description;
}

//-----------------------------------------------------------------------------

inline void Series::setDescription (const DicomValueType &val)
{
    m_description = val;
}

//-----------------------------------------------------------------------------

inline const DicomValuesType &Series::getPerformingPhysiciansName () const
{
    return m_performingPhysiciansName;
}

//-----------------------------------------------------------------------------

inline void Series::setPerformingPhysiciansName (const DicomValuesType &val)
{
    m_performingPhysiciansName = val;
}

//-----------------------------------------------------------------------------

}   //end namespace fwMedData

#endif // __FWMEDDATA_SERIES_HPP__



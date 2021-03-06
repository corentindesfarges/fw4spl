/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_STRING_HPP__
#define __FWDATA_STRING_HPP__


#include "fwData/factory/new.hpp"
#include "fwData/GenericField.hpp"
#include "fwData/Object.hpp"

fwCampAutoDeclareDataMacro((fwData)(String), FWDATA_API);

namespace fwData
{

/**
 * @brief This class contains an std::string value.
 *
 * String object is essentially used as a field in other objects.
 */
class FWDATA_CLASS_API String : public GenericField< std::string >
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (String)(::fwData::Object), ( ((const std::string)("")) ),
                                            GenericFieldFactory< String >);
    fwCampMakeFriendDataMacro((fwData)(String));

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWDATA_API String(::fwData::Object::Key key) throw();

    /**
     * @brief Destructor.
     */
    FWDATA_API virtual ~String() throw();

    /// Defines shallow copy
    FWDATA_API void shallowCopy( const Object::csptr& _source );

    /// Defines deep copy
    FWDATA_API void cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType &cache);

};

} // namespace fwData

#endif /* __FWDATA_STRING_HPP__ */


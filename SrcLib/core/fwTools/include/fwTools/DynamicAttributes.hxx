/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWTOOLS_DYNAMICATTRIBUTES_HXX__
#define __FWTOOLS_DYNAMICATTRIBUTES_HXX__

#include "fwTools/macros.hpp"

#include <fwCore/base.hpp>
#include <fwCore/Demangler.hpp>

#include <boost/ref.hpp>
#include <map>
#include <vector>

namespace fwTools
{

class DynamicAttributesBase
{
public:

    virtual fwToolsRegisterAttributeSignatureMacro() = 0;
};

template<class CLASS>
class DynamicAttributes : public DynamicAttributesBase
{

public:
    typedef std::string AttrNameType;
    typedef SPTR (CLASS)  AttrType;
    typedef std::vector< AttrNameType > AttrNameVectorType;
    typedef ::boost::reference_wrapper< AttrType > AttrRefType;
    typedef std::map< AttrNameType, AttrRefType > AttrMapType;

    typedef CSPTR (CLASS) ConstAttrType;

    DynamicAttributes();
    virtual ~DynamicAttributes();

    virtual ConstAttrType getConstAttribute( AttrNameType attrName ) const;
    virtual AttrType getAttribute( AttrNameType attrName );
    bool        hasAttribute( AttrNameType attrName );

    virtual fwToolsSetAttributeSignatureMacro()
    {
        FwCoreNotUsedMacro(attrName);
        FwCoreNotUsedMacro(_obj);
    };

    virtual AttrNameVectorType getAttributeNames();

    virtual fwToolsRegisterAttributeSignatureMacro() {
    };
protected:
    virtual AttrRefType getAttributeRef( AttrNameType attrName );
    AttrMapType __FWTOOLS_ATTRIBUTE_MAP_NAME;


};

//------------------------------------------------------------------------------

template< class CLASS >
DynamicAttributes< CLASS >::DynamicAttributes()
{
}

//------------------------------------------------------------------------------

template< class CLASS >
DynamicAttributes< CLASS >::~DynamicAttributes()
{
}

//------------------------------------------------------------------------------

template< class CLASS >
typename DynamicAttributes< CLASS >::AttrType DynamicAttributes< CLASS >::getAttribute( AttrNameType attrName )
{
    return this->getAttributeRef(attrName);
}

//------------------------------------------------------------------------------

template< class CLASS >
typename DynamicAttributes< CLASS >::AttrRefType DynamicAttributes< CLASS >::getAttributeRef( AttrNameType attrName )
{
    typename DynamicAttributes::AttrMapType::iterator iter = this->__FWTOOLS_ATTRIBUTE_MAP_NAME.find(attrName);
    OSLM_FATAL_IF(
        "Object "<< ::fwCore::getFullClassname< CLASS >() << "has no attribute named '"<< attrName << "'",
        iter == this->__FWTOOLS_ATTRIBUTE_MAP_NAME.end()
        );
    return (*iter).second;
}

//------------------------------------------------------------------------------

template< class CLASS >
typename DynamicAttributes< CLASS >::ConstAttrType DynamicAttributes< CLASS >::getConstAttribute( AttrNameType attrName )
const
{
    typename DynamicAttributes::AttrMapType::const_iterator iter = this->__FWTOOLS_ATTRIBUTE_MAP_NAME.find(attrName);
    OSLM_FATAL_IF(
        "Object "<< ::fwCore::getFullClassname< CLASS >() << "has no attribute named '"<< attrName << "'",
        iter == this->__FWTOOLS_ATTRIBUTE_MAP_NAME.end()
        );
    return (*iter).second.get();
}

//------------------------------------------------------------------------------

template< class CLASS >
bool DynamicAttributes< CLASS >::hasAttribute( AttrNameType attrName )
{
    typename DynamicAttributes::AttrMapType::iterator iter = this->__FWTOOLS_ATTRIBUTE_MAP_NAME.find(attrName);
    return iter != this->__FWTOOLS_ATTRIBUTE_MAP_NAME.end();
}

//------------------------------------------------------------------------------

template< class CLASS >
typename DynamicAttributes< CLASS >::AttrNameVectorType DynamicAttributes< CLASS >::getAttributeNames()
{
    AttrNameVectorType names;
    typename DynamicAttributes::AttrMapType::const_iterator iter;
    for ( iter = this->__FWTOOLS_ATTRIBUTE_MAP_NAME.begin(); iter != this->__FWTOOLS_ATTRIBUTE_MAP_NAME.end(); ++iter )
    {
        names.push_back((*iter).first);
    }
    return names;
}

} //end namespace fwTools

#endif //__FWTOOLS_DYNAMICATTRIBUTES_HXX__

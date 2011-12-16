/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_TU_STRUCTURETRAITSDICTIONARYTEST_HPP__
#define __FWDATA_TU_STRUCTURETRAITSDICTIONARYTEST_HPP__

#include <cppunit/extensions/HelperMacros.h>

class StructureTraitsDictionaryTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( StructureTraitsDictionaryTest );
    CPPUNIT_TEST( testAddingStructure );
    CPPUNIT_TEST_SUITE_END();

public:
    // interface
    void setUp();
    void tearDown();


    void testAddingStructure();
};

#endif // __FWDATA_TU_STRUCTURETRAITSDICTIONARYTEST_HPP__
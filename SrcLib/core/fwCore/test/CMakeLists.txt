fwLoadProperties()
fwUseForwardInclude(
    fwCore
    fwTest
)

find_package(CppUnit)
find_package(Boost COMPONENTS chrono regex REQUIRED)
    
fwInclude(
    ${CPPUNIT_INCLUDE_DIR}
)
fwLink(
    ${CPPUNIT_LIBRARY}
    ${Boost_CHRONO_LIBRARY}
    ${Boost_REGEX_LIBRARY}
)






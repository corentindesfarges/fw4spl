fwLoadProperties()
fwUseForwardInclude(
    fwCom
    fwGui
    fwMemory
    fwRuntime
    fwTools

    gui
)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package (LibXml2)

fwForwardInclude(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${LIBXML2_INCLUDE_DIR}
)
fwLink(
    ${Qt5Core_LIBRARIES} 
    ${Qt5Gui_LIBRARIES} 
    ${Qt5Widgets_LIBRARIES}
    ${LIBXML2_LIBRARIES} 
)

set_target_properties(${FWPROJECT_NAME} PROPERTIES AUTOMOC TRUE)



fwLoadProperties()

fwUseForwardInclude(
    fwData
    fwGui
    fwMedData
    fwRuntime
    fwServices
    fwTools

    gui
)

fwReq(
    visuVTKAdaptor
    uiImageQt
    ctrlSelection
    )

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Boost COMPONENTS regex REQUIRED)

fwForwardInclude(
    ${Boost_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

fwLink(
    ${Boost_REGEX_LIBRARY}
    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
)

set_target_properties(${FWPROJECT_NAME} PROPERTIES AUTOMOC TRUE)



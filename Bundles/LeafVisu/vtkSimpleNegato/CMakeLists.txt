fwLoadProperties()
fwUseForwardInclude(
    fwRender
    fwRenderVTK
    fwRuntime
)

find_package(VTK REQUIRED)

fwForwardInclude(
    ${VTK_INCLUDE_DIRS} 
)
fwLink(
    vtkInteractionWidgets
)


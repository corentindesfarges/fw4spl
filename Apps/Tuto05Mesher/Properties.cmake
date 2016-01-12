
set( NAME Tuto05Mesher )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES  )
set( REQUIREMENTS
    dataReg
    servicesReg
    gui
    guiQt
    io
    ioVTK
    visuVTK
    visuVTKQt
    uiIO
    vtkSimpleNegato
    vtkSimpleMesh
    opVTKMesh # provides services to generate a mesh from an image.
    fwLauncher
    appXml
)

bundleParam(appXml PARAM_LIST config PARAM_VALUES MesherConfig)


set( NAME Tuto14MeshGenerator )
set( VERSION 0.1 )
set( TYPE APP )
set( UNIQUE TRUE )
set( DEPENDENCIES  )
set( REQUIREMENTS
    dataReg
    ctrlSelection
    gui
    guiQt
    ioData
    ioVTK
    io
    servicesReg
    uiVisuQt
    uiIO
    visuVTK
    visuVTKQt
    visuVTKAdaptor
    fwLauncher
    appXml
)

bundleParam(appXml PARAM_LIST config PARAM_VALUES Tuto14MeshGenerator)


set( NAME Tuto03DataService )
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
    uiIO # contains services to show dialogs for reader/writer selection
    visuVTK
    visuVTKQt
    vtkSimpleNegato
    fwLauncher
    appXml
)

bundleParam(appXml PARAM_LIST config PARAM_VALUES tutoDataServiceConfig)

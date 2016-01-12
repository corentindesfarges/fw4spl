
set( NAME TutoGui )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES  )
set( REQUIREMENTS 
    io
    appXml
    gui
    guiQt
    dataReg
    servicesReg
    fwLauncher
)

bundleParam(appXml PARAM_LIST config PARAM_VALUES tutoGuiConfig)

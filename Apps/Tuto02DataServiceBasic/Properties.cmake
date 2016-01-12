
set( NAME Tuto02DataServiceBasic )
set( VERSION 0.1 )
set( TYPE APP )
set( DEPENDENCIES  )
set( REQUIREMENTS
    dataReg
    servicesReg
    gui
    guiQt
    io # contains the interface for reader and writer.
    ioVTK # contains the reader and writer for VTK files (image and mesh).
    visuVTK # loads VTK rendering library (fwRenderVTK).
    visuVTKQt # containsthe vtk Renderer window interactor manager using Qt.
    vtkSimpleNegato # contains a visualization service of medical image.
    fwLauncher
    appXml
)

bundleParam(appXml PARAM_LIST config PARAM_VALUES tutoDataServiceBasicConfig)

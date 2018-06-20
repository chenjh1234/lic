include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = reportlic

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_UTIL}

LIBS +=  $${LIBS_BUS}
LIBS += $${LIBS_APP}    
LIBS += -llicclient
#LIBS += -llicclient  
SOURCES += \ 
	   ../report/report.cpp 
#SOURCES +=  $${SOURCE_APP}
 

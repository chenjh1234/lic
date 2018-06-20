include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = borrow

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_UTIL}

LIBS +=  $${LIBS_BUS}
LIBS += $${LIBS_APP}

 
 

SOURCES += \
	  borrow.cpp 
SOURCES +=  $${SOURCE_APP}



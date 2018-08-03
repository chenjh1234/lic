
include (../licInclude.config)
 
INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_UTIL}
INCLUDEPATH += $${INCLUDE_SERVER}

 
TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = testagentapi

LIBS += \
	-L $${LIC_ROOT_PATH}/lib

LIBS += $${LIBS_BUS} $${LIBS_APP} -llicserver -llicconfig
	
 SOURCES +=  \
	   tagent.cpp
	    
 


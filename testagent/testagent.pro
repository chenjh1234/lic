
include (../licInclude.config)
 
INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_UTIL}

 
TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = testagent

LIBS += \
	-L $${LIC_ROOT_PATH}/lib

LIBS += $${LIBS_BUS} $${LIBS_APP} -llicserver -llicconfig
	
 SOURCES +=  \
	   testagent.cpp
	    
 


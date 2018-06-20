
include (../licInclude.config)
TEMPLATE = lib
  
DESTDIR =$${LIC_LIB}
TARGET = licserverdev

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_UTIL}


#LIBS +=  $${LIBS_BUS}
#LIBS +=  $${LIBS_APP}  
 
HEADERS +=   \
	 licServer.hpp
 
SOURCES +=  \
	    licServer.cpp 	 
	    

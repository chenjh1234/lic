
include (../licInclude.config)
TEMPLATE = lib
  
DESTDIR =$${LIC_LIB}
TARGET = licserverdev

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_SERVER}

 
#HEADERS +=   \
#	 licServer.hpp
 
SOURCES +=  \
	SCycleThread.cpp\
	    licAgent.cpp\
	    licServer.cpp 	 
	    

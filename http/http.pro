
include (../licInclude.config)
TEMPLATE = lib
 
INCLUDEPATH +=  $${INCLUDE_BUS}
	 
  
DESTDIR =$${LIC_LIB}

TARGET = lichttpcall
LIBS += \
	-L $${LIC_ROOT_PATH}/lib
	

 HEADERS +=  
	 
 

 SOURCES +=  \
	   http_call.cpp
 

message ("msg1 = " $${LIC_ROOT_PATH})# for inner varible
message ("msg_env = " $(LIC_ROOT_PATH)) # for env varible
message ("msg_home = " $(HOME)) # for env varible
include (../licInclude.config)
message ("msg 2 = " $${LIC_ROOT_PATH})

TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = testlic

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH +=$${INCLUDE_UTIL}
INCLUDEPATH +=$${INCLUDE_HTTP_CALL}  
INCLUDEPATH +=$${INCLUDE_SERVER}  

LIBS +=  $${LIBS_BUS}
LIBS += $${LIBS_APP}
	 
 
HEADERS +=  
 
SOURCES +=  \
	    lic.cpp 	
SOURCES +=  $${SOURCE_APP}

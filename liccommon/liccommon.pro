
include (../licInclude.config)
TEMPLATE = lib
INCLUDEPATH +=  $${INCLUDE_BUS}

INCLUDEPATH +=  ./\
	../licbase\
        ../licconfig\
	../licserver
INCLUDEPATH += $${HTTP_CALL}
 
DESTDIR =$${LIC_LIB}
TARGET = liccommon
 
HEADERS +=  

SOURCES += \
	   $${TETRIS_HOME}/src/lib/distributed_bus.cpp \
	   $${TETRIS_HOME}/src/lib/bus.cpp \   
	   jsonParser.cpp
 
 


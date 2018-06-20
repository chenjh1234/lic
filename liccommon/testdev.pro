
include (../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = reportlic
INCLUDE_UTILL= \
	 ./\
	../licbase\
	../client\
	../licserver
INCLUDE_UTILL +=$${INCLUDE_HTTP_CALL}
INCLUDE_UTILL +=$${INCLUDE_BUS}

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_UTILL}

LIBS_APPP = \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase  -llicclient -llichttpcall -lcurl   /lib64/libdl.so.2

LIBS +=  $${LIBS_BUS}
LIBS += $${LIBS_APPP}

SOURCES += \
	../util/report/report.cpp\
	   $${TETRIS_HOME}/src/lib/distributed_bus.cpp \
	   $${TETRIS_HOME}/src/lib/bus.cpp \   
	   jsonParser.cpp
 
 



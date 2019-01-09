include (../licInclude.config)
TEMPLATE = lib
 INCLUDEPATH +=  ./\
	../client

INCLUDEPATH += $${HTTP_CALL} 
DESTDIR =$${LIC_LIB}

TARGET = east

 
HEADERS +=  

SOURCES += east.cpp

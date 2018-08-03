
include (../licInclude.config)
TEMPLATE = lib
 
INCLUDEPATH +=  \
	../licconfig \
	../licbase 

  
DESTDIR =$${LIC_LIB}
TARGET = licserver
LIBS += \
	-L $${LIC_ROOT_PATH}/lib
	

 HEADERSSS +=  \
	   SLicData.h \
	   SLicMng.h \
	   SAppMng.h \
	   SAppInfo.h \
	   SPackMng.h \
	   SPackInfo.h \
#	   SCycleThread.h \
	   SLog.h 
 

 SOURCES +=  \
	   SLicMng.cpp \
	   SAppInfo.cpp \
	   SAppMng.cpp \
	   SPackInfo.cpp \
	   SPackMng.cpp \
	   SLicData.cpp \
#	   SCycleThread.cpp \
	   DBstat.cpp\
	   SAgent.cpp\
	   SLog.cpp 
 
 


# below is for install:  / is important we must install from /
APPLIC.path +=/$${LIC_ROOT_PATH}/file
APPLIC.files = ../apps/apps.cpp ../apps/apps.pro ../licwork/licfile.sample ../apps/licdev.pdf ../apps/licgen.pdf ../apps/stjob.pdf \
	../licwork/* ../testconfig/lic.conf  
INSTALLS +=APPLIC

SH.path +=/$${LIC_ROOT_PATH}/bin
SH.files = ../sh/* 
INSTALLS +=SH
#message ("apppath= " $$APPLIC.path)



include (../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = addr
 
HEADERS += 
HOME = "home1"

message("test home  LINUX_HOME = $(HOME),\$\$HOME=$${HOME},\$\${HOME}=$$HOME")
UNAME = $(USERNAME)
USER_NAME =$$UNAME

defined(HOME) :message("yes HOME is defined = $$(HOME)")


#touch("/dev","/dev") :message("yes /123ok")

message(USER_NAME = $$USER_NAME)
equals(USER_NAME,"cjh") { message("yes cjh")}
equals(USER_NAME,$(USERNAME)) { message("yesyes cjh")}
  

SOURCES +=  \
	addr.cpp
	    

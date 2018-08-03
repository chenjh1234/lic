TEMPLATE = subdirs
include (../licInclude.config)

SUBDIRS =  appt borrow borrowreturn createkey createlic loadfile mid passwd2m passwdtoday report\
	  showdb showproof unloadfile unpack uuid licgen stjob shutdownlic loadconfig viewconfig clearconfig viewagent
CONFIG += ordered




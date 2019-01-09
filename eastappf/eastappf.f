      function str2c( pstr) 
      character *(*) pstr 
      character (len = 2048) ::  str2c
      str2c = ""
      str2c =trim(pstr)//"\0"
      return
      end function str2c

      PROGRAM eastappf
      INTEGER :: i,n,number,delay
      CHARACTER(len=128) :: arg,pack,lgout,app
c   functions:
      character (len = 2048) ::  str2c
      integer:: east_login,east_logoutf
      logical(1):: b ,check_lic

      number = 1
      delay = 100
 
 
c"p1:package: package name\n"\
c"p2:number: number license apply(1)\n"\
c"p3:delay:time in second the app spend(100)\n"\
c"p4:if present: we do logout\n";
 
      i = 0
      DO
         CALL get_command_argument(i, arg)
         IF (LEN_TRIM(arg) == 0) EXIT
         WRITE (*,*) i,arg
         if(i == 1)  pack = arg
         if(i == 2)  read(arg,'(I4)') number
         if(i == 3)  read(arg,'(I4)') delay 
         if(i == 4)  lgout = arg
         i = i+1
      END DO
      print *," number ,delay = ",number,delay
      app = "eastappf"
c call login   
      i = east_login(str2c(pack),number,str2c(app))

      print *,"ret of eastlogin =  " ,i
      if (i < 0) then
            print * ,"eastappf license failed\n"
            stop
      endif
 
c delay:
      call sleep(delay); 
      print *,"logout = ",lgout
c logout:
      if (len(lgout) > 0) then
        i = east_logoutf()
        print *,"ret of eastlogout = ",i
        if (i < 0) then
            print *,"app license logout  failed \n"
            stop
        else
             print *,"app   logout ok \n"
        endif

      endif
 
      print *, "end of eastappf"

      END PROGRAM

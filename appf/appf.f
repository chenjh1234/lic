c===c add a "\0" to fortran string end:
      function str2c( pstr) 
      character *(*) pstr 
      character (len = 2048) ::  str2c
      str2c = ""
      str2c =trim(pstr)//"\0"
      return
      end function str2c
c===== promgarm start
      PROGRAM appf
      INTEGER :: i,n,number,delay
      CHARACTER(len=128) :: arg,vender,pack,version,nb,lgout
      CHARACTER(len=128) :: VENDER_SEED,p1,p2,p3,app
      CHARACTER(len=1024) :: pubkey,sign,sid
c   functions:
      character (len = 2048) ::  str2c
      integer:: login_lic,logout_licf
      logical(1):: b ,check_lic

      number = 1
      delay = 100

c seed:
      VENDER_SEED="0123456789\0"
c   pubkey:
      pubkey="-----BEGIN PUBLIC KEY-----\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNAD
     1CBiQKBgQDLpkCvoO/bC7bY2iAKK/LX+yMK\nCnL6FpatrM9Yk59ce7/S/T7OqgRhZY
     1MIhGU6rJOB7finilEYDAtt5OJYUqNF+cS0\nvZENDDsWWPvUYcYEcUurXN/G0PbcTj
     1U90mqcNXx5GC9AQQeV3PYiqzHhYoJqKECB\nibzelvxMWGgLUZM+swIDAQAB\n----
     1-END PUBLIC KEY-----\n\0"
c    args:

c"p1:vender: vender name\n"\
c"p2:package: package name\n"\
c"p3:version: version name\n"\
c"p4:number: number license apply(1)\n"\
c"p5:delay:time in second the app spend(100)\n"\
c"p6:if present: we do logout\n";
 
      i = 0
      DO
         CALL get_command_argument(i, arg)
         IF (LEN_TRIM(arg) == 0) EXIT
         WRITE (*,*) i,arg
         if(i == 1)  vender = arg
         if(i == 2)  pack = arg
         if(i == 3)  version = arg
         if(i == 4)  read(arg,'(I4)') number
         if(i == 5)  read(arg,'(I4)') delay 
         if(i == 6)  lgout = arg
         i = i+1
      END DO
         print *," number ,delay = ",number,delay
         print *," pubkey = ",pubkey
         app = "appf"
c call login   
         i = login_lic(str2c(vender),str2c(pack),str2c(version),number
     1       ,str2c(app),sign,sid)
      print *,"ret of login = " ,i
      if (i < 0) then
            print * ,"app license failed, please check  the  ELOG\n"
            stop
      endif
c check ret
      b = check_lic(sign,sid,trim(pubkey),trim(VENDER_SEED))
      print *,"ret of check = " ,b
      if (b .eqv. .false.) then
            print * ,"app checkValid failed\n";
            stop 
      endif
c delay:
      call sleep(delay); 
      print *,"logout = ",lgout
c logout:
      if (len(lgout) > 0) then
        i = logout_licf()
        print *,"ret logout = ",i
        if (i < 0) then
            print *,"app license logout  failed \n"
            stop
        else
             print *,"app   logout ok \n"
        endif
      endif
      print *, "end of appf"
      END PROGRAM

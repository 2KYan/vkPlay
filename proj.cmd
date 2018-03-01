::Project Startup
@echo off

goto _STARTUP_

:_STARTUP_
    set UCUSERNAME=%USERNAME%
    call :_TOUPPER_ UCUSERNAME
    ::call %HOME%\bin\startup.cmd
    ::
    set CYGWIN=nodosfilewarning
    set TZ=PST8PDT,M3.2.0/2,M11.1.0/2
    doskey gvim=gvim --remote-silent $*
    doskey history=doskey /history
    set PYTHONPATH=%PYTHONPATH%;%HOME%/scripts;
    set PERL5LIB=%PERL5LIB%;%HOME%/scripts;
    
    if exist %ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat (
            set TOOLKIT=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat
    )
    if not defined TOOLKIT set TOOLKIT=%VS140COMNTOOLS%/../../VC/vcvarsall.bat

:_SETUP_
    ::set DEV_HOME=%~p0
    ::set DEV_HOME=%~dp0
    set CURDIR=%~dp0
    set CURDIR=%CURDIR:~0,-1%
    for %%I in ("%CURDIR%") do (
        set "DEV_HOME=%%~fI"
        set "DEPOT=%%~nxI"
        set CURDIR=
    )

:_DEVSETUP_
    if not defined PATH_SHADOW set PATH_SHADOW=%PATH%
    set PATH=%PATH_SHADOW%;
    set PATH_SAVED=

    REM Project specific ENV variables

    if not defined ARCH         set ARCH=x64
    ::if not defined TARGET_ARCH  set TARGET_ARCH=x64
    if not defined BUILD_CONFIG set BUILD_CONFIG=Debug
    if not defined DEV_BRANCH   set DEV_BRANCH=master
    if not "%1"=="" set BUILD_CONFIG=%1 
    if not "%2"=="" set DEV_BRANCH=%2

    set BUILD_CONFIG=%BUILD_CONFIG: =%
    set DEV_BRANCH=%DEV_BRANCH: =%

    goto _COMMON_

:_COMMON_
    set SCRATCHDIR=%DEV_HOME%\build

    set DEV_BINARY=%DEV_HOME%\bin
    set OUTPUTDIR=%SCRATCHDIR%\B_%DEPOT%_%DEV_BRANCH%_%ARCH%_%BUILD_CONFIG%
    set OUTSCRATCH=%SCRATCHDIR%
    set TAG_DB=%OUTPUTDIR%\tags

    set PATH=%DEV_HOME%/extern/bin/%ARCH%;%DEV_BINARY%/%BUILD_CONFIG%;%PATH%
    set PYTHONPATH=%PYTHONPATH%;%DEV_HOME%/scripts;
    set PERL5LIB=%PERL5LIB%;%DEV_HOME%/scripts;

    goto _BUILD_TOOLS_

:_BUILD_TOOLS_
    pushd %DEV_HOME%
    doskey cmake=cmake -A %ARCH%
    call "%TOOLKIT%" %ARCH% >nul 2>&1
    ::call "%GFX_INFRA%/tool/perl/bin/qe.bat" setpath >nul 2>&1
    ::call "%DXSDK_DIR%/Utilities/bin/dx_setenv.cmd" >nul 2>&1
    popd

    goto _PROMPT_

:_PROMPT_
    set prompt=$D $T$h$h$h$h$h$h %DEPOT% %DEV_BRANCH% %ARCH% %BUILD_CONFIG% $+$M$P$G$_$$
    title %DEPOT%_%DEV_BRANCH%_%ARCH%_%BUILD_CONFIG%
    goto _FINISH_

:_FINISH_
    if not exist %OUTPUTDIR%\ mkdir %OUTPUTDIR%
    if not exist %SCRATCHDIR%\ mkdir %SCRATCHDIR%
    if not exist %DEV_BINARY%\ mkdir %DEV_BINARY%
    cd  /D %SCRATCHDIR%\
    goto _END_

:_END_
    start cmd  
    exit

:_TOLOWER_
:: Subroutine to convert a variable VALUE to all lower case.
:: The argument for this subroutine is the variable NAME.
FOR %%i IN ("A=a" "B=b" "C=c" "D=d" "E=e" "F=f" "G=g" "H=h" "I=i" "J=j" "K=k" "L=l" "M=m" "N=n" "O=o" "P=p" "Q=q" "R=r" "S=s" "T=t" "U=u" "V=v" "W=w" "X=x" "Y=y" "Z=z") DO CALL SET "%1=%%%1:%%~i%%"
GOTO:EOF

:_TOUPPER_
:: Subroutine to convert a variable VALUE to all UPPER CASE.
:: The argument for this subroutine is the variable NAME.
FOR %%i IN ("a=A" "b=B" "c=C" "d=D" "e=E" "f=F" "g=G" "h=H" "i=I" "j=J" "k=K" "l=L" "m=M" "n=N" "o=O" "p=P" "q=Q" "r=R" "s=S" "t=T" "u=U" "v=V" "w=W" "x=X" "y=Y" "z=Z") DO CALL SET "%1=%%%1:%%~i%%"
GOTO:EOF

:_TOCAPITAL_
:: Subroutine to convert a variable VALUE to Title Case.
:: The argument for this subroutine is the variable NAME.
FOR %%i IN (" a= A" " b= B" " c= C" " d= D" " e= E" " f= F" " g= G" " h= H" " i= I" " j= J" " k= K" " l= L" " m= M" " n= N" " o= O" " p= P" " q= Q" " r= R" " s= S" " t= T" " u= U" " v= V" " w= W" " x= X" " y= Y" " z= Z") DO CALL SET "%1=%%%1:%%~i%%"
GOTO:EOF

:_TEST_
    if "%1" == "" goto _END_
    echo %1 && shift
    goto _TEST_


@ECHO OFF
SETLOCAL EnableDelayedExpansion
SET EL=0

REM CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
REM IF ERRORLEVEL 1 SET EL=1

:: Build the library files for individual implementations
FOR /D %%K IN (crypto_kem\* crypto_sign\*) DO (
  FOR /D %%L IN (%%K\*) DO (
    CD %%L
    nmake /f Makefile.Microsoft_nmake clean
    IF ERRORLEVEL 1 SET EL=2
    nmake /f Makefile.Microsoft_nmake
    IF ERRORLEVEL 1 SET EL=3
    CD ..\..\..
  )
)

:: Build and run the functional tests and test vector programs for each implementation
FOR %%T IN (kem sign) DO (
  CD crypto_%%T
  FOR /D %%K IN (*) DO (
    SET schemeuppercase=%%K
    FOR %%B IN (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) DO SET "schemeuppercase=!schemeuppercase:%%B=%%B!"
    SET "schemeuppercase=!schemeuppercase:-=!"
    CD %%K
    FOR /D %%L IN (*) DO (
      CD ..\..\test
      nmake /NOLOGO /f Makefile.Microsoft_nmake /E TYPE=%%T SCHEME=%%K SCHEME_UPPERCASE=!schemeuppercase! IMPLEMENTATION=%%L clean
      IF ERRORLEVEL 1 SET EL=4
      nmake /NOLOGO /f Makefile.Microsoft_nmake /E TYPE=%%T SCHEME=%%K SCHEME_UPPERCASE=!schemeuppercase! IMPLEMENTATION=%%L
      IF ERRORLEVEL 1 SET EL=5
      CD ..
      bin\functest_%%K_%%L
      IF ERRORLEVEL 1 SET EL=6
      CD crypto_%%T\%%K
    )
    CD ..
  )
  CD ..
)

EXIT /b %EL%

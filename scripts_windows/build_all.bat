@ECHO OFF
SETLOCAL
SET EL=0

REM CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
REM IF ERRORLEVEL 1 SET EL=1

FOR /D %%J IN (crypto_kem, crypto_sig) DO (
  FOR /D %%K IN (%%J\*) DO (
    FOR /D %%L IN (%%K\*) DO (
      cd %%L
      nmake /f Makefile.Microsoft_nmake clean
      IF ERRORLEVEL 1 SET EL=2
      nmake /f Makefile.Microsoft_nmake
      IF ERRORLEVEL 1 SET EL=3
      cd ..\..\..
    )
  )
)

EXIT /b %EL%

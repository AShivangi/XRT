@echo off
setlocal enabledelayedexpansion

echo Starting continuous GEMM validation...
echo Press Ctrl+C to stop manually, or script will stop on first failure.
echo Script will automatically stop after 10 iterations.
echo ========================================================
echo.

set count=0

:loop
set /a count=count+1

rem Check if we've reached 10 iterations
if !count! gtr 10 (
    echo.
    echo ========================================================
    echo MAXIMUM ITERATIONS REACHED - Stopping after 10 runs
    echo Completed at: %date% %time%
    echo ========================================================
    pause
    exit /b 0
)

echo.
echo ========================================================
echo ITERATION !count! of 10
echo Started at: %date% %time%
echo ========================================================

call xrt-smi validate -r gemm

if errorlevel 1 (
    echo.
    echo ========================================================
    echo [ITERATION !count!] FAILED - Exit code: %errorlevel%
    echo Failed at: %date% %time%
    echo Total iterations completed: !count!
    echo ========================================================
    echo Script stopping due to failure.
    pause
    exit /b %errorlevel%
)

echo.
echo [ITERATION !count!] PASSED - Completed at: %date% %time%
echo Continuing to next iteration...

goto loop
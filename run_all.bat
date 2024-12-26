@echo off
REM Batch script to run executables for day1 to day25 if they exist
REM First run without output, then run with output for each executable

REM Enable delayed variable expansion
SETLOCAL ENABLEDELAYEDEXPANSION

REM Set the base path where the executables are located
SET BASE_PATH=C:\Users\matth\source\repos\AOC2024\out\build\x64-release

REM Loop through day1 to day25
for /L %%x in (1,1,25) do (
    REM Build the path to the directory and executable using delayed expansion
    SET DIR=%BASE_PATH%\day%%x
    SET EXE=!DIR!\day%%x.exe

    REM Check if the directory exists and if the executable exists
    IF EXIST "!EXE!" (
        REM First run: Run the executable without output (to cache the input)
        echo Running !EXE! without output...
        cd /D "!DIR!"
        "!EXE!" >nul 2>&1

        REM Second run: Run the executable with output
        echo Running !EXE! with output...
        cd /D "!DIR!"
        "!EXE!"
    ) ELSE (
        REM If executable does not exist, print a message
        echo !EXE! does not exist, skipping...
    )
)

REM End of script
echo All executables checked and run.
pause

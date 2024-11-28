@echo off
echo Creating deployment directory...
if exist deploy rmdir /s /q deploy
mkdir deploy

echo Copying executable...
copy /Y build\BinarySearchTreeVisualization.exe deploy\

echo Running windeployqt...
C:\Qt\6.8.0\mingw_64\bin\windeployqt.exe --qmldir . --no-translations --compiler-runtime deploy\BinarySearchTreeVisualization.exe

echo Copying additional runtime files...
copy /Y C:\Qt\6.8.0\mingw_64\bin\libgcc_s_seh-1.dll deploy\
copy /Y C:\Qt\6.8.0\mingw_64\bin\libstdc++-6.dll deploy\
copy /Y C:\Qt\6.8.0\mingw_64\bin\libwinpthread-1.dll deploy\

echo Creating resources directory...
mkdir deploy\resources
copy /Y resources\logo.svg deploy\resources\

echo Creating README...
copy /Y README.md deploy\

echo Creating installer configuration...
echo [Setup] > installer.iss
echo AppName=BST Visualizer >> installer.iss
echo AppVersion=1.0 >> installer.iss
echo DefaultDirName={pf}\BST Visualizer >> installer.iss
echo DefaultGroupName=BST Visualizer >> installer.iss
echo OutputDir=. >> installer.iss
echo OutputBaseFilename=BST_Visualizer_Setup >> installer.iss
echo [Files] >> installer.iss
echo Source: "deploy\*"; DestDir: "{app}"; Flags: recursesubdirs >> installer.iss
echo [Icons] >> installer.iss
echo Name: "{group}\BST Visualizer"; Filename: "{app}\BinarySearchTreeVisualization.exe" >> installer.iss
echo Name: "{group}\Uninstall BST Visualizer"; Filename: "{uninstallexe}" >> installer.iss

echo Deployment package created in 'deploy' directory
echo To create an installer, use Inno Setup with the generated installer.iss file

# PowerShell Deployment Script for BST Visualizer
Write-Host "BST Visualizer Deployment Script" -ForegroundColor Green

# Configuration
$QtPath = "C:\Qt\6.8.0\mingw_64"
$CMakePath = "C:\Qt\Tools\CMake_64\bin\cmake.exe"
$MinGWPath = "C:\Qt\Tools\mingw1310_64"
$BuildDir = ".\build"
$DeployDir = ".\deploy"

# Add MinGW to PATH
$env:Path = "$MinGWPath\bin;$env:Path"

# Verify tools exist
if (-not (Test-Path $CMakePath)) {
    Write-Host "Error: CMake not found at $CMakePath" -ForegroundColor Red
    exit 1
}

if (-not (Test-Path "$MinGWPath\bin\mingw32-make.exe")) {
    Write-Host "Error: MinGW not found at $MinGWPath" -ForegroundColor Red
    exit 1
}

if (-not (Test-Path $QtPath)) {
    Write-Host "Error: Qt not found at $QtPath" -ForegroundColor Red
    exit 1
}

# Create build directory if it doesn't exist
Write-Host "Setting up build directory..." -ForegroundColor Yellow
if (-not (Test-Path $BuildDir)) {
    New-Item -Path $BuildDir -ItemType Directory
}

# Build the project
Write-Host "Building project..." -ForegroundColor Yellow
Push-Location $BuildDir
& $CMakePath .. -G "MinGW Makefiles" `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_CXX_COMPILER="$MinGWPath\bin\g++.exe" `
    -DCMAKE_C_COMPILER="$MinGWPath\bin\gcc.exe" `
    -DCMAKE_MAKE_PROGRAM="$MinGWPath\bin\mingw32-make.exe" `
    -DCMAKE_PREFIX_PATH="$QtPath" `
    -DQt6_DIR="$QtPath\lib\cmake\Qt6"

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    Pop-Location
    exit 1
}

& $CMakePath --build . --config Release
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    Pop-Location
    exit 1
}
Pop-Location

# Create deploy directory
Write-Host "Creating deployment directory..." -ForegroundColor Yellow
if (Test-Path $DeployDir) {
    Remove-Item -Path $DeployDir -Recurse -Force
}
New-Item -Path $DeployDir -ItemType Directory

# Create resources directory in deploy
New-Item -Path "$DeployDir\resources" -ItemType Directory

# Copy executable
Write-Host "Copying executable..." -ForegroundColor Yellow
Copy-Item -Path "$BuildDir\BinarySearchTreeVisualization.exe" -Destination $DeployDir

# Run windeployqt
Write-Host "Running windeployqt..." -ForegroundColor Yellow
& "$QtPath\bin\windeployqt.exe" --qmldir . --no-translations --compiler-runtime "$DeployDir\BinarySearchTreeVisualization.exe"

# Copy MinGW runtime DLLs
Write-Host "Copying MinGW runtime..." -ForegroundColor Yellow
Copy-Item -Path "$QtPath\bin\libgcc_s_seh-1.dll" -Destination $DeployDir
Copy-Item -Path "$QtPath\bin\libstdc++-6.dll" -Destination $DeployDir
Copy-Item -Path "$QtPath\bin\libwinpthread-1.dll" -Destination $DeployDir

# Copy resources
Write-Host "Copying resources..." -ForegroundColor Yellow
if (Test-Path ".\resources\logo.svg") {
    Copy-Item -Path ".\resources\logo.svg" -Destination "$DeployDir\resources"
} else {
    Write-Host "Warning: logo.svg not found" -ForegroundColor Yellow
}

if (Test-Path ".\README.md") {
    Copy-Item -Path ".\README.md" -Destination $DeployDir
} else {
    Write-Host "Warning: README.md not found" -ForegroundColor Yellow
}

# Create installer directory
Write-Host "Creating installer directory..." -ForegroundColor Yellow
if (-not (Test-Path ".\installer")) {
    New-Item -Path ".\installer" -ItemType Directory
}

Write-Host "Deployment package created successfully!" -ForegroundColor Green
Write-Host "You can now use Inno Setup to create the installer." -ForegroundColor Green
Write-Host "Open installer.iss in Inno Setup Compiler and click Compile." -ForegroundColor Green

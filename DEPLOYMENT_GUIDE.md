# Qt Project Deployment Guide

## Quick Start
1. Copy `deploy.ps1` and `installer.iss` to your project
2. Update Qt paths in `deploy.ps1`
3. Run `deploy.ps1`
4. Compile `installer.iss` with Inno Setup

## Detailed Steps

### 1. Prerequisites Installation
- [ ] Qt 6.x (e.g., Qt 6.8.0)
- [ ] CMake (comes with Qt)
- [ ] MinGW (comes with Qt)
- [ ] Inno Setup Compiler

### 2. File Setup

#### 2.1 Copy These Files
1. `deploy.ps1` - Deployment script
2. `installer.iss` - Installer configuration

#### 2.2 Update Paths
In `deploy.ps1`, update these variables:
```powershell
$QtPath = "C:\Qt\6.8.0\mingw_64"              # Your Qt version
$CMakePath = "C:\Qt\Tools\CMake_64\bin\cmake.exe"
$MinGWPath = "C:\Qt\Tools\mingw1310_64"        # Your MinGW version
```

#### 2.3 Update Installer Info
In `installer.iss`, update these definitions:
```inno
#define MyAppName "Your App Name"
#define MyAppVersion "1.0"
#define MyAppPublisher "Your Name"
#define MyAppURL "Your Website/GitHub"
#define MyAppExeName "YourApp.exe"
```

### 3. Deployment Process

#### 3.1 Run Deployment Script
```powershell
# Open PowerShell and navigate to project directory
cd YourProjectPath
powershell -ExecutionPolicy Bypass -File deploy.ps1
```

This will:
- Build your project
- Create `deploy` directory
- Copy executable
- Deploy Qt dependencies
- Copy required DLLs

#### 3.2 Create Installer
1. Open `installer.iss` in Inno Setup Compiler
2. Press Ctrl+F9 or click Compile
3. Find installer in `installer` folder

### 4. Directory Structure
```
YourProject/
├── src/                 # Your source files
├── deploy.ps1           # Deployment script
├── installer.iss        # Installer script
├── build/              # Created by deploy.ps1
├── deploy/             # Created by deploy.ps1
└── installer/          # Created by Inno Setup
```

### 5. Troubleshooting

#### Common Issues

1. **CMake Not Found**
   ```powershell
   Error: CMake not found at [path]
   ```
   ➜ Update `$CMakePath` in deploy.ps1

2. **Qt Not Found**
   ```powershell
   Error: Qt not found at [path]
   ```
   ➜ Update `$QtPath` to match your Qt installation

3. **MinGW Not Found**
   ```powershell
   Error: MinGW not found at [path]
   ```
   ➜ Update `$MinGWPath` to match your MinGW version

4. **Missing DLLs**
   ```
   Error: Cannot find [dll_name]
   ```
   ➜ Check if windeployqt ran successfully
   ➜ Run windeployqt manually:
   ```powershell
   [QtPath]\bin\windeployqt.exe [YourApp.exe]
   ```

### 6. Verification Checklist

Before Distribution:
- [ ] Application runs from `deploy` folder
- [ ] All required DLLs are present
- [ ] Resources are copied correctly
- [ ] Installer creates start menu shortcuts
- [ ] Uninstaller works properly
- [ ] Application runs after installation

### 7. Tips & Best Practices

1. **Testing**
   - Test deployed app before creating installer
   - Test installer on clean Windows installation
   - Verify all features work after installation

2. **Path Management**
   - Keep installation paths short
   - Avoid spaces in paths when possible
   - Use environment variables when needed

3. **Deployment Options**
   ```powershell
   # Additional windeployqt options
   --no-translations    # Skip translations
   --no-system-d3d-compiler  # Skip D3D compiler
   --no-virtualkeyboard  # Skip virtual keyboard
   ```

4. **Installer Options**
   ```inno
   ; Add to installer.iss for custom features
   PrivilegesRequired=lowest  ; User-level install
   Compression=lzma2/ultra64  ; Better compression
   SetupIconFile=myicon.ico  ; Custom icon
   ```

### 8. Command Reference

```powershell
# Deploy
.\deploy.ps1

# Manual CMake build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

# Manual windeployqt
windeployqt.exe --qmldir . --no-translations YourApp.exe

# Run installer build (from Inno Setup Command Line Compiler)
iscc.exe installer.iss
```

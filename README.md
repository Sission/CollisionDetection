# CollisionDetection
## Setting
1. Open the project "CollisionDetection.vcxproj" in the Visual Studio;
2. Go to project name -> "Properties", then set the "Configuration" as "Release" and "Platform" as "x64";
3. Go to project name -> "Properties" -> "VC++ Directories", include the following two directories in the "Include Directories" and "Library Directories":
    1. ..\Libs\tinyobjloader\include
    1. ..\Libs\DirectXTex\include
4. Go to "Properties" -> "Linker" -> "General", add the path to your "DirectX-Tex.lib" file to the "Additional Library Directories";
5. Set All.hlslshader files’ -> "Properties" -> "General" -> "Excluded From Buildas" -> "Yes".
## Result

# CollisionDetection
## Requirement
1. A D3D11 capable graphics card with following driver versions:
NVIDIA: GeForce Game Ready Driver 372.90 or above
AMD: Radeon Software Version 16.9.1 or above
2. To build the demo the following is required:
Visual Studio 2019

## Setting
1. Open the project "CollisionDetection.vcxproj" in the Visual Studio;
2. Go to project name -> "Properties", then set the "Configuration" as "Release" and "Platform" as "x64";
![alt text](https://github.com/Sission/CollisionDetection/blob/main/Images/VSDirect.png)
3. Go to project name -> "Properties" -> "VC++ Directories", include the following two directories in the "Include Directories" and "Library Directories":
    1. ..\Libs\tinyobjloader\include
    1. ..\Libs\DirectXTex\include
4. Go to "Properties" -> "Linker" -> "General", add the path to your "DirectX-Tex.lib" file to the "Additional Library Directories";
![alt text](https://github.com/Sission/CollisionDetection/blob/main/Images/Linker.png)
5. Set All.hlslshader files’ -> "Properties" -> "General" -> "Excluded From Buildas" -> "Yes".
![alt text](https://github.com/Sission/CollisionDetection/blob/main/Images/HSLS.png)
## Meshed Objects
Stanford Bunny
* 15258 vertices
* 30338 faces

![alt text](https://github.com/Sission/CollisionDetection/blob/main/Images/BunnyMesh.png)

Statue
* 2835 vertices
* 5670 faces

![alt text](https://github.com/Sission/CollisionDetection/blob/main/Images/statue.png)

## GUI
* Object and camera keyboard and mouse control:
    * 1,2,3,4 to lock the target;
    * A,W,S,D,Q,Z to control the translation along axis;
    * I,K,J,L to control the rotation;
    * Mouse to control the view angle.
* Real Time data visualization:
    * Control Instruction;
    * Show the real-time triangle in collision;
    * The number of total counted frames;
    * Refresh rate (time/frame)
* Features:
    * Show the real-time triangle in collision;
    * Can load all .obj files less than 6M;
    * The number of total counted frames;
    * Texture loadable.

![alt text](https://github.com/Sission/CollisionDetection/blob/main/Images/GUI.png)
## Result
## References
[1] PardCode, C++ 3D Game Tutorial Series, (2020), GitHub repository, https://github.com/PardCode/CPP-3D-Game-Tutorial-Series.

[2] tinyobjloader(2020), GitHub repository, https://github.com/tinyobjloader/tinyobjloader

[3] Gottschalk, S. (1996). Separating axis theorem. Technical Report TR96-024, Department of Computer Science, UNC Chapel Hill.

[4] dyn4j, Blogs on Collision Detection: http://www.dyn4j.org/category/gamedev/collision-detection/

[5] Thomas Larsson and Tomas Akenine-Möller. “Collision detection for continuously deformingbodies”. In: Eurographics. 2001.

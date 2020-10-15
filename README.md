# CollisionDetection
## Requirement
1. A D3D11 capable graphics card with following driver versions:
* NVIDIA: GeForce Game Ready Driver 372.90 or above;
* AMD: Radeon Software Version 16.9.1 or above;
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

We categorize the pipeline into 4 major components: Tree construction, tree search, primitive test, and 3D rendering. Where the term tree search includes the combined time cost of broad phase and narrow phase detection. A simple collision routine is designed for two cases. In the first case, we move a Stanford bunny model toward another fixed model in the same type. The movement includes consistent translations and rotations. In the second case, we replicate the operation in the first case on two statue models with fewer primitives and smaller octrees. For both cases, we record the time consumption per frame of the mentioned four components as well as the collision scale along the whole process. The collision scale is quantified by the number of colliding primitives in the moving model times the number of colliding primitives in the fixed model, which indicates the total number of possible collision pairs traversed by the primitive test. The results are presented by the following figures.

From both figures, we notice that the time cost of primitive test and tree search starts to grow after the first collision happened and keeps increasing as the collision scale grows. While the time cost for primitive test grows much faster and exceed the cost of tree construction after reaching a certain level of collisions. The tree construction and 3D rendering time remain in stable before and after the collision. Notably, the time for rendering decreases after the collision happened. For this problem, we haven't found a reasonable explanation for it yet. While the increase of tree construction time is mainly due to the broad phase detection that limits the tree depth before the collision is detected. In addition, the first case contains more meshes and therefore is modeled with deeper BVHs, which results in a longer construction time after the collision happened.

![alt text](https://github.com/Sission/CollisionDetection/blob/main/Images/bunnytime.png)

![alt text](https://github.com/Sission/CollisionDetection/blob/main/Images/statuetime.png)

## References
[1] PardCode, C++ 3D Game Tutorial Series, (2020), GitHub repository, https://github.com/PardCode/CPP-3D-Game-Tutorial-Series.

[2] tinyobjloader(2020), GitHub repository, https://github.com/tinyobjloader/tinyobjloader

[3] Gottschalk, S. (1996). Separating axis theorem. Technical Report TR96-024, Department of Computer Science, UNC Chapel Hill.

[4] dyn4j, Blogs on Collision Detection: http://www.dyn4j.org/category/gamedev/collision-detection/

[5] Thomas Larsson and Tomas Akenine-Möller. “Collision detection for continuously deformingbodies”. In: Eurographics. 2001.

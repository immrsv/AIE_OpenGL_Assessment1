
Graphics Engine
Assessment Piece: Computer Graphics 
AIE 2017 Semester 1

Author: Graeme Jarvis
SID: s171866

==================================

Camera Controls:
+ Right-Mouse drag to look around
+ QE to roll
+ WSAD to move
+ Left-Shift to slow movement rate


Directory Structure:
+ (Solution)/bin : Linker output
+ (Solution)/obj : Compiler output/Intermediary objects
+ (Solution)/resources : static resources (models, textures, etc)
+ (Solution)/dependencies : 3rd Party libs/sdks


Known Issues:  
+ Single-Reflection mirrors work correctly, but secondary-reflection mirrors are incorrectly reflecting from "Scene" camera and not from "Observer" (primary-mirror) camera.

Comments:
+ UI elements for Entity rotations (Orientation and Spin) are Quaternion->Euler-Angle conversions.  Due to how Quaternions are translated, angles > 90 degrees may return odd representations.
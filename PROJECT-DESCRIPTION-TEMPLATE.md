# Desert of the Amethyst Core

166/2022 - Anđela Spasić  
A desert crystal core radiating as night falls, casting glow and shadow across the wasteland around it

## Controls

W/A/S/D -> Move forward/left/backward/right  
Mouse -> Look around Space -> Begin scene event chain F1 -> Toggle mouse cursor F2 -> Toggle debug GUI  
Esc -> Exit

## Features

### Fundamental:

[X] Model with lighting
[X] Two types of lighting with customizable colors and movement through GUI or ACTIONS
[X] Space pressed --- 3 seconds ---Triggers---> Moon rises ---> 20 seconds ---Triggers---> The crystal core ignites
casting Point Shadows on the scenery around it

### Group A:

[ ] Frame-buffers with post-processing   
[ ] Off-screen Anti-Aliasing  
[ ] Parallax Mapping
[X] Bloom with the use of HDR

### Group B:

[ ] Deferred Shading  
[X] Point Shadows  
[ ] SSAO

### Engine improvement:

[X] Light class hierarchy (`Light` base + `DirectionalLight`/`PointLight`)
[X] `BloomEffect` - HDR scene capture, bright-pass extract, ping-pong blur, combine
[X] `PointShadowFramebuffer` - cubemap depth capture
[X] Extended texture loading to support gltf format

## Models:

Crystals of Power - https://sketchfab.com/3d-models/crystals-of-power-f07e839957784ea390d512eab6fb61a8  
Wasteland Wagon - https://sketchfab.com/3d-models/wasteland-wagon-5a819233c4f24486bf655b3176c35103  
Planet - https://sketchfab.com/3d-models/mars-0bbd08db494b4e2a873c624021248d12  
Rock (12) - https://sketchfab.com/3d-models/rock-12-40b7f01003b14a95beba0361e6d3ca99  
Rock (15) - https://sketchfab.com/3d-models/rock-15-693dae21edda436694432530f8317506  
Rock (39) - https://sketchfab.com/3d-models/rock-39-f56f4adb41824360bdac7206d162a851  
Rock (41) - https://sketchfab.com/3d-models/rock-41-190182a8abbe4efcb7a6810ed322cccd  
Saguaro cactus - https://sketchfab.com/3d-models/saguaro-cd04754f15b7469a89fdcfb701a2522c  
Saguaro cactus 2 - https://sketchfab.com/3d-models/realistic-hd-saguaro-cactus-2030-532f007acf504ef49e231efbe357f79f

## Textures

Skybox (soft2) - https://skyboxgen.firebaseapp.com/

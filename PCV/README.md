# READ ME
## Requirement
Windows, vs 2017(not tested in other version), c++17 (used some c++17 features, it should have been set in project settings, no need to set it)

## Tips
This tool is for visualization of depth image. If give final image together, point clouds can have color.

Be cautious: 
* use the name convention: xxxx_depth.png, xxxx_final.png. Both should be 32bit(i.e. RGBA).
* Current depth image assumes you use gray scale and use red channel as depth. Say for a pixel of depth image (r,g,b,a). In the codes, point clouds z-value is 1.0 / r. So suggest you modify your depth image in this way. (Or you can change the codes directly, see the )


## Camera control
To control the camera, press mouse left button, don't release, and move mouse to change camera's orientation.

Use WASD to control forward, backward, left and right. Use Q and E to go up and down.


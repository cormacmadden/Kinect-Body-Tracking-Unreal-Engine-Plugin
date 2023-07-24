# Azure Kinect Plugin for Unreal Engine
Exposes Azure Kinect Support for integration into Unreal Engine Applications.

Updated Version of nama-gatsu's Azure Kinect Unreal Engine Plugin.
 - Fixed Importing Binaries Issues
 - Added Translation to skeletal tracking

![](./Docs/Sequence%2001.gif)

## Setup Instructions
* Platform: Win64
* Unreal Engine Version 5.2
* Download and Install `Azure Kinect SDK v1.4.1` from [here](https://github.com/microsoft/Azure-Kinect-Sensor-SDK/blob/develop/docs/usage.md)
* Download and Install `Azure Kinect Body Tracking SDK v1.1.0` from [here](https://docs.microsoft.com/en-us/azure/Kinect-dk/body-sdk-download)
* Create the following two System Environment variables:
    * AZUREKINECT_SDK that points to the Azure Kinect Body Tracking SDK root path `C:\Program Files\Azure Kinect SDK v1.4.1`
    * AZUREKINECT_BODY_SDK that points to the Azure Kinect Body Tracking SDK root path `C:\Program Files\Azure Kinect Body Tracking SDK`
* Add the following three paths to the PATH system environment variable
    * `C:\Program Files\Azure Kinect Body Tracking SDK\sdk\windows-desktop\amd64\release\bin`
    * `C:\Program Files\Azure Kinect SDK v1.4.1\sdk\windows-desktop\amd64\release\bin`
    * `C:\Program Files\Azure Kinect SDK v1.4.1\sdk\netstandard2.0\release`
* Create a Plugins folder inside your unreal project folder (Where the .uprojrct is located) 
* Clone this repo into the plugins folder.
## Features

### In-Editor activation

* Write Depth / Color buffer into `RenderTarget2D`s. 

![](./Docs/in-editor.gif)

### Blueprint activation

![](./Docs/bp.png)

### Niagara Particle

* You can modify base a niagara system `NS_KinectParticle`.

![](./Docs/animation.gif)


### Skeleton tracking

![]()

* Bone mapping node in Anim Graph

![](./Docs/animgraph.jpg)

## Notice

Depthe data are stored `RenderTarget2D` into standard 8bit RGBA texture.  
R: first 8bit as `uint8` of original `uint16` sample  
G: last 8bit as `uint8` of original `uint16` sample  
B: `0x00` or `0xFF` (if depth sample is invalid)  
A: `0xFF` (Constant value)

Thus we need conversion to acquire orignal depth samples.
```
// In MaterialEditor or Niagara, sample values in Depth texture are normalized to 0-1.
float DepthSample = (G * 256.0 + R) * 256.0; // millimetor
```

```
// In C++
uint8 R = Sample.R, G = Sample.G;
uint16 DepthSample = G << 8 | R;  // millimetor
```

Depth pixel from Azure Kinect SDK is originally a single `uint16` in millimetor. But `RenderTarget2D` can't store `uint16` as texture (`EPixelFormat::PF_R16_UINT` doesn't work for RenderTarget). 

# License
## MIT License
Copyright 2021 Ayumu Nagamtsu

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
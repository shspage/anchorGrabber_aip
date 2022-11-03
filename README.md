# anchorGrabber.aip

[readme in japanese](https://github.com/shspage/anchorGrabber_aip/blob/master/readme_ja.md)

__anchorGrabber.aip__  is a plug-in for Adobe Illustrator 2022 (mac).

You can move anchors close to the position where you started dragging together.

This plug-in uses Dear ImGui (https://github.com/ocornut/imgui) for the settings dialog.


## How To Use

![image](https://gist.github.com/shspage/5e54612b4b46ee946327a4436ad2f410/raw/b789c544524a2cd49c68a815210f480678fc89d8/anchorgrabber.png)  
video - 
https://twitter.com/shspage/status/1502867048992423937


## Items in the Settings Window

Double-click the tool icon to display the "__pixel distance settings__" dialog.

* __to grab anchors__ : Anchors within the set number of pixels (\*) distance centered on the tip of the pointer will be the target of movement. (\*: Number of pixels on the screen)
* __to ignore move__ : Movements less than the set number of pixels are ignored. (To prevent movement due to careless clicks.) You can move within the set distance by moving it beyond the set distance and then returning it.


## Working Environment

Adobe Illustrator 2022 (macOS (Apple Silicon))

<!-- Windows10 (win version) -->


## Development Environment

Adobe Illustrator 2022 SDK  
Xcode 13.3.1 / macOS 12.3 sdk / MacOS Monterey 12.3.1  
<!-- Xcode 12.5.1 / macOS 11.3 sdk / MacOS Big Sur -->
MacBook Air (M1)

<!-- Visual Studio 2017 / Windows10 -->


## Build

### Mac

Open _myImGuiDialog_mac/myImGuiDialog.xcodeproj_ and build _libmyImGuiDialog.a_ first.
Then open _anchorGrabber.xcodeproj_ and build _anchorGrabber.aip_

<!-- ### Windows

Open _SHFXRegularPolygon.sln_ and build solution.  
Projects will be built in the order of _myImGuiDailog_win_ and _SHFXRegularPolygon_ . -->


## Note For Build

* Place "anchorGrabber_aip" folder directly under Illustrator SDK's samplecode folder.
<!-- * (Windows) To build with the attached project file, it is necessary to convert the character code of the source code (.cpp, .h, .hpp) from UTF-8 to Multibyte(cp932). -->

## Installation

(After build)
Place __anchorGrabber.aip__ in the Adobe Illustrator plugin folder or additional plugin folder.

The next time you run Illustrator, the plug-in will be loaded and the icon for this plug-in will be added to the "Add-ons" category at the bottom of the "All Tools" drawer in the toolbox. You can use it by dragging the icon to the toolbox and placing it.

## License

MIT License  
Copyright (c) 2022 Hiroyuki Sato  
https://github.com/shspage  
See LICENSE.txt for details.

This software depends on the following resources which may have the licenses differs from this software.

* Dear ImGui : Copyright (c) 2014-2020 Omar Cornut  
Licensed under the MIT License  
https://github.com/ocornut/imgui

* Adobe Illustrator 2022 SDK  
Copyright (c) 2022 Adobe. All rights reserved.  
https://www.adobe.io/


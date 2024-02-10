// verinfo.h
#ifndef __VERINFO_H__
#define __VERINFO_H__

// Menu and About dialog
#ifndef kMySDKPluginName
#define kMySDKPluginName "anchorGrabber"
#endif

// -----------------------
// Version Info
#define kMyVerinfoMajorVersion 1
#define kMyVerinfoMinorVersion 2
#define kMyVerinfoRevisionVersion 0
#define kMyVerinfoBuildNumber 1
#define kMyVerinfoBetaString ""

#define kMyVerinfoOriginalFilename "anchorGrabber.aip"
#define kMyVerinfoFileDescription "Adobe Illustrator 2022 plug-in tool"


// --------
// rarely need to modify stuffs
#define kMyVerinfoLegalCopyright "Copyright 2022 Hiroyuki Sato, All rights reserved."

#define kMyVerinfoURL "http://github.com/shspage"
#define kMyVerinfoCompanyName ", " kMyVerinfoURL

#define kMyAboutDLOGPluginGroupName "AboutShspagePlugIns"
#define kMyAboutDLOGPluginGroupNameString "About Shspage plug-ins"


// ----
// no need to modify stuffs
#define __STRING__(x) MySDKDef_InternalMacroToString(x)
#define MySDKDef_InternalMacroToString(x) #x
// Menu and About Dialog
#define kMyAboutDLOGPopupTitle "About " kMySDKPluginName
#define kMySDKPluginVersion __STRING__(kMyVerinfoMajorVersion) "." __STRING__(kMyVerinfoMinorVersion) kMyVerinfoBetaString
#define kMyLegacyAboutDLOGString kMySDKPluginName " " kMySDKPluginVersion "\\n" kMyVerinfoURL
#define kMyLegacyAboutDLOGCopyRightString kMyVerinfoLegalCopyright
#define kMyAboutDLOGPopupString  kMyLegacyAboutDLOGString "\\n" kMyLegacyAboutDLOGCopyRightString "\\n\\n"
// Version Info
#define kMyVerinfoInternalName kMySDKPluginName
#define kMyVerinfoFileVersionString kMySDKPluginVersion
#define kMyVerinfoProductName kMyVerinfoInternalName
#define kMyVerinfoProductVersionString kMySDKPluginVersion

#endif

#include "IllustratorSDK.h"
#include "anchorGrabberSuites.h"

// Suite externs
extern "C"
{
	SPBlocksSuite* sSPBlocks = NULL;
	AIUnicodeStringSuite* sAIUnicodeString = NULL;
    AIToolSuite* sAITool = nullptr;
    AIArtSuite* sAIArt = NULL;
    AIPathSuite* sAIPath = NULL;
    AIHitTestSuite* sAIHitTest = NULL;
    AIDocumentViewSuite* sAIDocumentView = NULL;
    AIMatchingArtSuite* sAIMatchingArt = NULL;
}

// Import suites
ImportSuite gImportSuites[] = 
{
	kSPBlocksSuite, kSPBlocksSuiteVersion, &sSPBlocks,
	kAIUnicodeStringSuite, kAIUnicodeStringVersion, &sAIUnicodeString,
    kAIToolSuite, kAIToolVersion, &sAITool,
    kAIArtSuite, kAIArtSuiteVersion, &sAIArt,
    kAIPathSuite, kAIPathSuiteVersion, &sAIPath,
    kAIHitTestSuite, kAIHitTestSuiteVersion, &sAIHitTest,
    kAIDocumentViewSuite, kAIDocumentViewSuiteVersion, &sAIDocumentView,
    kAIMatchingArtSuite, kAIMatchingArtSuiteVersion, &sAIMatchingArt,
	nullptr, 0, nullptr
};

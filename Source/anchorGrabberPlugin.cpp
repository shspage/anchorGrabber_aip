#include "IllustratorSDK.h"
#include "anchorGrabberPlugin.h"
#include "anchorGrabberSuites.h"
#include "SDKErrors.h"
#include "myImGuiDialog.h"
#include <functional>

#ifndef CHKERR
#define CHKERR aisdk::check_ai_error(error)
#endif

Plugin* AllocatePlugin(SPPluginRef pluginRef)
{
	return new anchorGrabberPlugin(pluginRef);
}

void FixupReload(Plugin* plugin)
{
	anchorGrabberPlugin::FixupVTable((anchorGrabberPlugin*) plugin);
}

anchorGrabberPlugin::anchorGrabberPlugin(SPPluginRef pluginRef) :
	Plugin(pluginRef),
    fToolHandle(NULL),
    fResourceManagerHandle(NULL),
    fShutdownApplicationNotifier(NULL)
{
	strncpy(fPluginName, kanchorGrabberPluginName, kMaxStringLength);
}

anchorGrabberPlugin::~anchorGrabberPlugin()
{
}

ASErr anchorGrabberPlugin::StartupPlugin( SPInterfaceMessage *message )
{
    ASErr error = kNoErr;
    try {
        error = Plugin::StartupPlugin(message); CHKERR;
        
        // init parameters
        gparms.tolerance_drag = TOLERANCE_DRAG;
        gparms.tolerance_safe_click = TOLERANCE_SAFE_CLICK;
        
        gparms.mouseDowned = false;
        gparms.safeClick = true;
        
        gparms.curPointPtr = nullptr;
        
        error = AddNotifier(message); CHKERR;
        error = AddTool(message);
    }
    catch (ai::Error& ex) {
        error = ex;
    }
    catch (...)
    {
        error = kCantHappenErr;
    }
    return error;
}

ASErr anchorGrabberPlugin::ShutdownPlugin( SPInterfaceMessage *message )
{
    resetStatus();
    
    return Plugin::ShutdownPlugin(message);
}

ASErr anchorGrabberPlugin::PostStartupPlugin(){
    ASErr error = kNoErr;
    try {
        error = sAIUser->CreateCursorResourceMgr(fPluginRef, &fResourceManagerHandle); CHKERR;
    }
    catch (ai::Error& ex) {
        error = ex;
    }
    catch (...)
    {
        error = kCantHappenErr;
    }
    return error;
}

ASErr anchorGrabberPlugin::AddNotifier(SPInterfaceMessage *message){
    ASErr error = kNoErr;
    try {
        error = sAINotifier->AddNotifier(fPluginRef,
                                         "anchorGrabber " kAIApplicationShutdownNotifier,
                                         kAIApplicationShutdownNotifier,
                                         &fShutdownApplicationNotifier); CHKERR;
    }
    catch (ai::Error& ex) {
        error = ex;
    }
    catch(...)
    {
        error = kCantHappenErr;
    }
    return error;
    
}

ASErr anchorGrabberPlugin::Notify(AINotifierMessage* message){
    ASErr error = kNoErr;
    try
    {
        if (message->notifier == fShutdownApplicationNotifier)
        {
            if(fResourceManagerHandle != NULL)
            {
                sAIUser->DisposeCursorResourceMgr(fResourceManagerHandle);
                fResourceManagerHandle = NULL;
            }
        }
    }
    catch (ai::Error& ex) {
        error = ex;
    }
    catch(...)
    {
        error = kCantHappenErr;
    }
    
    return error;
}

AIErr anchorGrabberPlugin::AddTool( SPInterfaceMessage* message )
{
    AIErr error = kNoErr;
    AIAddToolData toolData;
    toolData.title = ai::UnicodeString::FromRoman("anchorGrabber");
    toolData.tooltip = ai::UnicodeString::FromRoman("anchorGrabber");
    toolData.sameGroupAs = kNoTool;
    toolData.sameToolsetAs = kNoTool;
    toolData.normalIconResID = kanchorGrabberToolIconResourceID;
    toolData.darkIconResID = kanchorGrabberToolIconResourceID;
    toolData.iconType = ai::IconType::kSVG;
    
    ai::int32 options = kToolWantsToTrackCursorOption;
    
    
    error = sAITool->GetToolNumberFromName(kanchorGrabberTool, &toolData.sameGroupAs);
    if (error) return error;
    
    error = sAITool->GetToolNumberFromName(kanchorGrabberTool, &toolData.sameToolsetAs);
    if (error) return error;
    
    if (!error) {
        error = sAITool->AddTool( message->d.self, kanchorGrabberTool, toolData,
                                 options, &fToolHandle);
    }
    
    return error;
}

ASErr anchorGrabberPlugin::SelectTool(AIToolMessage* message){
    ASErr error = kNoErr;
    return error;
}

void anchorGrabberPlugin::callbackFunc() {
    /* Used for processing during dialog display such as preview */
}

ASErr anchorGrabberPlugin::EditTool(AIToolMessage* message){
    ASErr error = kNoErr;
    
    tmpParms.tolerance_drag = gparms.tolerance_drag;
    tmpParms.tolerance_safe_click = gparms.tolerance_safe_click;

    std::function<void(void)> getCallback = std::bind(&anchorGrabberPlugin::callbackFunc, this);

#ifdef __APPLE__
    int dialogResult = myImGuiDialog::runModal(&tmpParms, getCallback);
#else
	AIWindowRef hwnd;
	error = sAIAppContext->GetPlatformAppWindow(&hwnd);
	int dialogResult = myImGuiDialog::runModal(hwnd, &tmpParms, getCallback);
#endif

    if (dialogResult == 2) {
        gparms.tolerance_drag = tmpParms.tolerance_drag;
        gparms.tolerance_safe_click = tmpParms.tolerance_safe_click;
    }
    
    return error;
}

// ----
static inline AIRealPoint pSub(const AIRealPoint &p1, const AIRealPoint &p2){
    return AIRealPoint{ p1.h - p2.h, p1.v - p2.v};
}
static inline AIRealPoint pAdd(const AIRealPoint &p1, const AIRealPoint &p2){
    return AIRealPoint{ p1.h + p2.h, p1.v + p2.v};
}
// ----
static void ensurePoint(AIRealPoint** pointPtr, AIToolMessage* message) {
    if (*pointPtr == nullptr) {
        *pointPtr = new AIRealPoint(message->cursor);
    }
}
static void setPoint(AIRealPoint** pointPtr, AIToolMessage* message) {
    if (*pointPtr == nullptr) {
        *pointPtr = new AIRealPoint(message->cursor);
    }
    else {
        (*pointPtr)->h = message->cursor.h;
        (*pointPtr)->v = message->cursor.v;
    }
}// ----

AIErr anchorGrabberPlugin::resetStatus() {
    AIErr error = kNoErr;
    try {
        gparms.mouseDowned = false;
        
        delete gparms.curPointPtr;
        gparms.curPointPtr = nullptr;
        
        gparms.selectedArtSpecs.clear();
        std::vector<SelectedArtSpec>().swap(gparms.selectedArtSpecs);
    }
    catch (ai::Error& ex) {
        error = ex;
    }
    catch (...)
    {
        error = kCantHappenErr;
    }
    return error;
}
// ----
ASErr anchorGrabberPlugin::selectSegmentsNearThePoint(AIArtHandle &art, const AIRealPoint &point,
                                                   AIReal tolerance){
    ASErr error = kNoErr;
    AIPathSegment *segs = nullptr;
    try {  // needed to delete segs[]
        ai::int16 segcount = 0;
        
        AIRealPoint downpoint = *(gparms.curPointPtr);

        SelectedArtSpec spec;
        spec.art = art;
        
        AIPoint vpoint;
        error = sAIDocumentView->ArtworkPointToViewPoint(nullptr, &point, &vpoint); CHKERR;
    
        error = sAIPath->GetPathSegmentCount(art, &segcount); CHKERR;
        segs = new AIPathSegment[segcount];
        error = sAIPath->GetPathSegments(art, 0, segcount, segs); CHKERR;
        
        AIPoint vp;
        for(ai::int16 i = 0; i < segcount; i++){
            error = sAIDocumentView->ArtworkPointToViewPoint(nullptr, &(segs[i].p), &vp); CHKERR;
            if(abs(vpoint.h - vp.h) <= tolerance && abs(vpoint.v - vp.v) <= tolerance){
                sAIPath->SetPathSegmentSelected(art, i, kSegmentPointSelected);
                spec.segSpecs.push_back(SegmentSpec{i, pSub(segs[i].p, downpoint)});
            }
        }
        if(spec.segSpecs.size() > 0){
            gparms.selectedArtSpecs.push_back(spec);
        }
    }
    catch (ai::Error& ex) {
        error = ex;
    }
    catch (...)
    {
        error = kCantHappenErr;
    }
    if(segs) delete[] segs;
    return error;
}
// ----
// throw exception if it needs. must call from inside try-catch
ASErr anchorGrabberPlugin::CheckForPointHit(AIToolMessage* message, bool is_once, const AIReal tolerance,
    bool &is_hit)
{
    ASErr error = kNoErr;
    std::vector<AIArtHandle> hitArts;
    std::vector<AIArtHandle> noHitArts;
    int loopcount = 0;
    
    ensurePoint(&(gparms.curPointPtr), message);
    AIRealPoint cpoint = *(gparms.curPointPtr);
    AIPoint vpoint;
    error = sAIDocumentView->ArtworkPointToViewPoint(nullptr, &cpoint, &vpoint); CHKERR;
    is_hit = false;
    
    if(!is_once){
        if (sAIMatchingArt->IsSomeArtSelected) {
            error = sAIMatchingArt->DeselectAll(); CHKERR;
        }
    }
    
    AIHitRef hitRef = NULL;
    AIToolHitData toolHitData;
    ai::int32 hitTestOption = kPHitRequest;
    while(true){
        loopcount++;
        if(loopcount > 100) break;
        
        hitRef = NULL;
        error = sAIHitTest->HitTestEx(NULL, &cpoint, tolerance, hitTestOption, &hitRef); CHKERR;
        error = sAIHitTest->GetHitData(hitRef, &toolHitData); CHKERR;
        error = sAIHitTest->Release(hitRef); CHKERR;
        
        if (toolHitData.hit && toolHitData.object != NULL) {
            AIPoint vp;
            error = sAIDocumentView->ArtworkPointToViewPoint(nullptr, &(toolHitData.point), &vp); CHKERR;
            if(abs(vpoint.h - vp.h) > tolerance || abs(vpoint.v - vp.v) > tolerance){
                noHitArts.push_back(toolHitData.object);
                error = sAIArt->SetArtUserAttr(toolHitData.object, kArtLocked, kArtLocked);
                continue;
            }
            if(is_once){
                is_hit = true;
                break;
            }
            hitArts.push_back(toolHitData.object);
            error = sAIArt->SetArtUserAttr(toolHitData.object, kArtLocked, kArtLocked);
        } else {
            break;
        }
    }
    if(noHitArts.size() > 0){
        for(auto itr = noHitArts.begin(); itr != noHitArts.end(); ++itr) {
            error = sAIArt->SetArtUserAttr(*itr, kArtLocked, 0); CHKERR;
         }
    }
    if(hitArts.size() > 0){
        for(auto itr = hitArts.begin(); itr != hitArts.end(); ++itr) {
            error = sAIArt->SetArtUserAttr(*itr, kArtLocked, 0); CHKERR;
            error = selectSegmentsNearThePoint(*itr, cpoint, tolerance); CHKERR;
         }
    }

    return error;
}
// ----
// click: select
// drag: move
// shift+drag: move vertical or horizontal
AIErr anchorGrabberPlugin::ToolMouseDown( AIToolMessage* message )
{
    AIErr error = kNoErr;
    try {
        error = resetStatus();
        if (!error) {
            bool is_hit = false;  // not used
            setPoint(&(gparms.curPointPtr), message);
            
            /*AIRealPoint cpoint = *(gparms.curPointPtr);
            AIPoint vpoint;
            error = sAIDocumentView->ArtworkPointToViewPoint(nullptr, &cpoint, &vpoint); CHKERR;*/
            
            error = CheckForPointHit(message, false, gparms.tolerance_drag, is_hit);
        }
        if (!error) {
            gparms.mouseDowned = true;
            gparms.safeClick = true;
        }
    }
    catch (ai::Error& ex) {
        error = ex;
    }
    return error;
}
// ----
ASErr anchorGrabberPlugin::ToolMouseDrag(AIToolMessage * message){
    ASErr error = kNoErr;
    try {
        if(!(gparms.mouseDowned)) return error;
        
        AIRealPoint cur = message->cursor;
        ensurePoint(&(gparms.curPointPtr), message);
        AIRealPoint dpoint = *(gparms.curPointPtr);
        
        if(gparms.safeClick && gparms.tolerance_safe_click > 0){
            AIPoint vpoint, vp;
            error =sAIDocumentView->ArtworkPointToViewPoint(nullptr, &cur, &vpoint); CHKERR;
            error = sAIDocumentView->ArtworkPointToViewPoint(nullptr, &dpoint, &vp); CHKERR;
            if(abs(vpoint.h - vp.h) <= gparms.tolerance_safe_click
               && abs(vpoint.v - vp.v) <= gparms.tolerance_safe_click){
                return error;
            }
        }
        if(gparms.safeClick) gparms.safeClick = false;
        // shiftkey -> vertical or horizontal
        if(message->event->modifiers & aiEventModifiers_shiftKey){
            AIRealPoint cursorDelta = pSub(cur, dpoint);
            if(abs(cursorDelta.h) < abs(cursorDelta.v)){
                cur.h = dpoint.h;
            } else {
                cur.v = dpoint.v;
            }
        }
        
        AIPathSegment segs[1];
        if(gparms.selectedArtSpecs.size() > 0){
            SelectedArtSpec spec;
            SegmentSpec segspec;
            AIRealPoint curmod;  // cur as SegmentSpec.delta applied
            AIRealPoint delta;  // curmod - anchor coord
            for(auto itr = gparms.selectedArtSpecs.begin(); itr != gparms.selectedArtSpecs.end(); ++itr) {
                spec = *itr;
                for(auto segitr = spec.segSpecs.begin(); segitr != spec.segSpecs.end(); ++segitr){
                    segspec = *segitr;
                    error = sAIPath->GetPathSegments(spec.art, segspec.idx, 1, segs); CHKERR;
                    curmod = pAdd(cur, segspec.delta);
                    delta = pSub(curmod, segs[0].p);
                    segs[0].p = curmod;
                    segs[0].in = pAdd(segs[0].in, delta);
                    segs[0].out = pAdd(segs[0].out, delta);
                    error = sAIPath->SetPathSegments(spec.art, segspec.idx, 1, segs); CHKERR;
                }
            }
        }
    }
    catch (ai::Error& ex) {
        error = ex;
    }
    catch (...)
    {
        error = kCantHappenErr;
    }
    return error;
}
// ----
ASErr anchorGrabberPlugin::ToolMouseUp(AIToolMessage * message){
    return resetStatus();
}
// ----
ASErr anchorGrabberPlugin::TrackToolCursor(AIToolMessage* message){
    ASErr error = kNoErr;
    try {
        bool is_hit = false;
        setPoint(&(gparms.curPointPtr), message);
        error = CheckForPointHit(message, true, gparms.tolerance_drag, is_hit); CHKERR;
        
        if(sAIUser != NULL){
            error = sAIUser->SetSVGCursor(
                                          is_hit ? kanchorGrabberToolHitCursorResourceID : kanchorGrabberToolCursorResourceID,
                                          fResourceManagerHandle); CHKERR;
         }
    }
    catch (ai::Error& ex) {
        error = ex;
    }
    catch(...)
    {
        error = kCantHappenErr;
    }
    return error;
    
}

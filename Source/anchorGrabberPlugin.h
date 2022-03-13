#ifndef __anchorGrabberPlugin_h__
#define __anchorGrabberPlugin_h__

#include "Plugin.hpp"
#include "anchorGrabberID.h"
#include "MyDialogParms.h"

struct SegmentSpec {
    ai::int16 idx;
    AIRealPoint delta;
};

struct SelectedArtSpec {
    AIArtHandle art;
    std::vector<SegmentSpec> segSpecs;
};

struct MyParms {
    AIReal tolerance_drag;
    AIReal tolerance_safe_click;
    AIRealPoint* curPointPtr;
    bool mouseDowned;
    bool safeClick;
    std::vector<SelectedArtSpec> selectedArtSpecs;
};

Plugin* AllocatePlugin(SPPluginRef pluginRef);

void FixupReload(Plugin* plugin);

class anchorGrabberPlugin : public Plugin
{
public:
	anchorGrabberPlugin(SPPluginRef pluginRef);
	virtual ~anchorGrabberPlugin();
	FIXUP_VTABLE_EX(anchorGrabberPlugin, Plugin);
	ASErr StartupPlugin( SPInterfaceMessage * message );
	ASErr ShutdownPlugin( SPInterfaceMessage * message );
    ASErr PostStartupPlugin();
    ASErr SelectTool( AIToolMessage* message );
    ASErr EditTool(AIToolMessage* message );
    ASErr ToolMouseDown( AIToolMessage* message );
    ASErr ToolMouseDrag(AIToolMessage * message);
    ASErr ToolMouseUp(AIToolMessage * message);
    ASErr TrackToolCursor(AIToolMessage* message);
    ASErr AddNotifier(SPInterfaceMessage *message);
    ASErr Notify(AINotifierMessage* message);
    void callbackFunc();

private:
    MyParms gparms;
    MyDialogParms tmpParms;
    AIToolHandle fToolHandle;
    AIResourceManagerHandle fResourceManagerHandle;
    AINotifierHandle fShutdownApplicationNotifier;
    
    AIErr AddTool(SPInterfaceMessage* message);
    ASErr CheckForPointHit(AIToolMessage* message, bool is_once, const AIReal tolerance, bool &is_hit);
    ASErr selectSegmentsNearThePoint(AIArtHandle &art, const AIRealPoint &point, AIReal tolerance);
    AIErr resetStatus();
};

#endif

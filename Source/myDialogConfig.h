#ifndef MyDialogConfig_h
#define MyDialogConfig_h

static const float kMyDialogWidth = 320.0f;

#ifdef __APPLE__
static const float kMyDialogHeight = 120.0f;
#else
static const float kMyDialogHeight = 200.0f;
#endif

static const char* kMyDialogTitle = "AnchorGrabber";

#ifndef kMaxPathSegments
#define kMaxPathSegments   32000
#endif

#endif /* MyDialogConfig_h */

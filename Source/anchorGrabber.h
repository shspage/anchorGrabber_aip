#ifndef __anchorGrabber_h__
#define __anchorGrabber_h__

#ifndef CHKERR
#define CHKERR aisdk::check_ai_error(error)
#endif

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
    //AIRect annotateRect;
    std::vector<SelectedArtSpec> selectedArtSpecs;
};

#endif

#ifndef MyDialogParms_h
#define MyDialogParms_h

/* ダイアログの表示を日本語にする場合、以下をアンコメントしてください。
 If you want the dialog to be displayed in Japanese, please uncomment the following. */
// #define DIALOG_LANG_JP

struct MyDialogParms {
    int tolerance_drag;
    int tolerance_safe_click;
};

static const double TOLERANCE_DRAG = 4.0;
static const double TOLERANCE_SAFE_CLICK = 2.0;

static const float MIN_HIT_TOLE = 1;
static const float MAX_HIT_TOLE = 10;
static const float MIN_SAFE_CLICK = 0;
static const float MAX_SAFE_CLICK = 10;

#ifdef DIALOG_LANG_JP
static const char* LABEL_TITLE = "* ピクセル距離設定";
static const char* LABEL_TOLERANCE = "アンカーを捕らえる範囲 (%d-%d)";
static const char* LABEL_SAFECLICK = "移動を無視する距離 (%d-%d)";
static const char* LABEL_CANCEL = "キャンセル";
static const char* LABEL_OK = "  OK  ";
#ifdef __APPLE__
static const char* FONT_NAME_JP = "/System/Library/Fonts/ヒラギノ丸ゴ ProN W4.ttc";
static const float FONT_SIZE_JP = 12.0f;
#else
static const char* FONT_NAME_JP = "C:\\windows\\fonts\\meiryo.ttc";
static const float FONT_SIZE_JP = 18.0f;
#endif
#else
static const char* LABEL_TITLE = "* pixel distance settings";
static const char* LABEL_TOLERANCE = "to grab anchors (%d-%d)";
static const char* LABEL_SAFECLICK = "to ignore move (%d-%d)";
static const char* LABEL_CANCEL = "Cancel";
static const char* LABEL_OK = "  OK  ";
#endif

#endif /* MyDialogParms_h */

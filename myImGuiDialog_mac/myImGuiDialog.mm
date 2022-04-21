
#import <Cocoa/Cocoa.h>
#include "../Source/myImGuiDialog.h"
#include "../Source/myDialogConfig.h"
#import "MyWindowController.h"

// --------
@interface MyObjCBase : NSObject
+ (int) runModal:(MyDialogParms*)parms completion:(std::function<void(void)>)callbackFunc;
@end

@implementation MyObjCBase
+ (int) runModal:(MyDialogParms*)parms completion:(std::function<void(void)>)callbackFunc
{
    int result = 0;
    
    NSRect frame = NSMakeRect(0, 0, kMyDialogWidth, kMyDialogHeight);
    
    //append "| NSWindowStyleMaskResizable" if it needs
    NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                                         styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskClosable
                                         backing:NSBackingStoreBuffered
                                         defer:YES];
    MyWindowController* dialog = [[MyWindowController alloc] initWithWindow:window];

    if(dialog){
        result = [dialog runModal:parms completion:callbackFunc];
        [dialog releaseDialog];
        dialog = nil;
    }
    return result;
}
@end

// ----------
int myImGuiDialog::runModal(MyDialogParms* parms, std::function<void(void)> callbackFunc)
{
    return [MyObjCBase runModal:parms completion:callbackFunc];
}

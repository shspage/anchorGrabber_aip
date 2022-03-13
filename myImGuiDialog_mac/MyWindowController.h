
#ifndef MyWindowController_h
#define MyWindowController_h

#import <Cocoa/Cocoa.h>
#include <functional>
#include "../Source/MyDialogParms.h"

@interface MyWindowController : NSWindowController{
}
- (instancetype)initWithWindow:(NSWindow*)window;
- (int) runModal:(MyDialogParms*)parms completion:(std::function<void(void)>)callbackFunc;
- (void) releaseDialog;
@end

#endif /* MyWindowController_h */

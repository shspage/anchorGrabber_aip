/*
 console app for testing
 */

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <functional>
#include "../Source/myImGuiDialog.h"
#include "../Source/MyDialogParms.h"

class MyClass
{
public:
    MyClass() : parms({1, 0}){};
    void callbackFunc();
    MyDialogParms* getParms(){ return &parms; }
private:
    MyDialogParms parms;
};

void MyClass::callbackFunc(){
    std::cout << "drag/track=" << parms.tolerance_drag\
        << "/safe click=" << parms.tolerance_safe_click << std::endl;
}

int main(int argc, const char * argv[]) {
    MyClass* cl = new MyClass();
    std::function<void(void)> cbfunc = std::bind(&MyClass::callbackFunc, cl);
    
    myImGuiDialog::runModal(cl->getParms(), cbfunc);
    
    std::cout << "end" << std::endl;
    return 0;
}

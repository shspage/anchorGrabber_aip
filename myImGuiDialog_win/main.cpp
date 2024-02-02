/*
 myImGuiDialog standalone dialog app for testing.
 To build, run build_win32.bat on your copy of vcvars32.bat or vcvarsall.bat.
*/

#include <stdio.h>
#include <iostream>
#include <functional>
#include "../Source/myImGuiDialog.h"
#include "../Source/MyDialogParms.h"

class MyClass
{
public:
	MyClass() : parms({ 1, 0 }) {};
	void callbackFunc();
	MyDialogParms* getParms() { return &parms; }
private:
	MyDialogParms parms;
};

void MyClass::callbackFunc() {
	std::cout << "drag/track=" << parms.tolerance_drag\
		<< "/safe click=" << parms.tolerance_safe_click << std::endl;
}

int main(int argc, const char * argv[]) {
	MyClass* cl = new MyClass();
	std::function<void(void)> cbfunc = std::bind(&MyClass::callbackFunc, cl);

	HWND hwnd = NULL;
	int result = myImGuiDialog::runModal(hwnd, cl->getParms(), cbfunc);

	std::cout << "result=" << result << std::endl;
	cbfunc();

	return 0;
}

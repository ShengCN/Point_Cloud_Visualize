#include <iostream>
#include "pcv_window.h"
#include "Global_Variables.h"

int main(void){	
	auto gv = Global_Variables::Instance();

	pcv_window wnd;
	wnd.create_window(gv->width, gv->height, gv->title);
	wnd.show();

	return 0;
}
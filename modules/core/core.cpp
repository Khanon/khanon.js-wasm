// core.cpp : Defines the entry point for the application.
//

#include <emscripten/emscripten.h>
#include "core.h"

using namespace std;

int main()
{
	printf("hola?");
	//cout << "Hello CMake." << endl;
	return 0;
}

EMSCRIPTEN_KEEPALIVE
int add() {
	emscripten_hide_mouse();
	return 69;
}

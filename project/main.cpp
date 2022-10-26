/******************************************************************************/
/*!
\file   main.cpp
\author David Ly
\par    email: dly\@digipen.edu
\par    GAM150 demo
\par	v0
\date   2018/03/11
*/
/******************************************************************************/
#include "SDL2/SDL.h"
#include <cassert>
#include <iostream>
#include <string>
#include "Application.h"


int main(int /*argc*/, char* /*argv*/ [])
{
    //////////////////////////This is the ONLY one you should declare an
    Application application;//application of your program! Do not declare
    //////////////////////////another one!!!!

	application.CreateDebugConsole();	// only in DEBUG mode

	if (!application.CanStartUp())
        return -1;
	while (!application.IsDone())
		application.Update();

	application.DeleteDebugConsole();
    return 0;
}

void CheckSDLError(int line)
{
    std::string error = SDL_GetError();
    if (error.empty())
        return;
    std::cerr << "SLD Error : " << error << '\n';
    if (line != -1)
        std::cerr << "\nLine : " << line << '\n';
    SDL_ClearError();
    assert(false);
}

/******************************************************************************/
/*!
\file   Debug.h
\author Juyong Jeong
\par    email: jeykop14\@gmail.com
\date   2017/04/02(yy/mm/dd)

\description
Contains Debug macros in debug mode

*****YOU CANNOT MODIFY THIS FILE!!*****
*/
/******************************************************************************/
#pragma once

namespace DebugTools
{
	//These functios should NOT be called
	bool Assert(int _expression, const char* _outputMessage,
		const char* _functionName, const char* _fileName,
		unsigned _lineNumber);

	void CreateConsole(void);
	void DestroyConsole(void);
	void ClearScreen(void);

	//This should be called for IMPORTANT MESSAGES to the user
	void CustomizedMessageBox(const char* _outputMessage);
}

//Macro for debug only
#if defined (_DEBUG)

//Use this macro instead of the function to ASSERT in debug only
#define DEBUG_ASSERT(exp, srt) if(DebugTools::Assert((exp), (srt),	\
	__FUNCTION__, __FILE__, __LINE__))								\
{																	\
	DebugBreak();													\
}
//Use this macro to create a console in debug only
#define DEBUG_CREATE_CONSOLE() DebugTools::CreateConsole()
//Use this macro to destroy a console in debug only
#define DEBUG_DESTROY_CONSOLE() DebugTools::DestroyConsole();
//Use this amscro to print to the console in debug only
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
//Use this macro instead of the funciton to clea the console in debug only
#define DEBUG_CLEAR() DebugTools::ClearScreen()
//If you have a leak, there is a number in curly braces next to the error,
//Put that numver in this function and check the call stack to see when and
//where the allocation happened. Set it to -1 to have it not break
#define DEBUG_LEAK_CHECKS(x)										\
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	\
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);				\
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);				\
	_CrtSetBreakAlloc((x));

#define DEBUG_CALL_CHECK()			\
	static int functionCounter = 0; \
	++functionCounter;				\
	DEBUG_ASSERT(functionCounter == 1, "This function should be called only once");

#else
//Debug Macros do nothing in release mode
//Use this macro instead of the function to ASSERT in debug only
#define DEBUG_ASSERT(expression, outputMessage)
#define DEBUG_CREATE_CONSOLE()
#define DEBUG_DESTROY_CONSOLE()
#define DEBUG_PRINT(...)
#define DEBUG_CLEAR()
#define DEBUG_LEAK_CHECKS(x)
#define DEBUG_CALL_CHECK()

#endif
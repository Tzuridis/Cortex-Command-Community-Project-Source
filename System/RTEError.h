#ifndef _RTEERROR_
#define _RTEERROR_

#include "allegro.h"
#include "loadpng.h"

#ifdef _WIN32
#define DEBUGGERBREAK __debugbreak();
#else
#define DEBUGGERBREAK ;
#endif

namespace RTE {

static BITMAP *g_ScreendumpBuffer= 0; //!< Buffer for saving abort screendumps.

/// <summary>
/// Pops up a message box dialog in the OS. For debug purposes mostly.
/// </summary>
/// <param name="message">The string that the message box should display.</param>
extern void ShowMessageBox(std::string message);

/// <summary>
/// Abort on Error function. Will try to dump a screenshot, show an abort message, and then quit the program immediately.
/// </summary>
/// <param name="description">The description of the abortion.</param>
/// <param name="file">The source file in which the abortion is made.</param>
/// <param name="line">The line where the abortion is made.</param>
extern bool RTEAbortFunc(const char *description, const char *file, int line);
extern bool RTEAbortFunc(const std::string description, const char *file, int line);
#define RTEAbort(description)                            \
	{                                                      \
		if (RTEAbortFunc(description, __FILE__, __LINE__)) { \
			DEBUGGERBREAK                                      \
		}                                                    \
	}
/// <summary>
/// A souped-up, customized assert function that brings up a nice dialog box on assert failure.
/// The user can choose to break or ignore the particular assertion failure once, or to always ignore.
/// </summary>
/// <param name="expression">The expression that will be asserted to be true.</param>
/// <param name="description">The description of the assertion.</param>
/// <param name="file">The source file in which the assertion is made.</param>
/// <param name="line">The line where the assertion is made.</param>
/// <param name="alwaysIgnore">A reference to a bool that is used in an "Always ignore" functionality.</param>
/// <returns>Whether the assertion failed AND the user chose to break in the dialog box.</returns>
extern bool RTEAssertFunc(bool expression, const char *description, const char *file, int line, bool &alwaysIgnore);
extern bool RTEAssertFunc(bool expression, const std::string description, const char *file, int line, bool &alwaysIgnore);
#define RTEAssert(expression, description)                                            \
	{                                                                                   \
		static bool alwaysIgnore= false;                                                  \
		if (!alwaysIgnore) {                                                              \
			if (RTEAssertFunc(expression, description, __FILE__, __LINE__, alwaysIgnore)) { \
				DEBUGGERBREAK                                                                 \
			}                                                                               \
		}                                                                                 \
	}
} // namespace RTE
#endif

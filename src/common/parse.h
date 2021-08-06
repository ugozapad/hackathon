#ifndef PARSE_H
#define PARSE_H

namespace engine
{

/*
=====================================================================================
SCRIPT PARSING
=====================================================================================
*/

// this just controls the comment printing, it doesn't actually load a file
	void Com_BeginParseSession(const char* filename);
	void Com_EndParseSession(void);

	int Com_GetCurrentParseLine(void);

	// Will never return NULL, just empty strings.
	// An empty string will only be returned at end of file.
	// ParseOnLine will return empty if there isn't another token on this line

	// this funny typedef just means a moving pointer into a const char * buffer
	const char* Com_Parse(const char* (*data_p));
	const char* Com_ParseOnLine(const char* (*data_p));
	const char* Com_ParseRestOfLine(const char* (*data_p));

	void Com_UngetToken(void);

	void Com_MatchToken(const char* (*buf_p), const char* match, bool warning = false);

	void Com_ScriptError(const char* msg, ...);
	void Com_ScriptWarning(const char* msg, ...);

	void Com_SkipBracedSection(const char* (*program));
	void Com_SkipRestOfLine(const char* (*data));

	float Com_ParseFloat(const char* (*buf_p));
	int	Com_ParseInt(const char* (*buf_p));

	void Com_Parse1DMatrix(const char* (*buf_p), int x, float* m);
	void Com_Parse2DMatrix(const char* (*buf_p), int y, int x, float* m);
	void Com_Parse3DMatrix(const char* (*buf_p), int z, int y, int x, float* m);
}

#endif // !PARSE_H

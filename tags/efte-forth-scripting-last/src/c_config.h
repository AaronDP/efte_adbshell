/*    c_config.h
 *
 *    Copyright (c) 2008, eFTE SF Group (see AUTHORS file)
 *    Copyright (c) 1994-1996, Marko Macek
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "sysdep.h"
#include "u_stack.h"

typedef struct _CurPos {
    unsigned int sz;
    char *a;
    char *c;
    char *z;
    int line;
    const char *name; // filename
} CurPos;

extern int CFteMain();

typedef struct {
    unsigned char tag;
    unsigned short len;
    void *obj;
} CachedObject;

// Cached objects
#define CACHE_SIZE 512000
extern CachedObject cache[CACHE_SIZE];
extern unsigned int cpos;

// these are indexes to shared variables
extern unsigned int verbosity;
extern unsigned int mousex;
extern unsigned int mousey;
extern unsigned int mousexrelative;
extern unsigned int mouseyrelative;
extern unsigned int mousebutton;
extern unsigned int mousewinsizex;
extern unsigned int mousewinsizey;
extern unsigned int mouseeventcounter;
extern unsigned int mouseeventtype;


// commands may flag branch conditions in here
// by shifting left, and setting (or not) bit 0
extern unsigned int BranchCondition;
extern CircularStack ParamStack;
extern CircularStack ControlStack;

extern int ScreenSizeX;
extern int ScreenSizeY;
extern bool CursorBlink;
extern bool CursorWithinEOL;
extern int CursorInsertMask;
extern int CursorOverMask;
extern int SystemClipboard;
extern int OpenAfterClose;
extern int ShowMenuBar;
extern int ShowToolBar;
extern int SelectPathname;
extern char DefaultModeName[32];
extern int WSStyleSearch;
extern char PrintDevice[MAXPATH];
extern char CompileCommand[256];
extern int KeepHistory;
extern int LoadDesktopOnEntry;
extern int SaveDesktopOnExit;
extern int KeepMessages;
extern int ScrollBorderX;
extern int ScrollBorderY;
extern int ScrollJumpX;
extern int ScrollJumpY;
extern int GUIDialogs;
extern int SevenBit;
extern int WeirdScroll;
extern int LoadDesktopMode;
extern char HelpCommand[128];
extern char *ConfigSourcePath;
extern char ConfigFileName[MAXPATH];
extern int IgnoreBufferList;
extern char CvsCommand[256];
extern char CvsLogMode[32];
extern char SvnCommand[256];
extern char SvnLogMode[32];
extern int ReassignModelIds;
extern int RecheckReadOnly;
extern char XShellCommand[256];
extern char BackupDirectory[MAXPATH];

const char *GetGUICharacters(const char *which, const char *defChars);
int LoadConfig(int argc, char **argv, char *CfgFileName);
int LoadDefaultConfig();
int GetIndentMode(const char *Str);
int GetHilitMode(const char *Str);
int AddCRegexp(int file, int line, int msg, const char *regexp);
int LoadFile(char *WhereName, char *CfgName);
int ProcessConfigFile(char *filename, char *buffer, int Level);
int ReadConfigFile(CurPos &cp);

#endif
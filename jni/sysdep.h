/*    sysdep.h
 *
 *    Copyright (c) 2008, eFTE SF Group (see AUTHORS file)
 *    Copyright (c) 1994-1996, Marko Macek
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 */

#ifndef SYSDEP_H_
#define SYSDEP_H_

/* Support big files on 32-bit platform, if possible. */
#define _FILE_OFFSET_BITS 64

#ifndef _LARGEFILE_SOURCE
# define _LARGEFILE_SOURCE
#endif // ifndef _LARGEFILE_SOURCE

#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#if defined(AIX) || defined(SCO) || defined(NCR)
# include <strings.h>
#endif // if defined(AIX) || defined(SCO) || defined(NCR)
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#ifdef DBMALLOC
# include <malloc.h>
#endif // ifdef DBMALLOC

#if defined(UNIX)
# define USE_DIRENT
#endif // if defined(UNIX)

#if defined(USE_DIRENT) // also needs fnmatch
# include <dirent.h>
#endif // if defined(USE_DIRENT)

#if defined(UNIX)
# include <unistd.h>
# include <pwd.h>
# if defined(__CYGWIN__)
#  include "fnmatch.h"
# else // if defined(__CYGWIN__)
#  include <fnmatch.h>
# endif // if defined(__CYGWIN__)
# define strnicmp strncasecmp
# define stricmp strcasecmp
# define filecmp strcmp

// #    define memicmp strncasecmp   // FIX, fails for nulls
extern "C" int memicmp(const void *s1,
                       const void *s2,
                       size_t      n);
#endif // if defined(UNIX)

#if defined(OS2)
# include <malloc.h>
# if !defined(__TOS_OS2__)
#  include <dos.h>
# endif // if !defined(__TOS_OS2__)
# include <io.h>
# include <process.h>
# if defined(BCPP) || defined(WATCOM) || defined(__TOS_OS2__)
#  include <direct.h>
# endif // if defined(BCPP) || defined(WATCOM) || defined(__TOS_OS2__)
# if defined(BCPP)
#  include <dir.h>
# endif // if defined(BCPP)
# define filecmp stricmp
# if !defined(__EMX__)
#  define NO_NEW_CPP_FEATURES
# endif // if !defined(__EMX__)
#endif // if defined(OS2)

#if defined(NT)
# include <malloc.h>
# include <dos.h>
# include <io.h>
# include <process.h>
# if defined(MSVC)
#  include <direct.h>
#  define HAVE_STRICMP
#  define snprintf _snprintf
#  define isdigit(x) isdigit((int)(unsigned char)(x))
#  define isalpha(x) isalpha((int)(unsigned char)(x))
#  define isalnum(x) isalnum((int)(unsigned char)(x))
#  define islower(x) islower((int)(unsigned char)(x))
#  define isupper(x) isupper((int)(unsigned char)(x))
#  define isprint(x) isprint((int)(unsigned char)(x))
#  define ispunct(x) ispunct((int)(unsigned char)(x))
#  define isspace(x) isspace((int)(unsigned char)(x))
#  define isgraph(x) isgraph((int)(unsigned char)(x))
# endif // if defined(MSVC)
# if defined(WATCOM) || defined(__WATCOM_CPLUSPLUS__)
#  define HAVE_STRLCPY
#  define HAVE_STRLCAT
#  define HAVE_STRICMP
#  include <direct.h>
# endif // if defined(WATCOM) || defined(__WATCOM_CPLUSPLUS__)
# if defined(BCPP)
#  include <dir.h>
#  define HAVE_STRICMP
# endif // if defined(BCPP)
# if defined(MINGW)
#  include <dir.h>
#  define HAVE_BOOL // older versions of MingW may not have it
# endif // if defined(MINGW)
# define filecmp stricmp
# define popen _popen
# define pclose _pclose
#endif // if defined(NT)

#ifndef MAXPATH
# define MAXPATH 1024
#endif // ifndef MAXPATH

#ifndef O_BINARY
# define O_BINARY 0 /* defined on OS/2, no difference on unix */
#endif // ifndef O_BINARY

#if defined(OS2) || defined(NT)
# if defined(__EMX__) || defined(WATCOM) || defined(__TOS_OS2__)
#  define FAKE_BEGINTHREAD_NULL NULL,
# else // if defined(__EMX__) || defined(WATCOM) || defined(__TOS_OS2__)
#  define FAKE_BEGINTHREAD_NULL
# endif // if defined(__EMX__) || defined(WATCOM) || defined(__TOS_OS2__)
#endif // if defined(OS2) || defined(NT)

#if (!defined(__IBMC__) && !defined(__IBMCPP__)) || !defined(OS2)
# define _LNK_CONV
#endif // if (!defined(__IBMC__) && !defined(__IBMCPP__)) || !defined(OS2)

#define PT_UNIXISH   0
#define PT_DOSISH    1

#ifndef S_ISDIR // NT, DOS, DOSP32
# ifdef S_IFDIR
#  define S_ISDIR(mode) ((mode) & S_IFDIR)
# else // ifdef S_IFDIR
#  define S_ISDIR(mode) ((mode) & _S_IFDIR)
# endif // ifdef S_IFDIR
#endif // ifndef S_ISDIR

#ifndef S_IWGRP
# define S_IWGRP 0
# define S_IWOTH 0
#endif // ifndef S_IWGRP

#if defined(OS2) || defined(NT)
# define PATHTYPE   PT_DOSISH
#else // if defined(OS2) || defined(NT)
# define PATHTYPE   PT_UNIXISH
#endif // if defined(OS2) || defined(NT)

#if defined __cplusplus && __cplusplus >= 199707L
# define HAVE_BOOL
#endif // if defined __cplusplus && __cplusplus >= 199707L

#if defined __GNUC__ && (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7))
# define HAVE_BOOL
#endif // if defined __GNUC__ && (__GNUC__ > 2 || (__GNUC__ == 2 &&
       // __GNUC_MINOR__ >= 7))

#if defined _G_HAVE_BOOL
# define HAVE_BOOL
#endif // if defined _G_HAVE_BOOL

#if defined __BORLANDC__ && __BORLANDC__ >= 0x0500
# define HAVE_BOOL
#endif // if defined __BORLANDC__ && __BORLANDC__ >= 0x0500

#if defined __BORLANDC__ && defined __OS2__
# define popen _popen
# define pclose _pclose
# define ftruncate _ftruncate
#endif // if defined __BORLANDC__ && defined __OS2__

// #undef HAVE_STRLCPY
// #undef HAVE_STRLCAT

#endif // __SYSDEP_H

/*    s_util.cpp
 *
 *    Copyright (c) 2008, eFTE SF Group (see AUTHORS file)
 *    Copyright (c) 1994-1996, Marko Macek
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 */

#include "fte.h"

#define BUF_SZ (sizeof(FileBuffer))

extern RxNode *CompletionFilter;

int MacroNum(const char *Name) {
    for (int i = 0; i < CMacros; i++)
        if (Macros[i].Name && (strcmp(Name, Macros[i].Name)) == 0)
            return i;
    return 0; // Nop
}

// should use DosCopy under OS/2...
static int copyfile(char *f1, char *f2) { // from F1 to F2
    void *buffer;
    int fd1, fd2;
    int rd;

    buffer = FileBuffer;

    if ((fd1 = open(f1, O_RDONLY | O_BINARY)) == -1)
        return -1;
    if ((fd2 = open(f2, O_WRONLY | O_BINARY | O_CREAT | O_TRUNC, 0666)) == -1) {
        close(fd1);
        return -1;
    }
    while ((rd = read(fd1, buffer, BUF_SZ)) > 0) {
        if (write(fd2, buffer, rd) != rd) {
            close(fd1);
            close(fd2);
            unlink(f2);
            return -1;
        }
    }
    close(fd2);
    close(fd1);
    return 0;
}

char *MakeBackup(char *FileName, char *NewName) {
//    static char NewName[260];
    int l = strlen(FileName);
    if (l <= 0)
        return NULL;

    /* try 1 */
    if (strlen(BackupDirectory) > 0) {
        char TmpFileName[MAXPATH];
        char TmpBackupName[MAXPATH];

        strcpy(TmpFileName, FileName);

        for (unsigned int idx=0; idx < strlen(TmpFileName); idx++)
            if (TmpFileName[idx] == '/' || TmpFileName[idx] == '\\' || TmpFileName[idx] == ':')
                TmpFileName[idx] = '_';
        snprintf(TmpBackupName, MAXPATH, "%s/%s", BackupDirectory, TmpFileName);
        ExpandPath(TmpBackupName, NewName, MAXPATH);
    } else
        snprintf(NewName, MAXPATH, "%s~", FileName);

    if (!IsSameFile(FileName, NewName)) {
        if (access(NewName, 0) == 0)                 // Backup already exists?
            unlink(NewName);                         // Then delete the file..
        if (access(FileName, 0) != 0)                // Original found?
            return NewName;
        if (copyfile(FileName, NewName) == 0)
            return NewName;
#if 0
        if (errno == 2)
            return NewName; /* file not found */
#endif
    }

    /* try 2: 8.3 */
    strcpy(NewName, FileName);
    NewName[l-1] = '~';
    if (!IsSameFile(FileName, NewName)) {
        if (access(NewName, 0) == 0)                   // Backup exists?
            unlink(NewName);                           // Then delete;
        if (access(FileName, 0) != 0)                  // Original exists?
            return NewName;                            // If not-> return base..
        if (copyfile(FileName, NewName) == 0)
            return NewName;
#if 0
        if (errno == 2)
            return NewName;
#endif
    }

    return NULL;
}

int GetCharFromEvent(TEvent &E, char *Ch) {
    *Ch = 0;
    if (E.Key.Code & kfModifier)
        return 0;
    if (kbCode(E.Key.Code) == kbEsc) {
        *Ch = 27;
        return 1;
    }
    if (kbCode(E.Key.Code) == kbEnter) {
        *Ch = 13;
        return 1;
    }
    if (kbCode(E.Key.Code) == (kbEnter | kfCtrl)) {
        *Ch = 10;
        return 1;
    }
    if (kbCode(E.Key.Code) == kbBackSp) {
        *Ch = 8;
        return 1;
    }
    if (kbCode(E.Key.Code) == (kbBackSp | kfCtrl)) {
        *Ch = 127;
        return 1;
    }
    if (kbCode(E.Key.Code) == kbTab) {
        *Ch = 9;
        return 1;
    }
    if (kbCode(E.Key.Code) == kbDel) {
        *Ch = 127;
        return 1;
    }
    if (keyType(E.Key.Code) == kfCtrl) {
        *Ch = (char)(E.Key.Code & 0x1F);
        return 1;
    }
    if (isAscii(E.Key.Code)) {
        *Ch = (char)E.Key.Code;
        return 1;
    }
    return 0;
}

int CompletePath(const char *Base, char *Match, int Count) {
    char Name[MAXPATH];
    const char *dirp;
    char *namep;
    int len, count = 0;
    char cname[MAXPATH];
    int hascname = 0;
    RxMatchRes RM;
    FileFind *ff;
    FileInfo *fi;
    int rc;

    if (strcmp(Base, "") == 0) {
        if (ExpandPath(".", Name, sizeof(Name)) != 0) return -1;
    } else {
        if (ExpandPath(Base, Name, sizeof(Name)) != 0) return -1;
    }
//    SlashDir(Name);
    dirp = Name;
    namep = SepRChr(Name);
    if (namep == Name) {
        dirp = SSLASH;
        namep = Name + 1;
    } else if (namep == NULL) {
        namep = Name;
        dirp = SDOT;
    } else {
        *namep = 0;
        namep++;
    }

    len = strlen(namep);
    strcpy(Match, dirp);
    SlashDir(Match);
    cname[0] = 0;

    ff = new FileFind(dirp, "*",
                      ffDIRECTORY | ffHIDDEN
#if defined(USE_DIRENT)
                      | ffFAST // for SPEED
#endif
                     );
    if (ff == 0)
        return 0;
    rc = ff->FindFirst(&fi);
    while (rc == 0) {
        const char *dname = fi->Name();

        // filter out unwanted files
        if ((strcmp(dname, ".") != 0) &&
                (strcmp(dname, "..") != 0) &&
                (!CompletionFilter || RxExec(CompletionFilter, dname, strlen(dname), dname, &RM) != 1)) {
            if ((
#if defined(UNIX)
                        strncmp
#else // os2, nt, ...
                        strnicmp
#endif
                        (namep, dname, len) == 0)
                    && (dname[0] != '.' || namep[0] == '.')) {
                count++;
                if (Count == count) {
                    Slash(Match, 1);
                    strcat(Match, dname);
                    if (
#if defined(USE_DIRENT) // for SPEED
                        IsDirectory(Match)
#else
                        fi->Type() == fiDIRECTORY
#endif
                    )
                        Slash(Match, 1);
                } else if (Count == -1) {

                    if (!hascname) {
                        strcpy(cname, dname);
                        hascname = 1;
                    } else {
                        int o = 0;
#ifdef UNIX
                        while (cname[o] && dname[o] && (cname[o] == dname[o])) o++;
#endif
#if defined(OS2) || defined(NT)
                        while (cname[o] && dname[o] && (toupper(cname[o]) == toupper(dname[o]))) o++;
#endif
                        cname[o] = 0;
                    }
                }
            }
        }
        delete fi;
        rc = ff->FindNext(&fi);
    }
    delete ff;
    if (Count == -1) {
        Slash(Match, 1);
        strcat(Match, cname);
        if (count == 1) SlashDir(Match);
    }
    return count;
}


#ifndef HAVE_GETTIMEOFDAY
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS) || defined(__BORLANDC__)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

#ifdef NT
#include <windows.h>
#endif

struct timezone
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

#ifndef OS2

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;

  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    /*converting file time to unix epoch*/
    tmpres /= 10;  /*convert into microseconds*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }

  return 0;
}

#else // Yes, OS/2
#define INCL_DOSERRORS
#define INCL_DOSMISC
#include <os2.h>

struct timeval {
    time_t tv_sec;
    long tv_usec;
};

int gettimeofday(struct timeval *tv, struct timezone *tz) {
    ULONG aulSysInfo[QSV_MAX] = {0};
    APIRET rc = DosQuerySysInfo(1L, QSV_MAX, (PVOID) aulSysInfo, sizeof(ULONG)*QSV_MAX);

    if(rc != NO_ERROR)
        return -1;

    if (NULL != tv) {
        tv->tv_sec = aulSysInfo[QSV_TIME_LOW];
        tv->tv_usec = 1000 * aulSysInfo[QSV_MS_COUNT];
    }

    return 0;
}

#endif
#endif
/*
 * Internal NT APIs and data structures
 *
 * Copyright (C) the Wine project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __WINE_WINTERNL_H
#define __WINE_WINTERNL_H

#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */


/**********************************************************************
 * Fundamental types and data structures
 */

typedef LONG NTSTATUS;

typedef CONST char *PCSZ;

typedef short CSHORT;
typedef CSHORT *PCSHORT;

typedef struct _STRING {
  USHORT Length;
  USHORT MaximumLength;
  PCHAR Buffer;
} STRING, *PSTRING;

typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;
typedef const STRING *PCANSI_STRING;

typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;
typedef const STRING *PCOEM_STRING;

typedef struct _UNICODE_STRING {
  USHORT Length;        /* bytes */
  USHORT MaximumLength; /* bytes */
  PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef const UNICODE_STRING *PCUNICODE_STRING;

#ifndef _FILETIME_
#define _FILETIME_
/* 64 bit number of 100 nanoseconds intervals since January 1, 1601 */
typedef struct _FILETIME
{
#ifdef WORDS_BIGENDIAN
  DWORD  dwHighDateTime;
  DWORD  dwLowDateTime;
#else
  DWORD  dwLowDateTime;
  DWORD  dwHighDateTime;
#endif
} FILETIME, *PFILETIME, *LPFILETIME;
#endif /* _FILETIME_ */

/*
 * RTL_SYSTEM_TIME and RTL_TIME_ZONE_INFORMATION are the same as
 * the SYSTEMTIME and TIME_ZONE_INFORMATION structures defined
 * in winbase.h, however we need to define them seperately so
 * winternl.h doesn't depend on winbase.h.  They are used by
 * RtlQueryTimeZoneInformation and RtlSetTimeZoneInformation.
 * The names are guessed; if anybody knows the real names, let me know.
 */
typedef struct _RTL_SYSTEM_TIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} RTL_SYSTEM_TIME, *PRTL_SYSTEM_TIME;

typedef struct _RTL_TIME_ZONE_INFORMATION {
    LONG Bias;
    WCHAR StandardName[32];
    RTL_SYSTEM_TIME StandardDate;
    LONG StandardBias;
    WCHAR DaylightName[32];
    RTL_SYSTEM_TIME DaylightDate;
    LONG DaylightBias;
} RTL_TIME_ZONE_INFORMATION, *PRTL_TIME_ZONE_INFORMATION;

typedef struct _CLIENT_ID
{
   HANDLE UniqueProcess;
   HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _CURDIR
{
    UNICODE_STRING DosPath;
    PVOID Handle;
} CURDIR, *PCURDIR;

typedef struct RTL_DRIVE_LETTER_CURDIR
{
    USHORT              Flags;
    USHORT              Length;
    ULONG               TimeStamp;
    UNICODE_STRING      DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct tagRTL_BITMAP {
    ULONG  SizeOfBitMap; /* Number of bits in the bitmap */
    PULONG Buffer; /* Bitmap data, assumed sized to a DWORD boundary */
} RTL_BITMAP, *PRTL_BITMAP;

typedef const RTL_BITMAP *PCRTL_BITMAP;

typedef struct tagRTL_BITMAP_RUN {
    ULONG StartingIndex; /* Bit position at which run starts */
    ULONG NumberOfBits;  /* Size of the run in bits */
} RTL_BITMAP_RUN, *PRTL_BITMAP_RUN;

typedef const RTL_BITMAP_RUN *PCRTL_BITMAP_RUN;

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
    ULONG               AllocationSize;
    ULONG               Size;
    ULONG               Flags;
    ULONG               DebugFlags;
    HANDLE              ConsoleHandle;
    ULONG               ConsoleFlags;
    HANDLE              hStdInput;
    HANDLE              hStdOutput;
    HANDLE              hStdError;
    CURDIR              CurrentDirectory;
    UNICODE_STRING      DllPath;
    UNICODE_STRING      ImagePathName;
    UNICODE_STRING      CommandLine;
    PWSTR               Environment;
    ULONG               dwX;
    ULONG               dwY;
    ULONG               dwXSize;
    ULONG               dwYSize;
    ULONG               dwXCountChars;
    ULONG               dwYCountChars;
    ULONG               dwFillAttribute;
    ULONG               dwFlags;
    ULONG               wShowWindow;
    UNICODE_STRING      WindowTitle;
    UNICODE_STRING      Desktop;
    UNICODE_STRING      ShellInfo;
    UNICODE_STRING      RuntimeInfo;
    RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

/* value for Flags field (FIXME: not the correct name) */
#define PROCESS_PARAMS_FLAG_NORMALIZED 1

typedef struct _PEB_LDR_DATA
{
    ULONG               Length;
    BOOLEAN             Initialized;
    PVOID               SsHandle;
    LIST_ENTRY          InLoadOrderModuleList;
    LIST_ENTRY          InMemoryOrderModuleList;
    LIST_ENTRY          InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _GDI_TEB_BATCH
{
    ULONG  Offset;
    HANDLE HDC;
    ULONG  Buffer[0x136];
} GDI_TEB_BATCH;

/***********************************************************************
 * PEB data structure
 */
typedef struct _PEB
{
    BOOLEAN                      InheritedAddressSpace;             /*  00 */
    BOOLEAN                      ReadImageFileExecOptions;          /*  01 */
    BOOLEAN                      BeingDebugged;                     /*  02 */
    BOOLEAN                      SpareBool;                         /*  03 */
    HANDLE                       Mutant;                            /*  04 */
    HMODULE                      ImageBaseAddress;                  /*  08 */
    PPEB_LDR_DATA                LdrData;                           /*  0c */
    RTL_USER_PROCESS_PARAMETERS *ProcessParameters;                 /*  10 */
    PVOID                        SubSystemData;                     /*  14 */
    HANDLE                       ProcessHeap;                       /*  18 */
    PRTL_CRITICAL_SECTION        FastPebLock;                       /*  1c */
    PVOID /*PPEBLOCKROUTINE*/    FastPebLockRoutine;                /*  20 */
    PVOID /*PPEBLOCKROUTINE*/    FastPebUnlockRoutine;              /*  24 */
    ULONG                        EnvironmentUpdateCount;            /*  28 */
    PVOID                        KernelCallbackTable;               /*  2c */
    PVOID                        EventLogSection;                   /*  30 */
    PVOID                        EventLog;                          /*  34 */
    PVOID /*PPEB_FREE_BLOCK*/    FreeList;                          /*  38 */
    ULONG                        TlsExpansionCounter;               /*  3c */
    PRTL_BITMAP                  TlsBitmap;                         /*  40 */
    ULONG                        TlsBitmapBits[2];                  /*  44 */
    PVOID                        ReadOnlySharedMemoryBase;          /*  4c */
    PVOID                        ReadOnlySharedMemoryHeap;          /*  50 */
    PVOID                       *ReadOnlyStaticServerData;          /*  54 */
    PVOID                        AnsiCodePageData;                  /*  58 */
    PVOID                        OemCodePageData;                   /*  5c */
    PVOID                        UnicodeCaseTableData;              /*  60 */
    ULONG                        NumberOfProcessors;                /*  64 */
    ULONG                        NtGlobalFlag;                      /*  68 */
    BYTE                         Spare2[4];                         /*  6c */
    LARGE_INTEGER                CriticalSectionTimeout;            /*  70 */
    ULONG                        HeapSegmentReserve;                /*  78 */
    ULONG                        HeapSegmentCommit;                 /*  7c */
    ULONG                        HeapDeCommitTotalFreeThreshold;    /*  80 */
    ULONG                        HeapDeCommitFreeBlockThreshold;    /*  84 */
    ULONG                        NumberOfHeaps;                     /*  88 */
    ULONG                        MaximumNumberOfHeaps;              /*  8c */
    PVOID                       *ProcessHeaps;                      /*  90 */
    PVOID                        GdiSharedHandleTable;              /*  94 */
    PVOID                        ProcessStarterHelper;              /*  98 */
    PVOID                        GdiDCAttributeList;                /*  9c */
    PVOID                        LoaderLock;                        /*  a0 */
    ULONG                        OSMajorVersion;                    /*  a4 */
    ULONG                        OSMinorVersion;                    /*  a8 */
    ULONG                        OSBuildNumber;                     /*  ac */
    ULONG                        OSPlatformId;                      /*  b0 */
    ULONG                        ImageSubSystem;                    /*  b4 */
    ULONG                        ImageSubSystemMajorVersion;        /*  b8 */
    ULONG                        ImageSubSystemMinorVersion;        /*  bc */
    ULONG                        ImageProcessAffinityMask;          /*  c0 */
    ULONG                        GdiHandleBuffer[34];               /*  c4 */
    ULONG                        PostProcessInitRoutine;            /* 14c */
    PRTL_BITMAP                  TlsExpansionBitmap;                /* 150 */
    ULONG                        TlsExpansionBitmapBits[32];        /* 154 */
    ULONG                        SessionId;                         /* 1d4 */
} PEB, *PPEB;


/***********************************************************************
 * TEB data structure
 */
#ifndef WINE_NO_TEB  /* don't define TEB if included from thread.h */
# ifndef WINE_TEB_DEFINED
# define WINE_TEB_DEFINED
typedef struct _TEB
{
    NT_TIB          Tib;                        /* 000 */
    PVOID           EnvironmentPointer;         /* 01c */
    CLIENT_ID       ClientId;                   /* 020 */
    PVOID           ActiveRpcHandle;            /* 028 */
    PVOID           ThreadLocalStoragePointer;  /* 02c */
    PPEB            Peb;                        /* 030 */
    ULONG           LastErrorValue;             /* 034 */
    ULONG           CountOfOwnedCriticalSections;/* 038 */
    PVOID           CsrClientThread;            /* 03c */
    PVOID           Win32ThreadInfo;            /* 040 */
    ULONG           Win32ClientInfo[31];        /* 044 used for user32 private data in Wine */
    PVOID           WOW32Reserved;              /* 0c0 */
    ULONG           CurrentLocale;              /* 0c4 */
    ULONG           FpSoftwareStatusRegister;   /* 0c8 */
    PVOID           SystemReserved1[54];        /* 0cc used for kernel32 private data in Wine */
    PVOID           Spare1;                     /* 1a4 */
    LONG            ExceptionCode;              /* 1a8 */
    BYTE            SpareBytes1[40];            /* 1ac */
    PVOID           SystemReserved2[10];        /* 1d4 used for ntdll private data in Wine */
    GDI_TEB_BATCH   GdiTebBatch;                /* 1fc */
    ULONG           gdiRgn;                     /* 6dc */
    ULONG           gdiPen;                     /* 6e0 */
    ULONG           gdiBrush;                   /* 6e4 */
    CLIENT_ID       RealClientId;               /* 6e8 */
    HANDLE          GdiCachedProcessHandle;     /* 6f0 */
    ULONG           GdiClientPID;               /* 6f4 */
    ULONG           GdiClientTID;               /* 6f8 */
    PVOID           GdiThreadLocaleInfo;        /* 6fc */
    PVOID           UserReserved[5];            /* 700 */
    PVOID           glDispachTable[280];        /* 714 */
    ULONG           glReserved1[26];            /* b74 */
    PVOID           glReserved2;                /* bdc */
    PVOID           glSectionInfo;              /* be0 */
    PVOID           glSection;                  /* be4 */
    PVOID           glTable;                    /* be8 */
    PVOID           glCurrentRC;                /* bec */
    PVOID           glContext;                  /* bf0 */
    ULONG           LastStatusValue;            /* bf4 */
    UNICODE_STRING  StaticUnicodeString;        /* bf8 used by advapi32 */
    WCHAR           StaticUnicodeBuffer[261];   /* c00 used by advapi32 */
    PVOID           DeallocationStack;          /* e0c */
    PVOID           TlsSlots[64];               /* e10 */
    LIST_ENTRY      TlsLinks;                   /* f10 */
    PVOID           Vdm;                        /* f18 */
    PVOID           ReservedForNtRpc;           /* f1c */
    PVOID           DbgSsReserved[2];           /* f20 */
    ULONG           HardErrorDisabled;          /* f28 */
    PVOID           Instrumentation[16];        /* f2c */
    PVOID           WinSockData;                /* f6c */
    ULONG           GdiBatchCount;              /* f70 */
    ULONG           Spare2;                     /* f74 */
    ULONG           Spare3;                     /* f78 */
    ULONG           Spare4;                     /* f7c */
    PVOID           ReservedForOle;             /* f80 */
    ULONG           WaitingOnLoaderLock;        /* f84 */
    PVOID           Reserved5[3];               /* f88 */
    PVOID          *TlsExpansionSlots;          /* f94 */
} TEB, *PTEB;
# endif /* WINE_TEB_DEFINED */
#endif  /* WINE_NO_TEB */

/***********************************************************************
 * Enums
 */

typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation = 1,
    FileFullDirectoryInformation,
    FileBothDirectoryInformation,
    FileBasicInformation,
    FileStandardInformation,
    FileInternalInformation,
    FileEaInformation,
    FileAccessInformation,
    FileNameInformation,
    FileRenameInformation,
    FileLinkInformation,
    FileNamesInformation,
    FileDispositionInformation,
    FilePositionInformation,
    FileFullEaInformation,
    FileModeInformation,
    FileAlignmentInformation,
    FileAllInformation,
    FileAllocationInformation,
    FileEndOfFileInformation,
    FileAlternateNameInformation,
    FileStreamInformation,
    FilePipeInformation,
    FilePipeLocalInformation,
    FilePipeRemoteInformation,
    FileMailslotQueryInformation,
    FileMailslotSetInformation,
    FileCompressionInformation,
    FileObjectIdInformation,
    FileCompletionInformation,
    FileMoveClusterInformation,
    FileQuotaInformation,
    FileReparsePointInformation,
    FileNetworkOpenInformation,
    FileAttributeTagInformation,
    FileTrackingInformation,
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef struct _FILE_DIRECTORY_INFORMATION {
    ULONG               NextEntryOffset;
    ULONG               FileIndex;
    LARGE_INTEGER       CreationTime;
    LARGE_INTEGER       LastAccessTime;
    LARGE_INTEGER       LastWriteTime;
    LARGE_INTEGER       ChangeTime;
    LARGE_INTEGER       EndOfFile;
    LARGE_INTEGER       AllocationSize;
    ULONG               FileAttributes;
    ULONG               FileNameLength;
    WCHAR               FileName[ANYSIZE_ARRAY];
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

typedef struct _FILE_FULL_DIRECTORY_INFORMATION {
    ULONG               NextEntryOffset;
    ULONG               FileIndex;
    LARGE_INTEGER       CreationTime;
    LARGE_INTEGER       LastAccessTime;
    LARGE_INTEGER       LastWriteTime;
    LARGE_INTEGER       ChangeTime;
    LARGE_INTEGER       EndOfFile;
    LARGE_INTEGER       AllocationSize;
    ULONG               FileAttributes;
    ULONG               FileNameLength;
    ULONG               EaSize;
    WCHAR               FileName[ANYSIZE_ARRAY];
} FILE_FULL_DIRECTORY_INFORMATION, *PFILE_FULL_DIRECTORY_INFORMATION,
  FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;

typedef struct _FILE_BOTH_DIRECTORY_INFORMATION {
    ULONG               NextEntryOffset;
    ULONG               FileIndex;
    LARGE_INTEGER       CreationTime;
    LARGE_INTEGER       LastAccessTime;
    LARGE_INTEGER       LastWriteTime;
    LARGE_INTEGER       ChangeTime;
    LARGE_INTEGER       EndOfFile;
    LARGE_INTEGER       AllocationSize;
    ULONG               FileAttributes;
    ULONG               FileNameLength;
    ULONG               EaSize;
    CHAR                ShortNameLength;
    WCHAR               ShortName[12];
    WCHAR               FileName[ANYSIZE_ARRAY];
} FILE_BOTH_DIRECTORY_INFORMATION, *PFILE_BOTH_DIRECTORY_INFORMATION,
  FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

typedef struct _FILE_BASIC_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _FILE_STANDARD_INFORMATION {
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;

typedef struct _FILE_INTERNAL_INFORMATION {
    LARGE_INTEGER IndexNumber;
} FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;

typedef struct _FILE_EA_INFORMATION {
    ULONG EaSize;
} FILE_EA_INFORMATION, *PFILE_EA_INFORMATION;

typedef struct _FILE_ACCESS_INFORMATION {
    ACCESS_MASK AccessFlags;
} FILE_ACCESS_INFORMATION, *PFILE_ACCESS_INFORMATION;

typedef struct _FILE_NAME_INFORMATION {
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

typedef struct _FILE_RENAME_INFORMATION {
    BOOLEAN Replace;
    HANDLE RootDir;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION;

typedef struct _FILE_NAMES_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAMES_INFORMATION, *PFILE_NAMES_INFORMATION;

typedef struct _FILE_DISPOSITION_INFORMATION {
    BOOLEAN DoDeleteFile;
} FILE_DISPOSITION_INFORMATION, *PFILE_DISPOSITION_INFORMATION;

typedef struct _FILE_POSITION_INFORMATION {
    LARGE_INTEGER CurrentByteOffset;
} FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;

typedef struct _FILE_ALIGNMENT_INFORMATION {
    ULONG AlignmentRequirement;
} FILE_ALIGNMENT_INFORMATION, *PFILE_ALIGNMENT_INFORMATION;

typedef struct _FILE_ALLOCATION_INFORMATION {
    LARGE_INTEGER AllocationSize;
} FILE_ALLOCATION_INFORMATION, *PFILE_ALLOCATION_INFORMATION;

typedef struct _FILE_END_OF_FILE_INFORMATION {
    LARGE_INTEGER EndOfFile;
} FILE_END_OF_FILE_INFORMATION, *PFILE_END_OF_FILE_INFORMATION;

typedef struct _FILE_NETWORK_OPEN_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;

typedef struct _FILE_FULL_EA_INFORMATION {
    ULONG NextEntryOffset;
    UCHAR Flags;
    UCHAR EaNameLength;
    USHORT EaValueLength;
    CHAR EaName[1];
} FILE_FULL_EA_INFORMATION, *PFILE_FULL_EA_INFORMATION;

typedef struct _FILE_MODE_INFORMATION {
    ULONG Mode;
} FILE_MODE_INFORMATION, *PFILE_MODE_INFORMATION;

typedef struct _FILE_STREAM_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG StreamNameLength;
    LARGE_INTEGER StreamSize;
    LARGE_INTEGER StreamAllocationSize;
    WCHAR StreamName[1];
} FILE_STREAM_INFORMATION, *PFILE_STREAM_INFORMATION;

typedef struct _FILE_ATTRIBUTE_TAG_INFORMATION
{
    ULONG FileAttributes;
    ULONG ReparseTag;
} FILE_ATTRIBUTE_TAG_INFORMATION, *PFILE_ATTRIBUTE_TAG_INFORMATION;

typedef struct _FILE_MAILSLOT_QUERY_INFORMATION {
    ULONG MaximumMessageSize;
    ULONG MailslotQuota;
    ULONG NextMessageSize;
    ULONG MessagesAvailable;
    LARGE_INTEGER ReadTimeout;
} FILE_MAILSLOT_QUERY_INFORMATION, *PFILE_MAILSLOT_QUERY_INFORMATION;

typedef struct _FILE_MAILSLOT_SET_INFORMATION {
    LARGE_INTEGER ReadTimeout;
} FILE_MAILSLOT_SET_INFORMATION, *PFILE_MAILSLOT_SET_INFORMATION;

typedef struct _FILE_ALL_INFORMATION
{
    FILE_BASIC_INFORMATION     BasicInformation;
    FILE_STANDARD_INFORMATION  StandardInformation;
    FILE_INTERNAL_INFORMATION  InternalInformation;
    FILE_EA_INFORMATION        EaInformation;
    FILE_ACCESS_INFORMATION    AccessInformation;
    FILE_POSITION_INFORMATION  PositionInformation;
    FILE_MODE_INFORMATION      ModeInformation;
    FILE_ALIGNMENT_INFORMATION AlignmentInformation;
    FILE_NAME_INFORMATION      NameInformation;
} FILE_ALL_INFORMATION, *PFILE_ALL_INFORMATION;

typedef enum _FSINFOCLASS {
    FileFsVolumeInformation = 1,
    FileFsLabelInformation,
    FileFsSizeInformation,
    FileFsDeviceInformation,
    FileFsAttributeInformation,
    FileFsControlInformation,
    FileFsFullSizeInformation,
    FileFsObjectIdInformation,
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;

typedef enum _KEY_INFORMATION_CLASS {
    KeyBasicInformation,
    KeyNodeInformation,
    KeyFullInformation
} KEY_INFORMATION_CLASS;

typedef enum _KEY_VALUE_INFORMATION_CLASS {
    KeyValueBasicInformation,
    KeyValueFullInformation,
    KeyValuePartialInformation,
    KeyValueFullInformationAlign64,
    KeyValuePartialInformationAlign64
} KEY_VALUE_INFORMATION_CLASS;

typedef enum _OBJECT_INFORMATION_CLASS {
    ObjectBasicInformation,
    ObjectNameInformation,
    ObjectTypeInformation,
    ObjectAllInformation,
    ObjectDataInformation
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation = 0,
    ProcessQuotaLimits = 1,
    ProcessIoCounters = 2,
    ProcessVmCounters = 3,
    ProcessTimes = 4,
    ProcessBasePriority = 5,
    ProcessRaisePriority = 6,
    ProcessDebugPort = 7,
    ProcessExceptionPort = 8,
    ProcessAccessToken = 9,
    ProcessLdtInformation = 10,
    ProcessLdtSize = 11,
    ProcessDefaultHardErrorMode = 12,
    ProcessIoPortHandlers = 13,
    ProcessPooledUsageAndLimits = 14,
    ProcessWorkingSetWatch = 15,
    ProcessUserModeIOPL = 16,
    ProcessEnableAlignmentFaultFixup = 17,
    ProcessPriorityClass = 18,
    ProcessWx86Information = 19,
    ProcessHandleCount = 20,
    ProcessAffinityMask = 21,
    ProcessPriorityBoost = 22,
    ProcessDeviceMap = 23,
    ProcessSessionInformation = 24,
    ProcessForegroundInformation = 25,
    ProcessWow64Information = 26,
    ProcessImageFileName = 27,
    ProcessLUIDDeviceMapsEnabled = 28,
    ProcessBreakOnTermination = 29,
    ProcessDebugObjectHandle = 30,
    ProcessDebugFlags = 31,
    ProcessHandleTracing = 32,
    MaxProcessInfoClass
} PROCESSINFOCLASS, PROCESS_INFORMATION_CLASS;

typedef enum _SECTION_INHERIT {
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation = 0,
    SystemCpuInformation = 1,
    SystemPerformanceInformation = 2,
    SystemTimeOfDayInformation = 3, /* was SystemTimeInformation */
    Unknown4,
    SystemProcessInformation = 5,
    Unknown6,
    Unknown7,
    SystemProcessorPerformanceInformation = 8,
    Unknown9,
    Unknown10,
    SystemModuleInformation = 11,
    Unknown12,
    Unknown13,
    Unknown14,
    Unknown15,
    SystemHandleInformation = 16,
    Unknown17,
    SystemPageFileInformation = 18,
    Unknown19,
    Unknown20,
    SystemCacheInformation = 21,
    Unknown22,
    SystemInterruptInformation = 23,
    SystemDpcBehaviourInformation = 24,
    SystemFullMemoryInformation = 25,
    SystemNotImplemented6 = 25,
    SystemLoadImage = 26,
    SystemUnloadImage = 27,
    SystemTimeAdjustmentInformation = 28,
    SystemTimeAdjustment = 28,
    SystemSummaryMemoryInformation = 29,
    SystemNotImplemented7 = 29,
    SystemNextEventIdInformation = 30,
    SystemNotImplemented8 = 30,
    SystemEventIdsInformation = 31,
    SystemCrashDumpInformation = 32,
    SystemExceptionInformation = 33,
    SystemCrashDumpStateInformation = 34,
    SystemKernelDebuggerInformation = 35,
    SystemContextSwitchInformation = 36,
    SystemRegistryQuotaInformation = 37,
    SystemCurrentTimeZoneInformation = 44,
    SystemTimeZoneInformation = 44,
    SystemLookasideInformation = 45,
    SystemSetTimeSlipEvent = 46,
    SystemCreateSession = 47,
    SystemDeleteSession = 48,
    SystemInvalidInfoClass4 = 49,
    SystemRangeStartInformation = 50,
    SystemVerifierInformation = 51,
    SystemAddVerifier = 52,
    SystemSessionProcessesInformation	= 53,
    SystemInformationClassMax
} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;

typedef enum _TIMER_TYPE {
    NotificationTimer,
    SynchronizationTimer
} TIMER_TYPE;

typedef enum _THREADINFOCLASS {
    ThreadBasicInformation,
    ThreadTimes,
    ThreadPriority,
    ThreadBasePriority,
    ThreadAffinityMask,
    ThreadImpersonationToken,
    ThreadDescriptorTableEntry,
    ThreadEnableAlignmentFaultFixup,
    ThreadEventPair_Reusable,
    ThreadQuerySetWin32StartAddress,
    ThreadZeroTlsCell,
    ThreadPerformanceCount,
    ThreadAmILastThread,
    ThreadIdealProcessor,
    ThreadPriorityBoost,
    ThreadSetTlsArrayAddress,
    ThreadIsIoPending,
    MaxThreadInfoClass
} THREADINFOCLASS;

typedef struct _THREAD_BASIC_INFORMATION
{
    NTSTATUS  ExitStatus;
    PVOID     TebBaseAddress;
    CLIENT_ID ClientId;
    ULONG     AffinityMask;
    LONG      Priority;
    LONG      BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef struct _KERNEL_USER_TIMES {
    LARGE_INTEGER  CreateTime;
    LARGE_INTEGER  ExitTime;
    LARGE_INTEGER  KernelTime;
    LARGE_INTEGER  UserTime;
} KERNEL_USER_TIMES, *PKERNEL_USER_TIMES;

typedef enum _WINSTATIONINFOCLASS {
    WinStationInformation = 8
} WINSTATIONINFOCLASS;

typedef enum _MEMORY_INFORMATION_CLASS {
    MemoryBasicInformation,
    MemoryWorkingSetList,
    MemorySectionName,
    MemoryBasicVlmInformation
} MEMORY_INFORMATION_CLASS;

typedef enum _MUTANT_INFORMATION_CLASS
{
    MutantBasicInformation
} MUTANT_INFORMATION_CLASS, *PMUTANT_INFORMATION_CLASS;

typedef struct _MUTANT_BASIC_INFORMATION {
    LONG        CurrentCount;
    BOOLEAN     OwnedByCaller;
    BOOLEAN     AbandonedState;
} MUTANT_BASIC_INFORMATION, *PMUTANT_BASIC_INFORMATION;

typedef enum _TIMER_INFORMATION_CLASS
{
    TimerBasicInformation = 0
} TIMER_INFORMATION_CLASS;

typedef struct _TIMER_BASIC_INFORMATION
{
    LARGE_INTEGER RemainingTime;
    BOOLEAN       TimerState;
} TIMER_BASIC_INFORMATION, *PTIMER_BASIC_INFORMATION;


/* return type of RtlDetermineDosPathNameType_U (FIXME: not the correct names) */
typedef enum
{
    INVALID_PATH = 0,
    UNC_PATH,              /* "//foo" */
    ABSOLUTE_DRIVE_PATH,   /* "c:/foo" */
    RELATIVE_DRIVE_PATH,   /* "c:foo" */
    ABSOLUTE_PATH,         /* "/foo" */
    RELATIVE_PATH,         /* "foo" */
    DEVICE_PATH,           /* "//./foo" */
    UNC_DOT_PATH           /* "//." */
} DOS_PATHNAME_TYPE;

/***********************************************************************
 * IA64 specific types and data structures
 */

#ifdef __ia64__

typedef struct _FRAME_POINTERS {
  ULONGLONG MemoryStackFp;
  ULONGLONG BackingStoreFp;
} FRAME_POINTERS, *PFRAME_POINTERS;

#define UNWIND_HISTORY_TABLE_SIZE 12

typedef struct _RUNTIME_FUNCTION {
  ULONG BeginAddress;
  ULONG EndAddress;
  ULONG UnwindInfoAddress;
} RUNTIME_FUNCTION, *PRUNTIME_FUNCTION;

typedef struct _UNWIND_HISTORY_TABLE_ENTRY {
  ULONG64 ImageBase;
  ULONG64 Gp;
  PRUNTIME_FUNCTION FunctionEntry;
} UNWIND_HISTORY_TABLE_ENTRY, *PUNWIND_HISTORY_TABLE_ENTRY;

typedef struct _UNWIND_HISTORY_TABLE {
  ULONG Count;
  UCHAR Search;
  ULONG64 LowAddress;
  ULONG64 HighAddress;
  UNWIND_HISTORY_TABLE_ENTRY Entry[UNWIND_HISTORY_TABLE_SIZE];
} UNWIND_HISTORY_TABLE, *PUNWIND_HISTORY_TABLE;

#endif /* defined(__ia64__) */

/***********************************************************************
 * Types and data structures
 */

/* This is used by NtQuerySystemInformation */
typedef struct _SYSTEM_THREAD_INFORMATION{
    FILETIME    ftKernelTime;
    FILETIME    ftUserTime;
    FILETIME    ftCreateTime;
    DWORD       dwTickCount;
    DWORD       dwStartAddress;
    DWORD       dwOwningPID;
    DWORD       dwThreadID;
    DWORD       dwCurrentPriority;
    DWORD       dwBasePriority;
    DWORD       dwContextSwitches;
    DWORD       dwThreadState;
    DWORD       dwWaitReason;
    DWORD       dwUnknown;
} SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREAD_INFORMATION;

typedef struct _IO_STATUS_BLOCK {
  union {
    NTSTATUS Status;
    PVOID Pointer;
  } DUMMYUNIONNAME;

  ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef void (WINAPI * PIO_APC_ROUTINE)(PVOID,PIO_STATUS_BLOCK,ULONG);

typedef struct _KEY_BASIC_INFORMATION {
    LARGE_INTEGER LastWriteTime;
    ULONG         TitleIndex;
    ULONG         NameLength;
    WCHAR         Name[1];
} KEY_BASIC_INFORMATION, *PKEY_BASIC_INFORMATION;

typedef struct _KEY_NODE_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG         TitleIndex;
    ULONG         ClassOffset;
    ULONG         ClassLength;
    ULONG         NameLength;
    WCHAR         Name[1];
   /* Class[1]; */
} KEY_NODE_INFORMATION, *PKEY_NODE_INFORMATION;

typedef struct _KEY_FULL_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG         TitleIndex;
    ULONG         ClassOffset;
    ULONG         ClassLength;
    ULONG         SubKeys;
    ULONG         MaxNameLen;
    ULONG         MaxClassLen;
    ULONG         Values;
    ULONG         MaxValueNameLen;
    ULONG         MaxValueDataLen;
    WCHAR         Class[1];
} KEY_FULL_INFORMATION, *PKEY_FULL_INFORMATION;

typedef struct _KEY_VALUE_ENTRY
{
    PUNICODE_STRING ValueName;
    ULONG           DataLength;
    ULONG           DataOffset;
    ULONG           Type;
} KEY_VALUE_ENTRY, *PKEY_VALUE_ENTRY;

typedef struct _KEY_VALUE_BASIC_INFORMATION {
    ULONG TitleIndex;
    ULONG Type;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_VALUE_BASIC_INFORMATION, *PKEY_VALUE_BASIC_INFORMATION;

typedef struct _KEY_VALUE_FULL_INFORMATION {
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataOffset;
    ULONG DataLength;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_VALUE_FULL_INFORMATION, *PKEY_VALUE_FULL_INFORMATION;

typedef struct _KEY_VALUE_PARTIAL_INFORMATION {
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataLength;
    UCHAR Data[1];
} KEY_VALUE_PARTIAL_INFORMATION, *PKEY_VALUE_PARTIAL_INFORMATION;

typedef struct _OBJECT_ATTRIBUTES {
  ULONG Length;
  HANDLE RootDirectory;
  PUNICODE_STRING ObjectName;
  ULONG Attributes;
  PVOID SecurityDescriptor;       /* type SECURITY_DESCRIPTOR */
  PVOID SecurityQualityOfService; /* type SECURITY_QUALITY_OF_SERVICE */
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _OBJECT_DATA_INFORMATION {
    BOOLEAN InheritHandle;
    BOOLEAN ProtectFromClose;
} OBJECT_DATA_INFORMATION, *POBJECT_DATA_INFORMATION;

typedef struct _PROCESS_BASIC_INFORMATION {
#ifdef __WINESRC__
    DWORD ExitStatus;
    DWORD PebBaseAddress;
    DWORD AffinityMask;
    DWORD BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
#else
    PVOID Reserved1;
    PPEB PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID Reserved3;
#endif
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

typedef struct _RTL_HEAP_DEFINITION {
    ULONG Length; /* = sizeof(RTL_HEAP_DEFINITION) */

    ULONG Unknown[11];
} RTL_HEAP_DEFINITION, *PRTL_HEAP_DEFINITION;

typedef struct _RTL_RWLOCK {
    RTL_CRITICAL_SECTION rtlCS;

    HANDLE hSharedReleaseSemaphore;
    UINT   uSharedWaiters;

    HANDLE hExclusiveReleaseSemaphore;
    UINT   uExclusiveWaiters;

    INT    iNumberActive;
    HANDLE hOwningThreadId;
    DWORD  dwTimeoutBoost;
    PVOID  pDebugInfo;
} RTL_RWLOCK, *LPRTL_RWLOCK;

/* System Information Class 0x00 */

typedef struct _SYSTEM_BASIC_INFORMATION {
#ifdef __WINESRC__
    DWORD dwUnknown1;
    ULONG uKeMaximumIncrement;
    ULONG uPageSize;
    ULONG uMmNumberOfPhysicalPages;
    ULONG uMmLowestPhysicalPage;
    ULONG uMmHighestPhysicalPage;
    ULONG uAllocationGranularity;
    PVOID pLowestUserAddress;
    PVOID pMmHighestUserAddress;
    ULONG uKeActiveProcessors;
    BYTE bKeNumberProcessors;
    BYTE bUnknown2;
    WORD wUnknown3;
#else
    BYTE Reserved1[24];
    PVOID Reserved2[4];
    CCHAR NumberOfProcessors;
#endif
} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

/* System Information Class 0x01 */

typedef struct _SYSTEM_CPU_INFORMATION {
    WORD Architecture;
    WORD Level;
    WORD Revision;       /* combination of CPU model and stepping */
    WORD Reserved;       /* always zero */
    DWORD FeatureSet;    /* see bit flags below */
} SYSTEM_CPU_INFORMATION, *PSYSTEM_CPU_INFORMATION;

/* definitions of bits in the Feature set for the x86 processors */
#define CPU_FEATURE_VME    0x00000005   /* Virtual 86 Mode Extensions */
#define CPU_FEATURE_TSC    0x00000002   /* Time Stamp Counter available */
#define CPU_FEATURE_CMOV   0x00000008   /* Conditional Move instruction*/
#define CPU_FEATURE_PGE    0x00000014   /* Page table Entry Global bit */ 
#define CPU_FEATURE_PSE    0x00000024   /* Page Size Extension */
#define CPU_FEATURE_MTRR   0x00000040   /* Memory Type Range Registers */
#define CPU_FEATURE_CX8    0x00000080   /* Compare and eXchange 8 byte instr. */
#define CPU_FEATURE_MMX    0x00000100   /* Multi Media eXtensions */
#define CPU_FEATURE_X86    0x00000200   /* seems to be alway ON, on the '86 */
#define CPU_FEATURE_PAT    0x00000400   /* Page Attribute Table */
#define CPU_FEATURE_FXSR   0x00000800   /* FXSAVE and FXSTORE instructions */
#define CPU_FEATURE_SEP    0x00001000   /* SYSENTER and SYSEXIT instructions */
#define CPU_FEATURE_SSE    0x00002000   /* SSE extenstions (ext. MMX) */
#define CPU_FEATURE_3DNOW  0x00008000   /* 3DNOW instructions available 
                                           (FIXME: needs to be confirmed) */
#define CPU_FEATURE_SSE2   0x00010000   /* SSE2 extensions (XMMI64) */
#define CPU_FEATURE_DS     0x00020000   /* Debug Store */
#define CPU_FEATURE_HTT    0x00040000   /* Hyper Threading Technology */

/* System Information Class 0x02 */

typedef struct _SYSTEM_PERFORMANCE_INFORMATION {
    BYTE Reserved1[312];
} SYSTEM_PERFORMANCE_INFORMATION, *PSYSTEM_PERFORMANCE_INFORMATION;

/* System Information Class 0x03 */

typedef struct _SYSTEM_TIMEOFDAY_INFORMATION {
#ifdef __WINESRC__
    LARGE_INTEGER liKeBootTime;
    LARGE_INTEGER liKeSystemTime;
    LARGE_INTEGER liExpTimeZoneBias;
    ULONG uCurrentTimeZoneId;
    DWORD dwUnknown1[5];
#else
    BYTE Reserved1[48];
#endif
} SYSTEM_TIMEOFDAY_INFORMATION, *PSYSTEM_TIMEOFDAY_INFORMATION; /* was SYSTEM_TIME_INFORMATION */

/* System Information Class 0x08 */

typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION {
#ifdef __WINESRC__
    LARGE_INTEGER liIdleTime;
    LARGE_INTEGER liKernelTime;
    LARGE_INTEGER liUserTime;
    DWORD dwSpare[5];
#else
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER Reserved1[2];
    ULONG Reserved2;
#endif
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, *PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

/* System Information Class 0x0b */

typedef struct _SYSTEM_DRIVER_INFORMATION {
    PVOID pvAddress;
    DWORD dwUnknown1;
    DWORD dwUnknown2;
    DWORD dwEntryIndex;
    DWORD dwUnknown3;
    char szName[MAX_PATH + 1];
} SYSTEM_DRIVER_INFORMATION, *PSYSTEM_DRIVER_INFORMATION;

/* System Information Class 0x10 */

typedef struct _SYSTEM_HANDLE_ENTRY {
    ULONG  OwnerPid;
    BYTE   ObjectType;
    BYTE   HandleFlags;
    USHORT HandleValue;
    PVOID  ObjectPointer;
    ULONG  AccessMask;
} SYSTEM_HANDLE_ENTRY, *PSYSTEM_HANDLE_ENTRY;

typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG               Count;
    SYSTEM_HANDLE_ENTRY Handle[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

/* System Information Class 0x15 */

typedef struct _SYSTEM_CACHE_INFORMATION {
    ULONG CurrentSize;
    ULONG PeakSize;
    ULONG PageFaultCount;
    ULONG MinimumWorkingSet;
    ULONG MaximumWorkingSet;
    ULONG unused[4];
} SYSTEM_CACHE_INFORMATION, *PSYSTEM_CACHE_INFORMATION;

/* System Information Class 0x17 */

typedef struct _SYSTEM_INTERRUPT_INFORMATION {
    BYTE Reserved1[24];
} SYSTEM_INTERRUPT_INFORMATION, *PSYSTEM_INTERRUPT_INFORMATION;

typedef struct _SYSTEM_CONFIGURATION_INFO {
    union {
        ULONG	OemId;
        struct {
	    WORD ProcessorArchitecture;
	    WORD Reserved;
	} tag1;
    } tag2;
    ULONG PageSize;
    PVOID MinimumApplicationAddress;
    PVOID MaximumApplicationAddress;
    ULONG ActiveProcessorMask;
    ULONG NumberOfProcessors;
    ULONG ProcessorType;
    ULONG AllocationGranularity;
    WORD  ProcessorLevel;
    WORD  ProcessorRevision;
} SYSTEM_CONFIGURATION_INFO, *PSYSTEM_CONFIGURATION_INFO;

typedef struct _SYSTEM_EXCEPTION_INFORMATION {
    BYTE Reserved1[16];
} SYSTEM_EXCEPTION_INFORMATION, *PSYSTEM_EXCEPTION_INFORMATION;

typedef struct _SYSTEM_LOOKASIDE_INFORMATION {
    BYTE Reserved1[32];
} SYSTEM_LOOKASIDE_INFORMATION, *PSYSTEM_LOOKASIDE_INFORMATION;

typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION {
	BOOLEAN  DebuggerEnabled;
	BOOLEAN  DebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

/* System Information Class 0x05 */

typedef struct _VM_COUNTERS_ {
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG  PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
} VM_COUNTERS, *PVM_COUNTERS;

typedef struct _SYSTEM_PROCESS_INFORMATION {
#ifdef __WINESRC__
    DWORD dwOffset;
    DWORD dwThreadCount;
    DWORD dwUnknown1[6];
    FILETIME ftCreationTime;
    FILETIME ftUserTime;
    FILETIME ftKernelTime;
    UNICODE_STRING ProcessName;
    DWORD dwBasePriority;
    DWORD dwProcessID;
    DWORD dwParentProcessID;
    DWORD dwHandleCount;
    DWORD dwUnknown3;
    DWORD dwUnknown4;
    VM_COUNTERS vmCounters;
    IO_COUNTERS ioCounters;
    SYSTEM_THREAD_INFORMATION ti[1];
#else
    ULONG NextEntryOffset;
    BYTE Reserved1[52];
    PVOID Reserved2[3];
    HANDLE UniqueProcessId;
    PVOID Reserved3;
    ULONG HandleCount;
    BYTE Reserved4[4];
    PVOID Reserved5[11];
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER Reserved6[6];
#endif
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

typedef struct _SYSTEM_REGISTRY_QUOTA_INFORMATION {
    ULONG RegistryQuotaAllowed;
    ULONG RegistryQuotaUsed;
    PVOID Reserved1;
} SYSTEM_REGISTRY_QUOTA_INFORMATION, *PSYSTEM_REGISTRY_QUOTA_INFORMATION;

typedef struct _SYSTEM_TIME_ADJUSTMENT {
    ULONG   TimeAdjustment;
    BOOLEAN TimeAdjustmentDisabled;
} SYSTEM_TIME_ADJUSTMENT, *PSYSTEM_TIME_ADJUSTMENT;

typedef struct _TIME_FIELDS
{   CSHORT Year;
    CSHORT Month;
    CSHORT Day;
    CSHORT Hour;
    CSHORT Minute;
    CSHORT Second;
    CSHORT Milliseconds;
    CSHORT Weekday;
} TIME_FIELDS, *PTIME_FIELDS;

typedef struct _WINSTATIONINFORMATIONW {
  BYTE Reserved2[70];
  ULONG LogonId;
  BYTE Reserved3[1140];
} WINSTATIONINFORMATIONW, *PWINSTATIONINFORMATIONW;

typedef BOOLEAN (WINAPI * PWINSTATIONQUERYINFORMATIONW)(HANDLE,ULONG,WINSTATIONINFOCLASS,PVOID,ULONG,PULONG);

typedef struct _LDR_RESOURCE_INFO
{
    ULONG Type;
    ULONG Name;
    ULONG Language;
} LDR_RESOURCE_INFO, *PLDR_RESOURCE_INFO;


/* debug buffer definitions */

typedef struct _DEBUG_BUFFER {
  HANDLE SectionHandle;
  PVOID  SectionBase;
  PVOID  RemoteSectionBase;
  ULONG  SectionBaseDelta;
  HANDLE EventPairHandle;
  ULONG  Unknown[2];
  HANDLE RemoteThreadHandle;
  ULONG  InfoClassMask;
  ULONG  SizeOfInfo;
  ULONG  AllocatedSize;
  ULONG  SectionSize;
  PVOID  ModuleInformation;
  PVOID  BackTraceInformation;
  PVOID  HeapInformation;
  PVOID  LockInformation;
  PVOID  Reserved[8];
} DEBUG_BUFFER, *PDEBUG_BUFFER;

#define PDI_MODULES                       0x01
#define PDI_BACKTRACE                     0x02
#define PDI_HEAPS                         0x04
#define PDI_HEAP_TAGS                     0x08
#define PDI_HEAP_BLOCKS                   0x10
#define PDI_LOCKS                         0x20

typedef struct _DEBUG_MODULE_INFORMATION {
  ULONG  Reserved[2];
  ULONG  Base;
  ULONG  Size;
  ULONG  Flags;
  USHORT Index;
  USHORT Unknown;
  USHORT LoadCount;
  USHORT ModuleNameOffset;
  CHAR   ImageName[256];
} DEBUG_MODULE_INFORMATION, *PDEBUG_MODULE_INFORMATION;

typedef struct _DEBUG_HEAP_INFORMATION {
  ULONG  Base;
  ULONG  Flags;
  USHORT Granularity;
  USHORT Unknown;
  ULONG  Allocated;
  ULONG  Committed;
  ULONG  TagCount;
  ULONG  BlockCount;
  ULONG  Reserved[7];
  PVOID  Tags;
  PVOID  Blocks;
} DEBUG_HEAP_INFORMATION, *PDEBUG_HEAP_INFORMATION;

typedef struct _DEBUG_LOCK_INFORMATION {
  PVOID  Address;
  USHORT Type;
  USHORT CreatorBackTraceIndex;
  ULONG  OwnerThreadId;
  ULONG  ActiveCount;
  ULONG  ContentionCount;
  ULONG  EntryCount;
  ULONG  RecursionCount;
  ULONG  NumberOfSharedWaiters;
  ULONG  NumberOfExclusiveWaiters;
} DEBUG_LOCK_INFORMATION, *PDEBUG_LOCK_INFORMATION;

typedef struct _PORT_MESSAGE_HEADER {
  USHORT DataSize;
  USHORT MessageSize;
  USHORT MessageType;
  USHORT VirtualRangesOffset;
  CLIENT_ID ClientId;
  ULONG MessageId;
  ULONG SectionSize;
} PORT_MESSAGE_HEADER, *PPORT_MESSAGE_HEADER, PORT_MESSAGE, *PPORT_MESSAGE;

typedef unsigned short RTL_ATOM, *PRTL_ATOM;

/* Wine doesn't implement atom table as NT does:
 * - in NT, atom tables are user space tables, which ntdll directly accesses
 * - on Wine, (even local) atom tables are wineserver object, hence a HANDLE
 */
typedef struct atom_table *RTL_ATOM_TABLE, **PRTL_ATOM_TABLE;

typedef enum _ATOM_INFORMATION_CLASS {
   AtomBasicInformation         = 0,
   AtomTableInformation         = 1,
} ATOM_INFORMATION_CLASS;

typedef struct _ATOM_BASIC_INFORMATION {
   USHORT       ReferenceCount;
   USHORT       Pinned;
   USHORT       NameLength;
   WCHAR        Name[1];
} ATOM_BASIC_INFORMATION, *PATOM_BASIC_INFORMATION;

/* FIXME: names probably not correct */
typedef struct _RTL_HANDLE
{
    struct _RTL_HANDLE * Next;
} RTL_HANDLE;

/* FIXME: names probably not correct */
typedef struct _RTL_HANDLE_TABLE
{
    ULONG MaxHandleCount;  /* 0x00 */
    ULONG HandleSize;      /* 0x04 */
    ULONG Unused[2];       /* 0x08-0x0c */
    PVOID NextFree;        /* 0x10 */
    PVOID FirstHandle;     /* 0x14 */
    PVOID ReservedMemory;  /* 0x18 */
    PVOID MaxHandle;       /* 0x1c */
} RTL_HANDLE_TABLE;

/***********************************************************************
 * Defines
 */

/* flags for NtCreateFile and NtOpenFile */
#define FILE_DIRECTORY_FILE             0x00000001
#define FILE_WRITE_THROUGH              0x00000002
#define FILE_SEQUENTIAL_ONLY            0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING  0x00000008
#define FILE_SYNCHRONOUS_IO_ALERT       0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT    0x00000020
#define FILE_NON_DIRECTORY_FILE         0x00000040
#define FILE_CREATE_TREE_CONNECTION     0x00000080
#define FILE_COMPLETE_IF_OPLOCKED       0x00000100
#define FILE_NO_EA_KNOWLEDGE            0x00000200
#define FILE_OPEN_FOR_RECOVERY          0x00000400
#define FILE_RANDOM_ACCESS              0x00000800
#define FILE_DELETE_ON_CLOSE            0x00001000
#define FILE_OPEN_BY_FILE_ID            0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT     0x00004000
#define FILE_NO_COMPRESSION             0x00008000
#define FILE_RESERVE_OPFILTER           0x00100000
#define FILE_TRANSACTED_MODE            0x00200000
#define FILE_OPEN_OFFLINE_FILE          0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY  0x00800000

#define FILE_ATTRIBUTE_VALID_FLAGS      0x00007fb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS  0x000031a7

/* status for NtCreateFile or NtOpenFile */
#define FILE_SUPERSEDED                 0
#define FILE_OPENED                     1
#define FILE_CREATED                    2
#define FILE_OVERWRITTEN                3
#define FILE_EXISTS                     4
#define FILE_DOES_NOT_EXIST             5

/* disposition for NtCreateFile */
#define FILE_SUPERSEDE                  0
#define FILE_OPEN                       1
#define FILE_CREATE                     2
#define FILE_OPEN_IF                    3
#define FILE_OVERWRITE                  4
#define FILE_OVERWRITE_IF               5
#define FILE_MAXIMUM_DISPOSITION        5

/* Characteristics of a File System */
#define FILE_REMOVABLE_MEDIA            0x00000001
#define FILE_READ_ONLY_DEVICE           0x00000002
#define FILE_FLOPPY_DISKETTE            0x00000004
#define FILE_WRITE_ONE_MEDIA            0x00000008
#define FILE_REMOTE_DEVICE              0x00000010
#define FILE_DEVICE_IS_MOUNTED          0x00000020
#define FILE_VIRTUAL_VOLUME             0x00000040
#define FILE_AUTOGENERATED_DEVICE_NAME  0x00000080
#define FILE_DEVICE_SECURE_OPEN         0x00000100

/* options for NtCreateNamedPipeFile */
#define FILE_PIPE_INBOUND               0x00000000
#define FILE_PIPE_OUTBOUND              0x00000001
#define FILE_PIPE_FULL_DUPLEX           0x00000002

#if (_WIN32_WINNT >= 0x0501)
#define INTERNAL_TS_ACTIVE_CONSOLE_ID ( *((volatile ULONG*)(0x7ffe02d8)) )
#endif /* (_WIN32_WINNT >= 0x0501) */

#define LOGONID_CURRENT    ((ULONG)-1)

#define OBJ_INHERIT          0x00000002L
#define OBJ_PERMANENT        0x00000010L
#define OBJ_EXCLUSIVE        0x00000020L
#define OBJ_CASE_INSENSITIVE 0x00000040L
#define OBJ_OPENIF           0x00000080L
#define OBJ_OPENLINK         0x00000100L
#define OBJ_KERNEL_HANDLE    0x00000200L
#define OBJ_VALID_ATTRIBUTES 0x000003F2L

#define SERVERNAME_CURRENT ((HANDLE)NULL)

typedef void (CALLBACK *PNTAPCFUNC)(ULONG_PTR,ULONG_PTR,ULONG_PTR); /* FIXME: not the right name */
typedef void (CALLBACK *PRTL_THREAD_START_ROUTINE)(LPVOID); /* FIXME: not the right name */


/* DbgPrintEx default levels */
#define DPFLTR_ERROR_LEVEL     0
#define DPFLTR_WARNING_LEVEL   1
#define DPFLTR_TRACE_LEVEL     2
#define DPFLTR_INFO_LEVEL      3
#define DPFLTR_MASK    0x8000000

/* Well-known LUID values */
#define SE_MIN_WELL_KNOWN_PRIVILEGE       2L
#define SE_CREATE_TOKEN_PRIVILEGE         2L
#define SE_ASSIGNPRIMARYTOKEN_PRIVILEGE   3L
#define SE_LOCK_MEMORY_PRIVILEGE          4L
#define SE_INCREASE_QUOTA_PRIVILEGE       5L
#define SE_UNSOLICITED_INPUT_PRIVILEGE    6L /* obsolete */
#define SE_MACHINE_ACCOUNT_PRIVILEGE      6L
#define SE_TCB_PRIVILEGE                  7L
#define SE_SECURITY_PRIVILEGE             8L
#define SE_TAKE_OWNERSHIP_PRIVILEGE       9L
#define SE_LOAD_DRIVER_PRIVILEGE         10L
#define SE_SYSTEM_PROFILE_PRIVILEGE      11L
#define SE_SYSTEMTIME_PRIVILEGE          12L
#define SE_PROF_SINGLE_PROCESS_PRIVILEGE 13L
#define SE_INC_BASE_PRIORITY_PRIVILEGE   14L
#define SE_CREATE_PAGEFILE_PRIVILEGE     15L
#define SE_CREATE_PERMANENT_PRIVILEGE    16L
#define SE_BACKUP_PRIVILEGE              17L
#define SE_RESTORE_PRIVILEGE             18L
#define SE_SHUTDOWN_PRIVILEGE            19L
#define SE_DEBUG_PRIVILEGE               20L
#define SE_AUDIT_PRIVILEGE               21L
#define SE_SYSTEM_ENVIRONMENT_PRIVILEGE  22L
#define SE_CHANGE_NOTIFY_PRIVILLEGE      23L
#define SE_REMOTE_SHUTDOWN_PRIVILEGE     24L
#define SE_UNDOCK_PRIVILEGE              25L
#define SE_SYNC_AGENT_PRIVILEGE          26L
#define SE_ENABLE_DELEGATION_PRIVILEGE   27L
#define SE_MANAGE_VOLUME_PRIVILEGE       28L
#define SE_IMPERSONATE_PRIVILEGE         29L
#define SE_CREATE_GLOBAL_PRIVILEGE       30L
#define SE_MAX_WELL_KNOWN_PRIVILEGE      SE_CREATE_GLOBAL_PRIVILEGE


/* Rtl*Registry* functions structs and defines */
#define RTL_REGISTRY_ABSOLUTE             0
#define RTL_REGISTRY_SERVICES             1
#define RTL_REGISTRY_CONTROL              2
#define RTL_REGISTRY_WINDOWS_NT           3
#define RTL_REGISTRY_DEVICEMAP            4
#define RTL_REGISTRY_USER                 5

#define RTL_REGISTRY_HANDLE       0x40000000
#define RTL_REGISTRY_OPTIONAL     0x80000000

#define RTL_QUERY_REGISTRY_SUBKEY         0x00000001
#define RTL_QUERY_REGISTRY_TOPKEY         0x00000002
#define RTL_QUERY_REGISTRY_REQUIRED       0x00000004
#define RTL_QUERY_REGISTRY_NOVALUE        0x00000008
#define RTL_QUERY_REGISTRY_NOEXPAND       0x00000010
#define RTL_QUERY_REGISTRY_DIRECT         0x00000020
#define RTL_QUERY_REGISTRY_DELETE         0x00000040

typedef NTSTATUS (WINAPI *PRTL_QUERY_REGISTRY_ROUTINE)( PCWSTR ValueName,
                                                        ULONG  ValueType,
                                                        PVOID  ValueData,
                                                        ULONG  ValueLength,
                                                        PVOID  Context,
                                                        PVOID  EntryContext);

typedef struct _RTL_QUERY_REGISTRY_TABLE
{
  PRTL_QUERY_REGISTRY_ROUTINE  QueryRoutine;
  ULONG  Flags;
  PWSTR  Name;
  PVOID  EntryContext;
  ULONG  DefaultType;
  PVOID  DefaultData;
  ULONG  DefaultLength;
} RTL_QUERY_REGISTRY_TABLE, *PRTL_QUERY_REGISTRY_TABLE;

typedef struct _KEY_MULTIPLE_VALUE_INFORMATION
{
  PUNICODE_STRING ValueName;
  ULONG DataLength;
  ULONG DataOffset;
  ULONG Type;
} KEY_MULTIPLE_VALUE_INFORMATION, *PKEY_MULTIPLE_VALUE_INFORMATION;

typedef VOID (*PTIMER_APC_ROUTINE) ( PVOID, ULONG, LONG );

typedef enum _EVENT_TYPE {
  NotificationEvent,
  SynchronizationEvent
} EVENT_TYPE, *PEVENT_TYPE;

typedef enum _EVENT_INFORMATION_CLASS {
  EventBasicInformation
} EVENT_INFORMATION_CLASS, *PEVENT_INFORMATION_CLASS;

typedef struct _EVENT_BASIC_INFORMATION {
  EVENT_TYPE EventType;
  LONG EventState;
} EVENT_BASIC_INFORMATION, *PEVENT_BASIC_INFORMATION;

typedef enum _SEMAPHORE_INFORMATION_CLASS {
  SemaphoreBasicInformation
} SEMAPHORE_INFORMATION_CLASS, *PSEMAPHORE_INFORMATION_CLASS;

typedef struct _SEMAPHORE_BASIC_INFORMATION {
  ULONG CurrentCount;
  ULONG MaximumCount;
} SEMAPHORE_BASIC_INFORMATION, *PSEMAPHORE_BASIC_INFORMATION;

typedef enum _SECTION_INFORMATION_CLASS
{
  SectionBasicInformation,
  SectionImageInformation,
} SECTION_INFORMATION_CLASS;

typedef struct _SECTION_BASIC_INFORMATION {
  ULONG BaseAddress;
  ULONG Attributes;
  LARGE_INTEGER Size;
} SECTION_BASIC_INFORMATION, *PSECTION_BASIC_INFORMATION;

typedef struct _SECTION_IMAGE_INFORMATION {
  PVOID EntryPoint;
  ULONG StackZeroBits;
  ULONG StackReserved;
  ULONG StackCommit;
  ULONG ImageSubsystem;
  WORD SubsystemVersionLow;
  WORD SubsystemVersionHigh;
  ULONG Unknown1;
  ULONG ImageCharacteristics;
  ULONG ImageMachineType;
  ULONG Unknown2[3];
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;

typedef struct _LPC_SECTION_WRITE {
  ULONG Length;
  HANDLE SectionHandle;
  ULONG SectionOffset;
  ULONG ViewSize;
  PVOID ViewBase;
  PVOID TargetViewBase;
} LPC_SECTION_WRITE, *PLPC_SECTION_WRITE;

typedef struct _LPC_SECTION_READ {
  ULONG Length;
  ULONG ViewSize;
  PVOID ViewBase;
} LPC_SECTION_READ, *PLPC_SECTION_READ;

typedef struct _LPC_MESSAGE {
  USHORT DataSize;
  USHORT MessageSize;
  USHORT MessageType;
  USHORT VirtualRangesOffset;
  CLIENT_ID ClientId;
  ULONG MessageId;
  ULONG SectionSize;
  UCHAR Data[ANYSIZE_ARRAY];
} LPC_MESSAGE, *PLPC_MESSAGE;

typedef enum _SHUTDOWN_ACTION {
  ShutdownNoReboot,
  ShutdownReboot,
  ShutdownPowerOff
} SHUTDOWN_ACTION, *PSHUTDOWN_ACTION;

typedef enum _KPROFILE_SOURCE {
  ProfileTime,
  ProfileAlignmentFixup,
  ProfileTotalIssues,
  ProfilePipelineDry,
  ProfileLoadInstructions,
  ProfilePipelineFrozen,
  ProfileBranchInstructions,
  ProfileTotalNonissues,
  ProfileDcacheMisses,
  ProfileIcacheMisses,
  ProfileCacheMisses,
  ProfileBranchMispredictions,
  ProfileStoreInstructions,
  ProfileFpInstructions,
  ProfileIntegerInstructions,
  Profile2Issue,
  Profile3Issue,
  Profile4Issue,
  ProfileSpecialInstructions,
  ProfileTotalCycles,
  ProfileIcacheIssues,
  ProfileDcacheAccesses,
  ProfileMemoryBarrierCycles,
  ProfileLoadLinkedIssues,
  ProfileMaximum
} KPROFILE_SOURCE, *PKPROFILE_SOURCE;

typedef struct _OBJDIR_INFORMATION {
  UNICODE_STRING ObjectName;
  UNICODE_STRING ObjectTypeName;
  BYTE Data[1];
} OBJDIR_INFORMATION, *POBJDIR_INFORMATION;

typedef struct _INITIAL_TEB {
  PVOID StackBase;
  PVOID StackLimit;
  PVOID StackCommit;
  PVOID StackCommitMax;
  PVOID StackReserved;
} INITIAL_TEB, *PINITIAL_TEB;

typedef enum _PORT_INFORMATION_CLASS {
  PortNoInformation
} PORT_INFORMATION_CLASS, *PPORT_INFORMATION_CLASS;

typedef enum _IO_COMPLETION_INFORMATION_CLASS {
  IoCompletionBasicInformation
} IO_COMPLETION_INFORMATION_CLASS, *PIO_COMPLETION_INFORMATION_CLASS;

typedef enum _HARDERROR_RESPONSE_OPTION {
  OptionAbortRetryIgnore,
  OptionOk,
  OptionOkCancel,
  OptionRetryCancel,
  OptionYesNo,
  OptionYesNoCancel,
  OptionShutdownSystem
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE {
  ResponseReturnToCaller,
  ResponseNotHandled,
  ResponseAbort,
  ResponseCancel,
  ResponseIgnore,
  ResponseNo,
  ResponseOk,
  ResponseRetry,
  ResponseYes
} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;

typedef enum _SYSDBG_COMMAND {
  SysDbgQueryModuleInformation,
  SysDbgQueryTraceInformation,
  SysDbgSetTracepoint,
  SysDbgSetSpecialCall,
  SysDbgClearSpecialCalls,
  SysDbgQuerySpecialCalls
} SYSDBG_COMMAND, *PSYSDBG_COMMAND;
 
/***********************************************************************
 * Inline functions
 */

#define InitializeObjectAttributes(p,n,a,r,s) \
    do { \
        (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
        (p)->RootDirectory = r; \
        (p)->Attributes = a; \
        (p)->ObjectName = n; \
        (p)->SecurityDescriptor = s; \
        (p)->SecurityQualityOfService = NULL; \
    } while (0)

#define NtCurrentProcess() ((HANDLE)-1)

#ifndef RtlFillMemory
#define RtlFillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#endif
#ifndef RtlMoveMemory
#define RtlMoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))
#endif
#define RtlStoreUlong(p,v)  do { ULONG _v = (v); memcpy((p), &_v, sizeof(_v)); } while (0)
#define RtlStoreUlonglong(p,v) do { ULONGLONG _v = (v); memcpy((p), &_v, sizeof(_v)); } while (0)
#define RtlRetrieveUlong(p,s) memcpy((p), (s), sizeof(ULONG))
#define RtlRetrieveUlonglong(p,s) memcpy((p), (s), sizeof(ULONGLONG))
#ifndef RtlZeroMemory
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

static __inline BOOLEAN RtlCheckBit(PCRTL_BITMAP lpBits, ULONG ulBit)
{
    if (lpBits && ulBit < lpBits->SizeOfBitMap &&
        lpBits->Buffer[ulBit >> 5] & (1 << (ulBit & 31)))
        return TRUE;
    return FALSE;
}

/* These are implemented as __fastcall, so we can't let Winelib apps link with them */
static __inline USHORT RtlUshortByteSwap(USHORT s)
{
    return (s >> 8) | (s << 8);
}
static __inline ULONG RtlUlongByteSwap(ULONG i)
{
#if defined(__i386__) && defined(__GNUC__)
    ULONG ret;
    __asm__("bswap %0" : "=r" (ret) : "0" (i) );
    return ret;
#else
    return ((ULONG)RtlUshortByteSwap((USHORT)i) << 16) | RtlUshortByteSwap((USHORT)(i >> 16));
#endif
}

/*************************************************************************
 * Loader functions and structures.
 *
 * Those are not part of standard Winternl.h
 */
typedef struct _LDR_MODULE
{
    LIST_ENTRY          InLoadOrderModuleList;
    LIST_ENTRY          InMemoryOrderModuleList;
    LIST_ENTRY          InInitializationOrderModuleList;
    void*               BaseAddress;
    void*               EntryPoint;
    ULONG               SizeOfImage;
    UNICODE_STRING      FullDllName;
    UNICODE_STRING      BaseDllName;
    ULONG               Flags;
    SHORT               LoadCount;
    SHORT               TlsIndex;
    HANDLE              SectionHandle;
    ULONG               CheckSum;
    ULONG               TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;

/* those defines are (some of the) regular LDR_MODULE.Flags values */
#define LDR_IMAGE_IS_DLL                0x00000004
#define LDR_LOAD_IN_PROGRESS            0x00001000
#define LDR_UNLOAD_IN_PROGRESS          0x00002000
#define LDR_NO_DLL_CALLS                0x00040000
#define LDR_PROCESS_ATTACHED            0x00080000
#define LDR_MODULE_REBASED              0x00200000

/* these ones is Wine specific */
#define LDR_DONT_RESOLVE_REFS           0x40000000
#define LDR_WINE_INTERNAL               0x80000000

/* FIXME: to be checked */
#define MAXIMUM_FILENAME_LENGTH 256

typedef struct _SYSTEM_MODULE
{
    ULONG               Reserved1;
    ULONG               Reserved2;
    PVOID               ImageBaseAddress;
    ULONG               ImageSize;
    ULONG               Flags;
    WORD                Id;
    WORD                Rank;
    WORD                Unknown;
    WORD                NameOffset;
    BYTE                Name[MAXIMUM_FILENAME_LENGTH];
} SYSTEM_MODULE, *PSYSTEM_MODULE;

typedef struct _SYSTEM_MODULE_INFORMATION
{
    ULONG               ModulesCount;
    SYSTEM_MODULE       Modules[1]; /* FIXME: should be Modules[0] */
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

NTSTATUS WINAPI LdrDisableThreadCalloutsForDll(HMODULE);
NTSTATUS WINAPI LdrFindEntryForAddress(const void*, PLDR_MODULE*);
NTSTATUS WINAPI LdrLockLoaderLock(ULONG,ULONG*,ULONG*);
NTSTATUS WINAPI LdrQueryProcessModuleInformation(SYSTEM_MODULE_INFORMATION*, ULONG, ULONG*);
NTSTATUS WINAPI LdrUnloadDll(HMODULE);
NTSTATUS WINAPI LdrUnlockLoaderLock(ULONG,ULONG);

/* list manipulation macros */
#define InitializeListHead(le)  (void)((le)->Flink = (le)->Blink = (le))
#define InsertHeadList(le,e)    do { PLIST_ENTRY f = (le)->Flink; (e)->Flink = f; (e)->Blink = (le); f->Blink = (e); (le)->Flink = (e); } while (0)
#define InsertTailList(le,e)    do { PLIST_ENTRY b = (le)->Blink; (e)->Flink = (le); (e)->Blink = b; b->Flink = (e); (le)->Blink = (e); } while (0)
#define IsListEmpty(le)         ((le)->Flink == (le))
#define RemoveEntryList(e)      do { PLIST_ENTRY f = (e)->Flink, b = (e)->Blink; f->Blink = b; b->Flink = f; (e)->Flink = (e)->Blink = NULL; } while (0)
static  __inline PLIST_ENTRY RemoveHeadList(PLIST_ENTRY le)
{
    PLIST_ENTRY f, b, e;

    e = le->Flink;
    f = le->Flink->Flink;
    b = le->Flink->Blink;
    f->Blink = b;
    b->Flink = f;

    if (e != le) e->Flink = e->Blink = NULL;
    return e;
}
static __inline PLIST_ENTRY RemoveTailList(PLIST_ENTRY le)
{
    PLIST_ENTRY f, b, e;

    e = le->Blink;
    f = le->Blink->Flink;
    b = le->Blink->Blink;
    f->Blink = b;
    b->Flink = f;

    if (e != le) e->Flink = e->Blink = NULL;
    return e;
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* defined(__cplusplus) */

#endif  /* __WINE_WINTERNL_H */
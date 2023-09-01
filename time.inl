/***
*time.inl - inline definitions for time-related functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the time-related function definitions.
*
*       [Public]
*
****/

#pragma once

#if !defined(__CRTDECL)
#define __CRTDECL   __cdecl
#endif

#ifndef _INC_TIME_INL
#define _INC_TIME_INL
#ifndef RC_INVOKED

#ifdef _USE_32BIT_TIME_T
static __inline double __CRTDECL difftime(time_t _Time1, time_t _Time2)
{
    return _difftime32(_Time1,_Time2);
}
_CRT_INSECURE_DEPRECATE(ctime_s) static __inline char * __CRTDECL ctime(const time_t * _Time)
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    return _ctime32(_Time);
#pragma warning( pop )
}
#if __STDC_WANT_SECURE_LIB__
static __inline errno_t __CRTDECL ctime_s(char *_Buffer, size_t _SizeInBytes, const time_t * _Time)
{
    return _ctime32_s(_Buffer, _SizeInBytes, _Time);
}
#endif
_CRT_INSECURE_DEPRECATE(gmtime_s) static __inline struct tm * __CRTDECL gmtime(const time_t * _Time)
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    return _gmtime32(_Time);
#pragma warning( pop )
}
#if __STDC_WANT_SECURE_LIB__
static __inline errno_t __CRTDECL gmtime_s(struct tm * _Tm, const time_t * _Time)
{
    return _gmtime32_s(_Tm, _Time);
}
#endif
_CRT_INSECURE_DEPRECATE(localtime_s) static __inline struct tm * __CRTDECL localtime(const time_t * _Time)
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    return _localtime32(_Time);
#pragma warning( pop )
}
static __inline errno_t __CRTDECL localtime_s(struct tm * _Tm, const time_t * _Time)
{
    return _localtime32_s(_Tm, _Time);
}
static __inline time_t __CRTDECL mktime(struct tm * _Tm)
{
    return _mktime32(_Tm);
}
static __inline time_t __CRTDECL _mkgmtime(struct tm * _Tm)
{
    return _mkgmtime32(_Tm);
}
static __inline time_t __CRTDECL time(time_t * _Time)
{
    return _time32(_Time);
}
#else
static __inline double __CRTDECL difftime(time_t _Time1, time_t _Time2)
{
    return _difftime64(_Time1,_Time2);
}
_CRT_INSECURE_DEPRECATE(ctime_s) static __inline char * __CRTDECL ctime(const time_t * _Time)
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    return _ctime64(_Time);
#pragma warning( pop )
}
#if __STDC_WANT_SECURE_LIB__
static __inline errno_t __CRTDECL ctime_s(char *_Buffer, size_t _SizeInBytes, const time_t * _Time)
{
    return _ctime64_s(_Buffer, _SizeInBytes, _Time);
}
#endif
_CRT_INSECURE_DEPRECATE(gmtime_s) static __inline struct tm * __CRTDECL gmtime(const time_t * _Time)
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    return _gmtime64(_Time);
#pragma warning( pop )
}
#if __STDC_WANT_SECURE_LIB__
static __inline errno_t __CRTDECL gmtime_s(struct tm * _Tm, const time_t * _Time)
{
    return _gmtime64_s(_Tm, _Time);
}
#endif
_CRT_INSECURE_DEPRECATE(localtime_s) static __inline struct tm * __CRTDECL localtime(const time_t * _Time)
{
#pragma warning( push )
#pragma warning( disable : 4996 )
    return _localtime64(_Time);
#pragma warning( pop )
}
static __inline errno_t __CRTDECL localtime_s(struct tm * _Tm, const time_t * _Time)
{
    return _localtime64_s(_Tm, _Time);
}
static __inline time_t __CRTDECL mktime(struct tm * _Tm)
{
    return _mktime64(_Tm);
}
static __inline time_t __CRTDECL _mkgmtime(struct tm * _Tm)
{
    return _mkgmtime64(_Tm);
}
static __inline time_t __CRTDECL time(time_t * _Time)
{
    return _time64(_Time);
}
#endif /* _USE_32BIT_TIME_T */


#endif /* RC_INVOKED */
#endif /* _INC_TIME_INL */

#pragma once

#if !defined(__CRTDECL)
#define __CRTDECL   __cdecl
#endif

#ifndef _INC_STAT_INL
#define _INC_STAT_INL

/* _STATIC_ASSERT is for enforcing boolean/integral conditions at compile time.
   Since it is purely a compile-time mechanism that generates no code, the check
   is left in even if _DEBUG is not defined. */

#ifndef _STATIC_ASSERT
#define _STATIC_ASSERT(expr) typedef char __static_assert_t[ (expr) ]
#endif

#if     !__STDC__

   /* Non-ANSI names for compatibility */

#ifdef _USE_32BIT_TIME_T
static __inline int __CRTDECL fstat(int _Desc, struct stat * _Stat)
{
	_STATIC_ASSERT(sizeof(struct stat) == sizeof(struct _stat32));
	return _fstat32(_Desc, (struct _stat32 *)_Stat);
}
static __inline int __CRTDECL stat(const char * _Filename, struct stat * _Stat)
{
	_STATIC_ASSERT(sizeof(struct stat) == sizeof(struct _stat32));
	return _stat32(_Filename, (struct _stat32 *)_Stat);
}
#else
static __inline int __CRTDECL fstat(int _Desc, struct stat * _Stat)
{
	_STATIC_ASSERT(sizeof(struct stat) == sizeof(struct _stat64i32));
	return _fstat64i32(_Desc, (struct _stat64i32 *)_Stat);
}
static __inline int __CRTDECL stat(const char * _Filename, struct stat * _Stat)
{
	_STATIC_ASSERT(sizeof(struct stat) == sizeof(struct _stat64i32));
	return _stat64i32(_Filename, (struct _stat64i32 *)_Stat);
}
#endif /* _USE_32BIT_TIME_T */
#endif /* !__STDC__ */

#endif


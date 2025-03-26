#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	long __cdecl _ftol(double d);
	long long __cdecl _ftol2(double d);
	void* __cdecl _chkstk(void);

	void __cdecl _except_handler3(void);
	void __cdecl _except_handler4(void);

	//void __cdecl _purecall(void);

	// Intrinsic float versions
	double __cdecl _CIsin(double x);
	double __cdecl _CIcos(double x);
	double __cdecl _CIsqrt(double x);

	// Runtime checks (safe to stub out)
	//void __cdecl _RTC_CheckEsp(void);

	// Optional SEH prolog/epilog helpers
	void __cdecl _EH_prolog(void);
	void __cdecl _EH_epilog(void);

#ifdef __cplusplus
}
#endif
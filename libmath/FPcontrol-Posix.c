#include "lib9.h"
#include "fpuctl.h"
#include "mathi.h"

#include <stdio.h>
#include <fenv.h>
#ifdef __APPLE__
#pragma STDC FENV_ACCESS_ON
#endif

void
FPinit(void)
{
    fexcept_t fcr;
    /* XXX: FPPDBL? */
    feclearexcept(FE_ALL_EXCEPT); /* Clear pending exceptions */
    fegetexceptflag(&fcr, 0);
    fesetexceptflag(&fcr, FE_INVALID|FE_DIVBYZERO|FE_UNDERFLOW|FE_OVERFLOW);
    fesetround(FE_TONEAREST);
}

ulong
getFPstatus(void)
{
	ulong fsr = 0;

	/* on specific machines, could be table lookup */
	if(fetestexcept(FE_INEXACT))   fsr |= INEX;
	if(fetestexcept(FE_OVERFLOW))  fsr |= OVFL;
	if(fetestexcept(FE_UNDERFLOW)) fsr |= UNFL;
	if(fetestexcept(FE_DIVBYZERO)) fsr |= ZDIV;
	if(fetestexcept(FE_INVALID))   fsr |= INVAL;
	return fsr;
}

ulong
FPstatus(ulong fsr, ulong mask)
{
    fexcept_t fcr;
	int fsr9 = 0;
	int old = getFPstatus();
	fsr = (fsr&mask) | (old&~mask);
	if(fsr&INEX)  fsr9 |= FE_INEXACT;
	if(fsr&OVFL)  fsr9 |= FE_OVERFLOW;
	if(fsr&UNFL)  fsr9 |= FE_UNDERFLOW;
	if(fsr&ZDIV)  fsr9 |= FE_DIVBYZERO;
	if(fsr&INVAL) fsr9 |= FE_INVALID;
    fegetexceptflag(&fcr, 0);
    fesetexceptflag(&fcr, fsr9);
	return(old&mask);
}

ulong
getFPcontrol(void)
{
	ulong fcr = 0;
	switch(fegetround()){
		case FE_TONEAREST:	fcr = RND_NR; break;
		case FE_DOWNWARD:	fcr = RND_NINF; break;
		case FE_UPWARD:	    fcr = RND_PINF; break;
		case FE_TOWARDZERO:	fcr = RND_Z; break;
	}
	if(fetestexcept(FE_INEXACT))   fcr |= INEX;
	if(fetestexcept(FE_OVERFLOW))  fcr |= OVFL;
	if(fetestexcept(FE_UNDERFLOW)) fcr |= UNFL;
	if(fetestexcept(FE_DIVBYZERO)) fcr |= ZDIV;
	if(fetestexcept(FE_INVALID))   fcr |= INVAL;
	return fcr;
}

ulong
FPcontrol(ulong fcr, ulong mask)
{
    fexcept_t flag;
	int fcr9 = 0 /*XXX: FPPDBL?*/;
    int fcr9_r = 0;
	ulong old = getFPcontrol();
	fcr = (fcr&mask) | (old&~mask);
	if(fcr&INEX)  fcr9 |= FE_INEXACT;
	if(fcr&OVFL)  fcr9 |= FE_OVERFLOW;
	if(fcr&UNFL)  fcr9 |= FE_UNDERFLOW;
	if(fcr&ZDIV)  fcr9 |= FE_DIVBYZERO;
	if(fcr&INVAL) fcr9 |= FE_INVALID;
	switch(fcr&RND_MASK){
		case RND_NR:	fcr9_r |= FE_TONEAREST; break;
		case RND_NINF:	fcr9_r |= FE_DOWNWARD; break;
		case RND_PINF:	fcr9_r |= FE_UPWARD; break;
		case RND_Z:	    fcr9_r |= FE_TOWARDZERO; break;
	}
    fegetexceptflag(&flag, 0);
    fesetexceptflag(&flag, fcr9);
    fesetround(fcr9_r);
	return(old&mask);
}

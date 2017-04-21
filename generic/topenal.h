/*
 * topenal.h -
 *
 * Tcl bindings for OpenAL (Open Audio library)
 * Copyright (C) Danilo Chang 2016
 */

#ifndef _OPENAL
#define _OPENAL

/*
 * For C++ compilers, use extern "C"
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <tcl.h>
#include <AL/al.h>
#include <AL/alc.h>

int List_device_Cmd(ClientData arg, Tcl_Interp * interp, int objc, Tcl_Obj * CONST objv[]);
int List_capture_Cmd(ClientData arg, Tcl_Interp * interp, int objc, Tcl_Obj * CONST objv[]);
int DeviceMain(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int DeviceObjCmd(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int CaptureMain(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int CaptureObjCmd(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);


/*
 * Only the _Init function is exported.
 */

extern DLLEXPORT int	Openal_Init(Tcl_Interp * interp);

/*
 * end block for C++
 */

#ifdef __cplusplus
}
#endif

#endif /* _OPENAL */

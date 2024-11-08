/*
 * tclopenal.c -
 *
 * Tcl bindings for OpenAL (Open Audio library)
 * Copyright (C) Danilo Chang 2016-2018
 */

/*
 * For C++ compilers, use extern "C"
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topenal.h"

/*
 * end block for C++
 */

#ifdef __cplusplus
}
#endif


int List_device_Cmd(ClientData arg, Tcl_Interp * interp, int objc, Tcl_Obj * const objv[])
{
    ALboolean enumeration;
    const ALCchar *device;
    const ALCchar *next;
    Tcl_Obj *pResultStr = NULL;
    size_t len = 0;

    if(objc != 1)
    {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    // Check enumeration is supported or not
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE) {
        return TCL_ERROR;
    }

    device = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    next = alcGetString(NULL, ALC_DEVICE_SPECIFIER) + 1;

    pResultStr = Tcl_NewListObj(0, NULL);
    while (device && *device != '\0' && next && *next != '\0') {
        len = strlen(device);
        Tcl_ListObjAppendElement(interp, pResultStr, Tcl_NewStringObj(device, len));
        device += (len + 1);
        next += (len + 2);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int List_capture_Cmd(ClientData arg, Tcl_Interp * interp, int objc, Tcl_Obj * const objv[])
{
    ALboolean enumeration;
    const ALCchar *device;
    const ALCchar *next;
    Tcl_Obj *pResultStr = NULL;
    size_t len = 0;

    if(objc != 1)
    {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    // Check enumeration is supported or not
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE) {
        return TCL_ERROR;
    }

    device = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
    next = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER) + 1;

    pResultStr = Tcl_NewListObj(0, NULL);
    while (device && *device != '\0' && next && *next != '\0') {
        len = strlen(device);
        Tcl_ListObjAppendElement(interp, pResultStr, Tcl_NewStringObj(device, len));
        device += (len + 1);
        next += (len + 2);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


/*
 *----------------------------------------------------------------------
 *
 * Openal_Init --
 *
 *	Initialize the new package.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	The Openal package is created.
 *
 *----------------------------------------------------------------------
 */

int
Openal_Init(Tcl_Interp *interp)
{
    if (Tcl_InitStubs(interp, "8.4", 0) == NULL) {
	return TCL_ERROR;
    }
    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
	return TCL_ERROR;
    }
	
    Tcl_CreateObjCommand(interp, "::openal::list_devices", List_device_Cmd,
            NULL, NULL);
    
    Tcl_CreateObjCommand(interp, "::openal::list_captures", List_capture_Cmd,
            NULL, NULL);

    Tcl_CreateObjCommand(interp, "::openal::device", (Tcl_ObjCmdProc *) DeviceMain,
            (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);    

    Tcl_CreateObjCommand(interp, "::openal::capture", (Tcl_ObjCmdProc *) CaptureMain,
            (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    return TCL_OK;
}

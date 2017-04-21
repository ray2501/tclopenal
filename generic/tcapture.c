/*
 * tcapture.c -
 *
 * Tcl bindings for OpenAL (Open Audio library)
 * Copyright (C) Danilo Chang 2016-2017
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
#include <AL/al.h>
#include <AL/alc.h>

/*
 * end block for C++
 */

#ifdef __cplusplus
}
#endif

typedef struct CaptureData CaptureData;

struct CaptureData {
  ALCdevice *device;
  Tcl_Interp *interp;
  char *buffer;
  int buffersize;
  int samplerate;
  int channels;
  int bits;
};


int CaptureObjCmd(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv)
{
  CaptureData *pDevice = (CaptureData *) cd;
  int choice;
  int rc = TCL_OK;

  static const char *CAPTURE_strs[] = {
    "start",
    "captureSamples",
    "stop",
    "close",
    0
  };

  enum CAPTURE_enum {
    CAPTURE_START,
    CAPTURE_SAMPLES,
    CAPTURE_STOP,
    CAPTURE_CLOSE,
  };

  if( objc < 2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
    return TCL_ERROR;
  }

  if( Tcl_GetIndexFromObj(interp, objv[1], CAPTURE_strs, "option", 0, &choice) ){
    return TCL_ERROR;
  }

  switch( (enum CAPTURE_enum)choice ){

    /*
     * Start capture
     */
    case CAPTURE_START: {
      int size = 0;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      if(pDevice->buffer == NULL) {
        size = pDevice->buffersize * pDevice->channels * pDevice->bits / 8;
        pDevice->buffer = (char *) malloc (size * sizeof(char));
        if( pDevice->buffer == 0 ) {
            Tcl_SetResult(interp, (char *)"malloc failed", TCL_STATIC);
           return TCL_ERROR;
        }
      }

      alcCaptureStart(pDevice->device);
      break;
    }

    /*
     * capture samples
     */
    case CAPTURE_SAMPLES: {
      ALint sample;
      Tcl_Obj *return_obj = NULL;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      /*
       * When alcGetIntegerv sample is 0, returns an empty string to user
       */
      alcGetIntegerv(pDevice->device, ALC_CAPTURE_SAMPLES,
                     (ALCsizei)sizeof(ALint), &sample);
      if (sample > 0) {
          if(sample > pDevice->buffersize) { // Prevent bigger than our buffer
             sample = pDevice->buffersize;
          }

          alcCaptureSamples(pDevice->device, (ALCvoid *) pDevice->buffer, sample);
      } else {
          return_obj = Tcl_NewStringObj((char *) "", -1);
      }

      return_obj = Tcl_NewStringObj((char *) pDevice->buffer,
                   sample * pDevice->channels * pDevice->bits / 8);

      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    /*
     * Stop capture
     */
    case CAPTURE_STOP: {
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alcCaptureStop(pDevice->device);

      if(pDevice->buffer) free(pDevice->buffer);
      pDevice->buffer = NULL;

      break;
    }

    /*
     * Close function
     */
    case CAPTURE_CLOSE: {
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alcCaptureCloseDevice(pDevice->device);

      Tcl_Free((char *)pDevice);
      pDevice = NULL;

      Tcl_DeleteCommand(interp, Tcl_GetStringFromObj(objv[0], 0));
      break;
    }

  } /* End of the SWITCH statement */

  return rc;
}


int CaptureMain(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv)
{
  CaptureData *p;
  ALboolean extExist;
  const char *zArg = NULL;
  const char *devicename = NULL;
  const char *format = "AL_FORMAT_STEREO16";
  ALenum alFormat = AL_FORMAT_STEREO16;
  int samplerate = 44100;
  int buffersize = 0;
  int len = 0;
  int i = 0;

  if( objc < 2 || (objc&1)!=0 ){
    Tcl_WrongNumArgs(interp, 1, objv,
      "CAPTURE ?-device name? ?-rate samplerate? ?-format format? ?-buffersize size? "
    );
    return TCL_ERROR;
  }

  for(i=2; i+1<objc; i+=2){
    zArg = Tcl_GetStringFromObj(objv[i], 0);

    if( strcmp(zArg, "-device")==0 ){
      devicename = Tcl_GetStringFromObj(objv[i+1], &len);
      if(!devicename || len < 1) {
         Tcl_AppendResult(interp, "Error: device name is empty string!", (char*)0);
         return TCL_ERROR;
      }
    } else if( strcmp(zArg, "-rate")==0 ){
      if(Tcl_GetIntFromObj(interp, objv[i+1], &samplerate) != TCL_OK) {
         return TCL_ERROR;
      }

      if(samplerate <= 0) {
         Tcl_AppendResult(interp, "Error: samplerate needs > 0", (char*)0);
         return TCL_ERROR;
      }
    } else if( strcmp(zArg, "-format")==0 ){
      format = Tcl_GetStringFromObj(objv[i+1], &len);
      if(!format || len < 1) {
         Tcl_AppendResult(interp, "Error: format is empty string!", (char*)0);
         return TCL_ERROR;
      }
    } else if( strcmp(zArg, "-buffersize")==0 ){
      if(Tcl_GetIntFromObj(interp, objv[i+1], &buffersize) != TCL_OK) {
         return TCL_ERROR;
      }

      if(buffersize <= 0) {
         Tcl_AppendResult(interp, "Error: buffersize needs > 0", (char*)0);
         return TCL_ERROR;
      }
    } else {
      Tcl_AppendResult(interp, "unknown option: ", zArg, (char*)0);
      return TCL_ERROR;
    }
  }

  // An OpenAL 1.1 implementation will always support the ALC_EXT_CAPTURE
  // extension. Is it need to check?
  extExist = alcIsExtensionPresent(NULL, "ALC_EXT_CAPTURE");
  if (extExist == AL_FALSE) {
    return TCL_ERROR;
  }

  p = (CaptureData *)Tcl_Alloc( sizeof(*p) );
  if( p==0 ){
    Tcl_SetResult(interp, (char *)"malloc failed", TCL_STATIC);
    return TCL_ERROR;
  }

  memset(p, 0, sizeof(*p));
  p->interp = interp;

  if(strcmp(format, "AL_FORMAT_MONO8")==0) {
	  p->channels = 1;
	  p->bits = 8;
	  alFormat = AL_FORMAT_MONO8;
  } else if(strcmp(format, "AL_FORMAT_MONO16")==0) {
	  p->channels = 1;
	  p->bits = 16;
	  alFormat = AL_FORMAT_MONO16;
  } else if(strcmp(format, "AL_FORMAT_STEREO8")==0) {
	  p->channels = 2;
	  p->bits = 8;
	  alFormat = AL_FORMAT_STEREO8;
  } else if(strcmp(format, "AL_FORMAT_STEREO16")==0) {
	  p->channels = 2;
	  p->bits = 16;
	  alFormat = AL_FORMAT_STEREO16;
  } else {
	  p->channels = 2;
	  p->bits = 16;
	  alFormat = AL_FORMAT_STEREO16;
  }

  p->buffersize = buffersize;
  p->samplerate = samplerate;
  p->buffer = NULL;

  /*
   * User does not specify a device name, so get the default device
   */
  if(!devicename) {
    devicename = alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
  }

  if(p->buffersize == 0) {
      buffersize = p->samplerate;
      p->buffersize = buffersize;
  }

  // The bufsize argument specifies the number of sample frames
  p->device = alcCaptureOpenDevice(devicename, p->samplerate, alFormat,
                                   p->buffersize);
  if (!p->device) {
     alcCaptureCloseDevice(p->device);
     Tcl_Free((char *) p);

     Tcl_AppendResult(interp, "Error: alcCaptureOpenDevice open fail!", (char*)0);
     return TCL_ERROR;
  }

  zArg = Tcl_GetStringFromObj(objv[1], 0);
  Tcl_CreateObjCommand(interp, zArg, CaptureObjCmd, (char*)p, (Tcl_CmdDeleteProc *)NULL);

  return TCL_OK;
}


/*
 * topenal.c -
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
#include <AL/al.h>
#include <AL/alc.h>

/*
 * end block for C++
 */

#ifdef __cplusplus
}
#endif

typedef struct DeviceData DeviceData;

struct DeviceData {
  ALCdevice *device;
  Tcl_Interp *interp;
  ALCcontext *context;
  ALuint source;
  ALuint *buffer;
  int buffer_index;
};


int DeviceObjCmd(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv)
{
  DeviceData *pDevice = (DeviceData *) cd;
  int choice;
  int rc = TCL_OK;

  static const char *DEVICE_strs[] = {
    "setListener",
    "getListener",
    "createSource",
    "isValidSource",
    "setSource",
    "getSource",
    "queueBuffers",
    "unqueueBuffers",
    "playSource",
    "pauseSource",
    "rewindSource",
    "stopSource",
    "destroySource",
    "createBuffer",
    "isValidBuffer",
    "bufferData",
    "getBuffer",
    "destroyBuffer",
    "close",
    0
  };

  enum DEVICE_enum {
    DEVICE_LISTENER_SET,
    DEVICE_LISTENER_GET,
    DEVICE_CREATESOURCE,
    DEVICE_ISVALIDSOURCE,
    DEVICE_SETSOURCE,
    DEVICE_GETSOURCE,
    DEVICE_QUEUE_BUFFER,
    DEVICE_UNQUEUE_BUFFER,
    DEVICE_PLAYSOURCE,
    DEVICE_PAUSESOURCE,
    DEVICE_REWINDSOURCE,
    DEVICE_STOPSOURCE,
    DEVICE_DESTROYSOURCE,
    DEVICE_CREATEBUFFER,
    DEVICE_ISVALIDBUFFER,
    DEVICE_BUFFERDATA,
    DEVICE_GETBUFFER,
    DEVICE_DESTROYBUFFER,
    DEVICE_CLOSE,
  };

  if( objc < 2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
    return TCL_ERROR;
  }

  if( Tcl_GetIndexFromObj(interp, objv[1], DEVICE_strs, "option", 0, &choice) ){
    return TCL_ERROR;
  }

  switch( (enum DEVICE_enum)choice ){

    /*
     * Setup a listener property
     */
    case DEVICE_LISTENER_SET: {
      Tcl_Obj *objPtr = NULL;
      const char *zArg = NULL;
      double value1 = 0.0, value2 = 0.0, value3 = 0.0;
      ALfloat listenerOri[6];
      int count = 0, i = 0;
      ALCenum error;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "property setting ");
        return TCL_ERROR;
      }

      zArg = Tcl_GetStringFromObj(objv[2], 0);
      if(strcmp(zArg, "AL_GAIN")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
             return TCL_ERROR;
         }

         alListenerf(AL_GAIN, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_POSITION")==0) {
         if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
            return TCL_ERROR;
         }

         if(count != 3) {
            Tcl_AppendResult(interp, "AL_POSITION: parameter number is wrong!", (char*)0);
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 0, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 1, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value2) != TCL_OK) {
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 2, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value3) != TCL_OK) {
            return TCL_ERROR;
         }

         alListener3f(AL_POSITION, (ALfloat) value1, (ALfloat) value2,
                      (ALfloat) value3);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_POSITION fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_VELOCITY")==0) {
         if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
            return TCL_ERROR;
         }

         if(count != 3) {
            Tcl_AppendResult(interp, "AL_VELOCITY: parameter number is wrong!", (char*)0);
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 0, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 1, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value2) != TCL_OK) {
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 2, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value3) != TCL_OK) {
            return TCL_ERROR;
         }

         alListener3f(AL_VELOCITY, (ALfloat) value1, (ALfloat) value2,
                      (ALfloat) value3);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_VELOCITY fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_ORIENTATION")==0) {
         if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
            return TCL_ERROR;
         }

         if(count != 6) {
            Tcl_AppendResult(interp, "AL_ORIENTATION: parameter number is wrong!", (char*)0);
            return TCL_ERROR;
         }

         for(i = 0; i < 6; i++) {
            Tcl_ListObjIndex(interp, objv[3], i, &objPtr);
            if(Tcl_GetDoubleFromObj(interp, objPtr, &value1) != TCL_OK) {
                return TCL_ERROR;
            }
            listenerOri[i] = (ALfloat) value1;
         }

         alListenerfv(AL_ORIENTATION, listenerOri);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_VELOCITY fail!", (char*)0);
           return TCL_ERROR;
         }
      } else {
         Tcl_AppendResult(interp, "setListener: Unknown OpenAL property", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Retrieve a listener property
     */
    case DEVICE_LISTENER_GET: {
      Tcl_Obj *return_obj = NULL;
      const char *zArg = NULL;
      ALfloat value1 = 0.0, value2 = 0.0, value3 = 0.0;
      ALfloat listenerOri[6];
      ALCenum error;
      int i = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "property");
        return TCL_ERROR;
      }

      zArg = Tcl_GetStringFromObj(objv[2], 0);
      if(strcmp(zArg, "AL_GAIN")==0) {
         alGetListenerf(AL_GAIN, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_POSITION")==0) {
         alGetListener3f(AL_POSITION, &value1, &value2, &value3);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_POSITION fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewListObj(3, NULL);
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value1));
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value2));
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value3));
      } else if(strcmp(zArg, "AL_VELOCITY")==0) {
         alGetListener3f(AL_VELOCITY, &value1, &value2, &value3);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_VELOCITY fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewListObj(3, NULL);
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value1));
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value2));
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value3));
      } else if(strcmp(zArg, "AL_ORIENTATION")==0) {
         alGetListenerfv(AL_ORIENTATION, listenerOri);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_ORIENTATION fail!", (char*)0);
           return TCL_ERROR;
         }

         /*
          * ORIENTATION is a pair of 3-tuples representing the 'at' direction
          * vector and 'up' direction of the Object in Cartesian space
          */
         return_obj = Tcl_NewListObj(0, NULL);
         for(i = 0; i <  6; i++) {
           Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) listenerOri[i]));
         }
      } else {
         Tcl_AppendResult(interp, "getListener: Unknown OpenAL property", (char*)0);
         return TCL_ERROR;
      }

      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    /*
     * Generate a source to use
     */
    case DEVICE_CREATESOURCE: {
      ALCenum error;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alGenSources((ALuint)1, &pDevice->source);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alGenSources fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Verify a handle is a valid Source
     */
    case DEVICE_ISVALIDSOURCE: {
      Tcl_Obj *return_obj = NULL;
      ALboolean result;
      ALCenum error;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      result = alIsSource(pDevice->source);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alIsSource fail!", (char*)0);
         return TCL_ERROR;
      }

      if(result == AL_FALSE) {
         return_obj = Tcl_NewBooleanObj(0);
      } else {
         return_obj = Tcl_NewBooleanObj(1);
      }

      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    /*
     * Set an OpenAL source property
     */
    case DEVICE_SETSOURCE: {
      Tcl_Obj *objPtr = NULL;
      const char *zArg = NULL;
      const char *state;
      double value1 = 0.0, value2 = 0.0, value3 = 0.0;
      ALfloat listenerOri[6];
      int count = 0, i = 0;
      int len = 0;
      int isLooping = 0, relative = 0;
      ALCenum error;

      if( objc != 4 ){
        Tcl_WrongNumArgs(interp, 2, objv, "property setting ");
        return TCL_ERROR;
      }

      zArg = Tcl_GetStringFromObj(objv[2], 0);
      if(strcmp(zArg, "AL_BUFFER")==0) {
         /*
          * Give the index of our buffer
          */
         if(Tcl_GetIntFromObj(interp, objv[3], &relative) != TCL_OK) {
            return TCL_ERROR;
         }

         if( relative < 0) {
            Tcl_AppendResult(interp, "Error: index needs >= 0!", (char*)0);
            return TCL_ERROR;
         }

         if(relative >= pDevice->buffer_index) {
            Tcl_AppendResult(interp, "Error: index out of range!", (char*)0);
            return TCL_ERROR;
         }

         alSourcei(pDevice->source, AL_BUFFER, pDevice->buffer[relative]);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_BUFFER fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_LOOPING")==0) {
         if(Tcl_GetBooleanFromObj(interp, objv[3], &isLooping) != TCL_OK) {
            return TCL_ERROR;
         }

         if(isLooping) {
             alSourcei(pDevice->source, AL_LOOPING, AL_TRUE);
         } else {
             alSourcei(pDevice->source, AL_LOOPING, AL_FALSE);
         }

         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_LOOPING fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_SOURCE_RELATIVE")==0) {
         if(Tcl_GetBooleanFromObj(interp, objv[3], &relative) != TCL_OK) {
            return TCL_ERROR;
         }

         if(relative) {
             alSourcei(pDevice->source, AL_SOURCE_RELATIVE, AL_TRUE);
         } else {
             alSourcei(pDevice->source, AL_SOURCE_RELATIVE, AL_FALSE);
         }

         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_SOURCE_RELATIVE fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_SOURCE_STATE")==0) {
         state = Tcl_GetStringFromObj(objv[3], &len);
         if(!state || len < 1) {
           Tcl_AppendResult(interp, "AL_SOURCE_STATE: string is empty!", (char*)0);
           return TCL_ERROR;
         }

         if(strcmp(state, "AL_INITIAL")==0) {
             alSourcei(pDevice->source, AL_SOURCE_STATE, AL_INITIAL);
         } else if(strcmp(state, "AL_PLAYING")==0) {
             alSourcei(pDevice->source, AL_SOURCE_STATE, AL_PLAYING);
         } else if(strcmp(state, "AL_PAUSED")==0) {
             alSourcei(pDevice->source, AL_SOURCE_STATE, AL_PAUSED);
         } else if(strcmp(state, "AL_STOPPED")==0) {
             alSourcei(pDevice->source, AL_SOURCE_STATE, AL_STOPPED);
         } else {
             Tcl_AppendResult(interp, "AL_SOURCE_STATE: no valid state!", (char*)0);
             return TCL_ERROR;
         }

         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_SOURCE_STATE fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_PITCH")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_PITCH, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_PITCH fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_GAIN")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_GAIN, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_MIN_GAIN")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_MIN_GAIN, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_MIN_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_MAX_GAIN")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_MAX_GAIN, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_MAX_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_MAX_DISTANCE")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_MAX_DISTANCE, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_MAX_DISTANCE fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_ROLLOFF_FACTOR")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_ROLLOFF_FACTOR, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_ROLLOFF_FACTOR fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_CONE_OUTER_GAIN")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_CONE_OUTER_GAIN, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_CONE_OUTER_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_CONE_INNER_ANGLE")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_CONE_INNER_ANGLE, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_CONE_INNER_ANGLE fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_CONE_OUTER_ANGLE")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_CONE_OUTER_ANGLE, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_CONE_OUTER_ANGLE fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_REFERENCE_DISTANCE")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_REFERENCE_DISTANCE, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_REFERENCE_DISTANCE fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_SEC_OFFSET")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_SEC_OFFSET, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_SEC_OFFSET fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_SAMPLE_OFFSET")==0) {
         if(Tcl_GetDoubleFromObj(interp, objv[3], &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         alSourcef(pDevice->source, AL_SAMPLE_OFFSET, (ALfloat) value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_SAMPLE_OFFSET fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_POSITION")==0) {
         if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
            return TCL_ERROR;
         }

         if(count != 3) {
            Tcl_AppendResult(interp, "AL_POSITION: parameter number is wrong!", (char*)0);
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 0, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 1, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value2) != TCL_OK) {
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 2, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value3) != TCL_OK) {
            return TCL_ERROR;
         }

         alSource3f(pDevice->source, AL_POSITION, (ALfloat) value1, (ALfloat) value2,
                      (ALfloat) value3);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_POSITION fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_VELOCITY")==0) {
         if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
            return TCL_ERROR;
         }

         if(count != 3) {
            Tcl_AppendResult(interp, "AL_VELOCITY: parameter number is wrong!", (char*)0);
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 0, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value1) != TCL_OK) {
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 1, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value2) != TCL_OK) {
            return TCL_ERROR;
         }

         Tcl_ListObjIndex(interp, objv[3], 2, &objPtr);
         if(Tcl_GetDoubleFromObj(interp, objPtr, &value3) != TCL_OK) {
            return TCL_ERROR;
         }

         alSource3f(pDevice->source, AL_VELOCITY, (ALfloat) value1, (ALfloat) value2,
                      (ALfloat) value3);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_VELOCITY fail!", (char*)0);
           return TCL_ERROR;
         }
      } else if(strcmp(zArg, "AL_ORIENTATION")==0) {
         if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
            return TCL_ERROR;
         }

         if(count != 6) {
            Tcl_AppendResult(interp, "AL_ORIENTATION: parameter number is wrong!", (char*)0);
            return TCL_ERROR;
         }

         for(i = 0; i < 6; i++) {
            Tcl_ListObjIndex(interp, objv[3], i, &objPtr);
            if(Tcl_GetDoubleFromObj(interp, objPtr, &value1) != TCL_OK) {
                return TCL_ERROR;
            }
            listenerOri[i] = (ALfloat) value1;
         }

         alSourcefv(pDevice->source, AL_ORIENTATION, listenerOri);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Set AL_VELOCITY fail!", (char*)0);
           return TCL_ERROR;
         }
      } else {
         Tcl_AppendResult(interp, "setSource: Unknown OpenAL property", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Retrieve an OpenAL source property
     */
    case DEVICE_GETSOURCE: {
      Tcl_Obj *return_obj = NULL;
      const char *zArg = NULL;
      ALfloat value1 = 0.0, value2 = 0.0, value3 = 0.0;
      ALfloat listenerOri[6];
      ALint source_state;
      ALint relative;
      ALint processed, queued;
      ALCenum error;
      int i = 0;

      if( objc != 3 ){
        Tcl_WrongNumArgs(interp, 2, objv, "property");
        return TCL_ERROR;
      }

      zArg = Tcl_GetStringFromObj(objv[2], 0);
      if(strcmp(zArg, "AL_SOURCE_RELATIVE")==0) {
         alGetSourcei(pDevice->source, AL_SOURCE_RELATIVE, &relative );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_SOURCE_RELATIVE fail!", (char*)0);
           return TCL_ERROR;
         }

         if(relative == AL_FALSE) {
           return_obj = Tcl_NewBooleanObj(0);
         } else {
           return_obj = Tcl_NewBooleanObj(1);
         }
      } else if(strcmp(zArg, "AL_SOURCE_STATE")==0) {
         alGetSourcei(pDevice->source, AL_SOURCE_STATE, &source_state );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_SOURCE_STATE fail!", (char*)0);
           return TCL_ERROR;
         }

         if(source_state==AL_INITIAL) {
             return_obj = Tcl_NewStringObj((char *) "AL_INITIAL", -1);
         } else if(source_state==AL_PLAYING) {
             return_obj = Tcl_NewStringObj((char *) "AL_PLAYING", -1);
         } else if(source_state==AL_PAUSED) {
             return_obj = Tcl_NewStringObj((char *) "AL_PAUSED", -1);
         } else if(source_state==AL_STOPPED) {
             return_obj = Tcl_NewStringObj((char *) "AL_STOPPED", -1);
         }
      } else if(strcmp(zArg, "AL_PITCH")==0) {
         alGetSourcef(pDevice->source, AL_PITCH, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_PITCH fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_GAIN")==0) {
         alGetSourcef(pDevice->source, AL_GAIN, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_MIN_GAIN")==0) {
         alGetSourcef(pDevice->source, AL_MIN_GAIN, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_MIN_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_MAX_GAIN")==0) {
         alGetSourcef(pDevice->source, AL_MAX_GAIN, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_MAX_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_MAX_DISTANCE")==0) {
         alGetSourcef(pDevice->source, AL_MAX_DISTANCE, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_MAX_DISTANCE fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_ROLLOFF_FACTOR")==0) {
         alGetSourcef(pDevice->source, AL_ROLLOFF_FACTOR, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_ROLLOFF_FACTOR fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_CONE_OUTER_GAIN")==0) {
         alGetSourcef(pDevice->source, AL_CONE_OUTER_GAIN, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_CONE_OUTER_GAIN fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_CONE_INNER_ANGLE")==0) {
         alGetSourcef(pDevice->source, AL_CONE_INNER_ANGLE, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_CONE_INNER_ANGLE fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_CONE_OUTER_ANGLE")==0) {
         alGetSourcef(pDevice->source, AL_CONE_OUTER_ANGLE, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_CONE_OUTER_ANGLE fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_REFERENCE_DISTANCE")==0) {
         alGetSourcef(pDevice->source, AL_REFERENCE_DISTANCE, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_REFERENCE_DISTANCE fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_BUFFERS_PROCESSED")==0) {
         alGetSourcei(pDevice->source, AL_BUFFERS_PROCESSED, &processed );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_BUFFERS_PROCESSED fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewIntObj((int) processed);
      } else if(strcmp(zArg, "AL_BUFFERS_QUEUED")==0) {
         alGetSourcei(pDevice->source, AL_BUFFERS_QUEUED, &queued );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_BUFFERS_QUEUED fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewIntObj((int) queued);
      } else if(strcmp(zArg, "AL_SEC_OFFSET")==0) {
         alGetSourcef(pDevice->source, AL_SEC_OFFSET, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_SEC_OFFSET fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_SAMPLE_OFFSET")==0) {
         alGetSourcef(pDevice->source, AL_SAMPLE_OFFSET, &value1 );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_SAMPLE_OFFSET fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewDoubleObj((double) value1);
      } else if(strcmp(zArg, "AL_POSITION")==0) {
         alGetSource3f(pDevice->source, AL_POSITION, &value1, &value2, &value3);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_POSITION fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewListObj(3, NULL);
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value1));
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value2));
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value3));
      } else if(strcmp(zArg, "AL_VELOCITY")==0) {
         alGetSource3f(pDevice->source, AL_VELOCITY, &value1, &value2, &value3);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_VELOCITY fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewListObj(3, NULL);
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value1));
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value2));
         Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) value3));
      } else if(strcmp(zArg, "AL_ORIENTATION")==0) {
         alGetSourcefv(pDevice->source, AL_ORIENTATION, listenerOri);
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_ORIENTATION fail!", (char*)0);
           return TCL_ERROR;
         }

         /*
          * ORIENTATION is a pair of 3-tuples representing the 'at' direction
          * vector and 'up' direction of the Object in Cartesian space
          */
         return_obj = Tcl_NewListObj(0, NULL);
         for(i = 0; i <  6; i++) {
           Tcl_ListObjAppendElement(interp, return_obj, Tcl_NewDoubleObj((double) listenerOri[i]));
         }
      } else {
         Tcl_AppendResult(interp, "getSource: Unknown OpenAL property", (char*)0);
         return TCL_ERROR;
      }

      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    /*
     * This function queues a set of buffers attached to a source
     */
    case DEVICE_QUEUE_BUFFER: {
      ALCenum error;
      int number = 1;

      if( objc != 2 && objc != 3){
        Tcl_WrongNumArgs(interp, 2, objv, "?number?");
        return TCL_ERROR;
      }

      if(objc==3) {
        if(Tcl_GetIntFromObj(interp, objv[2], &number) != TCL_OK) {
           return TCL_ERROR;
        }

        if(number <= 0) {
           Tcl_AppendResult(interp, "Error: buffer number needs > 0!", (char*)0);
           return TCL_ERROR;
        }
      }

      alSourceQueueBuffers(pDevice->source, number, pDevice->buffer);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alSourceQueueBuffers fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * This function unqueues a set of buffers attached to a source
     */
    case DEVICE_UNQUEUE_BUFFER: {
      ALCenum error;
      int number = 1;

      if( objc != 2 && objc != 3){
        Tcl_WrongNumArgs(interp, 2, objv, "?number?");
        return TCL_ERROR;
      }

      if(objc==3) {
        if(Tcl_GetIntFromObj(interp, objv[2], &number) != TCL_OK) {
           return TCL_ERROR;
        }

        if(number <= 0) {
           Tcl_AppendResult(interp, "Error: buffer number needs > 0!", (char*)0);
           return TCL_ERROR;
        }
      }

      alSourceUnqueueBuffers(pDevice->source, number, pDevice->buffer);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alSourceUnqueueBuffers fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Play a source
     */
    case DEVICE_PLAYSOURCE: {
      ALCenum error;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alSourcePlay(pDevice->source);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alSourcePlay fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Pause a source
     */
    case DEVICE_PAUSESOURCE: {
      ALCenum error;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alSourcePause(pDevice->source);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alSourcePause fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Rewind a source
     */
    case DEVICE_REWINDSOURCE: {
      ALCenum error;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alSourceRewind(pDevice->source);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alSourceRewind fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Stop a source
     */
    case DEVICE_STOPSOURCE: {
      ALCenum error;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alSourceStop(pDevice->source);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alSourceStop fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Destroy a source
     */
    case DEVICE_DESTROYSOURCE: {
      ALCenum error;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alDeleteSources((ALuint)1, &pDevice->source);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alDeleteSources fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Generate a buffer to use
     */
    case DEVICE_CREATEBUFFER: {
      ALCenum error;
      int number = 1;

      if( objc != 2 && objc != 3){
        Tcl_WrongNumArgs(interp, 2, objv, "?number?");
        return TCL_ERROR;
      }

      if(objc==3) {
        if(Tcl_GetIntFromObj(interp, objv[2], &number) != TCL_OK) {
           return TCL_ERROR;
        }

        if(number <= 0) {
           Tcl_AppendResult(interp, "Error: buffer number needs > 0!", (char*)0);
           return TCL_ERROR;
        }
      }

      pDevice->buffer_index = number;
      pDevice->buffer = (ALuint *) malloc (pDevice->buffer_index * sizeof(ALuint));
      alGenBuffers((ALuint) pDevice->buffer_index, pDevice->buffer);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alGenBuffers fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Check buffer is valid
     */
    case DEVICE_ISVALIDBUFFER: {
      Tcl_Obj *return_obj = NULL;
      ALboolean result;
      ALCenum error;
      int index = 0;

      if( objc != 2 && objc != 3){
        Tcl_WrongNumArgs(interp, 2, objv, "?index?");
        return TCL_ERROR;
      }

      if(objc==3) {
        if(Tcl_GetIntFromObj(interp, objv[2], &index) != TCL_OK) {
           return TCL_ERROR;
        }

        if(index < 0) {
           Tcl_AppendResult(interp, "Error: index needs >= 0!", (char*)0);
           return TCL_ERROR;
        }
      }

      if(index >= pDevice->buffer_index) {
         Tcl_AppendResult(interp, "Error: index out of range!", (char*)0);
         return TCL_ERROR;
      }

      result = alIsBuffer(pDevice->buffer[index]);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alIsBuffer fail!", (char*)0);
         return TCL_ERROR;
      }

      if(result == AL_FALSE) {
         return_obj = Tcl_NewBooleanObj(0);
      } else {
         return_obj = Tcl_NewBooleanObj(1);
      }

      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    /*
     * Load a buffer with data
     */
    case DEVICE_BUFFERDATA: {
      ALCenum error;
      ALenum alFormat = AL_FORMAT_STEREO16;
      const char *format = NULL;
      const unsigned char *byteArray = NULL;
      int len, bytelength;
      int samplerate;
      int index = 0;

      if( objc != 5 && objc != 6 ){
        Tcl_WrongNumArgs(interp, 2, objv, "format data samplerate ?index? ");
        return TCL_ERROR;
      }

      if(objc==6) {
        if(Tcl_GetIntFromObj(interp, objv[5], &index) != TCL_OK) {
           return TCL_ERROR;
        }

        if(index < 0) {
           Tcl_AppendResult(interp, "Error: index needs >= 0!", (char*)0);
           return TCL_ERROR;
        }
      }

      if(index >= pDevice->buffer_index) {
         Tcl_AppendResult(interp, "Error: index out of range!", (char*)0);
         return TCL_ERROR;
      }

      format = Tcl_GetStringFromObj(objv[2], &len);
      if(!format || len < 1) {
         Tcl_AppendResult(interp, "Error: format is empty string!", (char*)0);
         return TCL_ERROR;
      }

      if(strcmp(format, "AL_FORMAT_MONO8")==0) {
          alFormat = AL_FORMAT_MONO8;
      } else if(strcmp(format, "AL_FORMAT_MONO16")==0) {
          alFormat = AL_FORMAT_MONO16;
      } else if(strcmp(format, "AL_FORMAT_STEREO8")==0) {
          alFormat = AL_FORMAT_STEREO8;
      } else if(strcmp(format, "AL_FORMAT_STEREO16")==0) {
          alFormat = AL_FORMAT_STEREO16;
      } else {
          alFormat = AL_FORMAT_STEREO16;
      }

      byteArray = Tcl_GetByteArrayFromObj(objv[3], &bytelength);
      if(!byteArray || bytelength < 1) {
         Tcl_AppendResult(interp, "Error: data is empty string!", (char*)0);
         return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[4], &samplerate) != TCL_OK) {
         return TCL_ERROR;
      }

      if( samplerate <= 0) {
         Tcl_AppendResult(interp, "Error: samplerate needs > 0!", (char*)0);
         return TCL_ERROR;
      }

      alBufferData(pDevice->buffer[index], alFormat, (unsigned char *) byteArray,
                   bytelength, samplerate);
      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alBufferData fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Get a buffer property
     */
    case DEVICE_GETBUFFER: {
      Tcl_Obj *return_obj = NULL;
      const char *zArg = NULL;
      ALint value;
      ALCenum error;
      int index = 0;

      if( objc != 3 && objc != 4){
        Tcl_WrongNumArgs(interp, 2, objv, "property ?index? ");
        return TCL_ERROR;
      }

      if(objc==4) {
        if(Tcl_GetIntFromObj(interp, objv[3], &index) != TCL_OK) {
           return TCL_ERROR;
        }

        if(index < 0) {
           Tcl_AppendResult(interp, "Error: index needs >= 0!", (char*)0);
           return TCL_ERROR;
        }
      }

      if(index >= pDevice->buffer_index) {
         Tcl_AppendResult(interp, "Error: index out of range!", (char*)0);
         return TCL_ERROR;
      }

      zArg = Tcl_GetStringFromObj(objv[2], 0);
      if(strcmp(zArg, "AL_SIZE")==0) {
         alGetBufferi(pDevice->buffer[index], AL_SIZE, &value );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_SIZE fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewIntObj((int) value);
      } else if(strcmp(zArg, "AL_CHANNELS")==0) {
         alGetBufferi(pDevice->buffer[index], AL_CHANNELS, &value );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_CHANNELS fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewIntObj((int) value);
      } else if(strcmp(zArg, "AL_BITS")==0) {
         alGetBufferi(pDevice->buffer[index], AL_BITS, &value );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_BITS fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewIntObj((int) value);
      } else if(strcmp(zArg, "AL_FREQUENCY")==0) {
         alGetBufferi(pDevice->buffer[index], AL_FREQUENCY, &value );
         error = alGetError();
         if (error != AL_NO_ERROR) {
           Tcl_AppendResult(interp, "Get AL_FREQUENCY fail!", (char*)0);
           return TCL_ERROR;
         }

         return_obj = Tcl_NewIntObj((int) value);
      } else {
         Tcl_AppendResult(interp, "getBuffer: Unknown OpenAL property", (char*)0);
         return TCL_ERROR;
      }

      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    /*
     * Destroy a buffer
     */
    case DEVICE_DESTROYBUFFER: {
      ALCenum error;

      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alDeleteBuffers((ALuint) pDevice->buffer_index, pDevice->buffer);

      /*
       * Free memory
       */
      if(pDevice->buffer) free(pDevice->buffer);
      pDevice->buffer = NULL;
      pDevice->buffer_index = 0;

      error = alGetError();
      if (error != AL_NO_ERROR) {
         Tcl_AppendResult(interp, "alDeleteBuffers fail!", (char*)0);
         return TCL_ERROR;
      }

      break;
    }

    /*
     * Close function
     */
    case DEVICE_CLOSE: {
      if( objc != 2 ){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      alcMakeContextCurrent(NULL);
      alcDestroyContext(pDevice->context);
      alcCloseDevice(pDevice->device);

      Tcl_Free((char *)pDevice);
      pDevice = NULL;

      Tcl_DeleteCommand(interp, Tcl_GetStringFromObj(objv[0], 0));
      break;
    }

  } /* End of the SWITCH statement */

  return rc;
}


int DeviceMain(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv)
{
  DeviceData *p;
  const char *zArg = NULL;
  const char *devicename = NULL;
  int len = 0;
  int i = 0;

  if( objc < 2 || (objc&1)!=0 ){
    Tcl_WrongNumArgs(interp, 1, objv, "HANDLE ?-device name? ");
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
    } else {
      Tcl_AppendResult(interp, "unknown option: ", zArg, (char*)0);
      return TCL_ERROR;
    }
  }

  p = (DeviceData *)Tcl_Alloc( sizeof(*p) );
  if( p==0 ){
    Tcl_SetResult(interp, (char *)"malloc failed", TCL_STATIC);
    return TCL_ERROR;
  }

  memset(p, 0, sizeof(*p));
  p->interp = interp;
  p->source = 0;
  p->buffer = NULL;
  p->buffer_index = 0;

  /*
   * User does not specify a device name, so get the default device
   */
  if(!devicename) {
    devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
  }

  p->device = alcOpenDevice(devicename);
  if (!p->device) {
     alcCloseDevice(p->device);
     Tcl_Free((char *) p);

     Tcl_AppendResult(interp, "Error: alcOpenDevice open fail!", (char*)0);
     return TCL_ERROR;
  }

  p->context = alcCreateContext(p->device, NULL);
  if (!alcMakeContextCurrent(p->context)) {
     alcCloseDevice(p->device);
     Tcl_Free((char *) p);

     Tcl_AppendResult(interp, "Failed to make default context!", (char*)0);
     return TCL_ERROR;
  }

  zArg = Tcl_GetStringFromObj(objv[1], 0);
  Tcl_CreateObjCommand(interp, zArg, DeviceObjCmd, (char*)p, (Tcl_CmdDeleteProc *)NULL);

  return TCL_OK;
}


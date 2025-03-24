#include "DebugLogging.h"

    bool logToFile(const char *message ){
        SceUID logFile = sceIoOpen("ux0:/data/debug.log", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND,0777);
        if (logFile >= 0){
            sceIoWrite(logFile,message,sceClibStrnlen(message,512));
            sceIoWrite(logFile,"\n",1);
            sceIoClose(logFile);
            return true;
        }
        else{
            sceClibPrintf("[ERROR]: Failed to open log file \n");
        
        }
        return false;
    }


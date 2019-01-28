/********************************************************
 * Creatint for practice, free learning purpose.
 * 
 * filename: Demo.c
 * author: Martin
 * date: Thu Jan 24 2019
 * description: Get devices informations using portaudio
 * 
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"


int main(int argc, char *argv[])
{
    PaError err = paNoError;
    int i = 0;
    printf("\n===== Get device informations =====\n");
    err = Pa_Initialize();
    if( err != paNoError ) goto done;

    PaDeviceIndex index = Pa_GetDeviceCount();//返回设备数量
    if (index < 0) {
        printf("There is no device.\n");
        goto done;
    }

    printf("\nDevices sum numbers: %d\n", index);
    for (i = 0; i < index; i++) {
        PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        printf("Device NO.%d\n", i);
        printf("1.Device name: %s\n", deviceInfo->name);
        printf("2.Device maxInputChannels: %d\n", deviceInfo->maxInputChannels);
        printf("3.Device maxOutputChannels: %d\n", deviceInfo->maxOutputChannels);
        printf("4.Device defaultSampleRate: %f\n", deviceInfo->defaultSampleRate);
        printf("5.Device defaultLowInputLatency: %f\n", deviceInfo->defaultLowInputLatency);
        printf("6.Device defaultLowOutputLatency: %f\n", deviceInfo->defaultLowOutputLatency);
        printf("7.Device defaultHighInputLatency: %f\n", deviceInfo->defaultHighInputLatency);
        printf("8.Device defaultHighOutputLatency: %f\n\n", deviceInfo->defaultHighOutputLatency);
    }

    PaDeviceIndex defaultInputDeviceIndex = Pa_GetDefaultInputDevice();
    if (defaultInputDeviceIndex < 0) {
        printf("There is no default input device.\n");
    }
    else {
        printf("===== Default input device informations =====\n");
        PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(defaultInputDeviceIndex);
        printf("Default input device name: %s\n", deviceInfo->name);
        printf("Default input device maxInputChannels: %d\n", deviceInfo->maxInputChannels);
        printf("Default input device maxOutputChannels: %d\n", deviceInfo->maxOutputChannels);
        printf("Default input device defaultSampleRate: %f\n", deviceInfo->defaultSampleRate);
        printf("Default input device defaultLowInputLatency: %f\n", deviceInfo->defaultLowInputLatency);
        printf("Default input device defaultLowOutputLatency: %f\n", deviceInfo->defaultLowOutputLatency);
        printf("Default input device defaultHighInputLatency: %f\n", deviceInfo->defaultHighInputLatency);
        printf("Default input device defaultHighOutputLatency: %f\n\n", deviceInfo->defaultHighOutputLatency);
    }
    PaDeviceIndex defaultOutputDeviceIndex = Pa_GetDefaultOutputDevice();
    if (defaultOutputDeviceIndex < 0) {
        printf("There is no default output device.\n");
    }
    else {
        printf("===== Default output device informations =====\n");
        PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(defaultOutputDeviceIndex);
        printf("Default output device name: %s\n", deviceInfo->name);
        printf("Default output device maxInputChannels: %d\n", deviceInfo->maxInputChannels);
        printf("Default output device maxOutputChannels: %d\n", deviceInfo->maxOutputChannels);
        printf("Default output device defaultSampleRate: %f\n", deviceInfo->defaultSampleRate);
        printf("Default output device defaultLowInputLatency: %f\n", deviceInfo->defaultLowInputLatency);
        printf("Default output device defaultLowOutputLatency: %f\n", deviceInfo->defaultLowOutputLatency);
        printf("Default output device defaultHighInputLatency: %f\n", deviceInfo->defaultHighInputLatency);
        printf("Default output device defaultHighOutputLatency: %f\n\n", deviceInfo->defaultHighOutputLatency);
    }
    
done:
    //释放资源
    Pa_Terminate();
    return err;
}

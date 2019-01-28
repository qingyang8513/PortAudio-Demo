/********************************************************
 * Creatint for practice, free learning purpose.
 * 
 * filename: Demo.c
 * author: Martin
 * date: Thu Jan 24 2019
 * description: detect user input parameters usning in app
 * 
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <sys/time.h>
#include "portaudio.h"


#define FRAMES_PER_BUFFER   (512)
typedef short SAMPLE;

typedef struct
{
    int            frameIndex;       /* 当前帧指针. */
    int            maxFrameIndex;    /* 最大帧数量. */
    char           *recordedData;    /* 录音音频数据指针. */
    unsigned long  dataLength;       /* 录音数据总长度. */
    char           *fileName;        /* 保持文件名称. */
    int            channels;         /* 通道数. */
    double         sampleRate;       /* 采样率. */
    PaSampleFormat sampleType;       /* 采样数据类型. PaSampleFormat */
    int            dataBits;         /* 单个采样数据长度，单位：bit. */
} paTestData;

/* functions definitions */
static void help(void);
static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );

int main(int argc, char *argv[])
{
    /* option sturct to store input parameters. */
    struct option long_option[] = {
        {"help", 0, NULL, 'h'},
        {"device", 1, NULL, 'D'},
        {"rate", 1, NULL, 'r'},
        {"channel", 0, NULL, 'c'},
        {"format", 1, NULL, 'f'},
        {"second", 1, NULL, 's'},
        {NULL, 0, NULL, 0},
    };
    int    morehelp = 0;
    int    param = 0;
    char   *fileName = NULL;
    char   *deviceName = "default";
    char   *streamDataFormat = "S16_LE";
    double sampleRate = 16000;
    int    channels = 1;
    int    seconds = 5;

    unsigned long sampleType = paInt16;


    PaStreamParameters  inputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    paTestData          data;//自定义数据结构
    int                 i;
    int                 totalFrames;
    int                 numSamples;
    int                 numBytes;
    int                 bits_data;


    if (argc < 2) {
        help();
        return 0;
    }
    
    /* Input detected. */
    while(1){
        int c;
        
        if ((c = getopt_long(argc, argv, "hD:r:f:c:s:", long_option, NULL)) < 0) {
            break;
        }
        
        switch (c)
        {
            case 'h':
                morehelp++;
                break;

            case 'D':
                deviceName = (char *)malloc(strlen(optarg) + 1);
                memset(deviceName, 0, strlen(optarg) + 1);
                memcpy(deviceName, optarg, strlen(optarg));
            break;
            
            case 'r':
                sampleRate = atof(optarg);
                break;

            case 'f':
                streamDataFormat = (char *)malloc(strlen(optarg) + 1);
                memset(streamDataFormat, 0, strlen(optarg) + 1);
                memcpy(streamDataFormat, optarg, strlen(optarg));
            break;

            case 'c':
                channels = atoi(optarg);
                break;
            
            case 's':
                seconds = atoi(optarg);
                break;

            default:
                break;
        }
    }

    /* Display help information. */
    if (morehelp) {
        help();
        return 0;
    }

    /* parameters confirm */
    if (morehelp > 0) {
        if ((argc % 2) == 0) {
            printf("Parameters error.\n\n");
            help();
            return 0;
        }
    }
    else {
        if ((argc % 2) == 1) {
            printf("Parameters error.\n\n");
            help();
            return 0;
        }
    }

    /* get file name */
    fileName = argv[argc - 1];

    /* print parameters */
    printf("\n===== Input parameters =====\n");
    printf("Device name: %s\n", deviceName);
    printf("Sample rate: %d\n", (int)sampleRate);
    printf("Data format: %s\n", streamDataFormat);
    printf("Channels   : %d\n", channels);
    printf("Record time: %ds\n", seconds);
    printf("File name  : %s\n", fileName);
    printf("=============================\n\n");
    fflush(stdout);

    /*  */
    if (!strcmp(streamDataFormat, "U8"))  { sampleType = paUInt8; bits_data = 8; printf("Sample Format: %s, dataBits: %d, paFormat: %ul\n\n", streamDataFormat, bits_data, sampleType); }
    else if (!strcmp(streamDataFormat, "S8")) { sampleType = paInt8; bits_data = 8; printf("Sample Format: %s, dataBits: %d, paFormat: %ul\n\n", streamDataFormat, bits_data, sampleType); }
    else if (!strcmp(streamDataFormat, "S16_LE")) { sampleType = paInt16; bits_data = 16; printf("Sample Format: %s, dataBits: %d, paFormat: %ul\n\n", streamDataFormat, bits_data, sampleType); }
    else if (!strcmp(streamDataFormat, "S24_LE")) { sampleType = paInt24; bits_data = 24; printf("Sample Format: %s, dataBits: %d, paFormat: %ul\n\n", streamDataFormat, bits_data, sampleType); }
    else if (!strcmp(streamDataFormat, "S32_LE")) { sampleType = paInt32; bits_data = 32; printf("Sample Format: %s, dataBits: %d, paFormat: %ul\n\n", streamDataFormat, bits_data, sampleType); }
    else if (!strcmp(streamDataFormat, "F32")) { sampleType = paFloat32; bits_data = sizeof(float) * 8; printf("Sample Format: %s, dataBits: %d, paFormat: %ul\n\n", streamDataFormat, bits_data, sampleType); }
    else { sampleType = paInt16; bits_data = 16; printf("Sample Format: %s, dataBits: %d, paFormat: %ul\n\n", streamDataFormat, bits_data, sampleType); }

    /* Test data settings */
    data.frameIndex = 0;
    data.sampleRate = sampleRate;
    data.channels = channels;
    data.sampleType = sampleType;
    data.dataBits = bits_data;
    data.fileName = fileName;
    data.maxFrameIndex = seconds * data.sampleRate; 
    data.dataLength = data.dataBits / 8 * data.maxFrameIndex * data.channels;
    data.recordedData = malloc( data.dataLength ); 
    if( data.recordedData == NULL )
    {
        printf("Could not allocate record array.\n");
        goto done;
    }
    memset(data.recordedData, 0, data.dataLength);

    printf("===== Test data parameters =====\n");
    printf("frameIndex: %d\n", data.frameIndex);
    printf("maxFrameIndex: %d\n", data.maxFrameIndex);
    printf("channels: %d\n", data.channels);
    printf("sampleType: %ul\n", data.sampleType);
    printf("dataBits: %d\n", data.dataBits);
    printf("sampleRate: %f\n", data.sampleRate);
    printf("fileName: %s\n", data.fileName);
    printf("dataLength: %d\n", data.dataLength);
    printf("recordedData: %ld\n", data.recordedData);
    printf("================================\n\n");

    /* PortAudio initialize and open stream */
    printf("===== PortAudio initialize =====\n");
    err = Pa_Initialize();
    if( err != paNoError ) goto done;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto done;
    }
    else {
        printf("\nDefault input device index: %d\n", inputParameters.device);
    }
    inputParameters.channelCount = data.channels;                    /* stereo input */
    inputParameters.sampleFormat = data.sampleType;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  /* &outputParameters, */
              data.sampleRate,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              recordCallback,
              &data );
    if( err != paNoError ) goto done;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto done;
    printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

    while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
    {
        Pa_Sleep(1000);
        printf("index = %d\n", data.frameIndex ); fflush(stdout);
    }
    if( err < 0 ) goto done;

    err = Pa_CloseStream(stream);
    if(err != paNoError) goto done;

    //存储数据到文件
    printf("\n=== Now writing recorded data of file %s. ===\n", data.fileName); fflush(stdout);
    FILE  *fid;
    fid = fopen(data.fileName, "wb");
    if( fid == NULL )
    {
        printf("Could not open file.\n");
    }
    else
    {
        fwrite( data.recordedData, data.dataBits / 8 * data.channels, data.maxFrameIndex, fid );
        fclose( fid );
        printf("Wrote file over.\n");
        printf("===========================================.\n\n");
    }

done:
    //释放资源
    Pa_Terminate();
    if( data.recordedData ) {
        free( data.recordedData );
    }
    if( err != paNoError )
    {
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        err = 1;
    }
    if (!fileName) {
        free(fileName);
    }
    if (!deviceName) {
        free(deviceName);
    }
    if (!streamDataFormat) {
        free(streamDataFormat);
    }

    return err;
}

static void help(void)
{
    printf("Usage: Demo [OPTION]... [FILE]...\n\n"
    "-h,--help      show help information\n"
    "-D,--device    set device name\n"
    "-r,--rate      frame rate in Hz\n"
    "-f,--format    stream data format\n"
    "               U8 S8 S16_LE S24_LE S32_LE F32\n"
    "-c,--channel   channels to record\n"
    "-s,--second    time to record in second\n\n"
    );
    fflush(stdout);
}

static int recordCallback( const void *inputBuffer,
                           void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    //printf("recordCallback: buffer:%d, framesPerBuffer:%d\n", inputBuffer, framesPerBuffer);

    paTestData *data = (paTestData*)userData;
    const char *rptr = (const char*)inputBuffer;
    //char *dataptr = (char *)data->recordedData;
    char *wptr = &data->recordedData[data->dataBits / 8 * data->frameIndex * data->channels];

    long framesToCalc;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;//剩余空间

    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;

    if( framesLeft < framesPerBuffer )//当剩余空间小于此次录音获取的大小时，返回完成
    {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else
    {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    //录音数据存储
    if( inputBuffer )
    {
        memcpy(wptr, rptr, data->dataBits / 8 * data->channels * framesToCalc);
    }

    //更新录音参数
    data->frameIndex += framesToCalc;
    return finished;
}


# PortAudio-Demo
PortAudio demos running in Raspberry Pi.  
## Environment
  PortAudio[http://portaudio.com/docs/v19-doxydocs/index.html] is a cross-platform, open-source C language library for real-time audio input and output. The library provides functions that allow your software to acquire and output real-time audio streams from your computer's hardware audio interfaces. It is designed to simplify writing cross-platform audio applications, and also to simplify the development of audio software in general by hiding the complexities of dealing directly with each native audio API. PortAudio is used to implement sound recording, editing and mixing applications, software synthesizers, effects processors, music players, internet telephony applications, software defined radios and more. Supported platforms include MS Windows, Mac OS X and Linux. Third-party language bindings make it possible to call PortAudio from other programming languages including Java, C++, C#, Python, PureBasic, FreePascal and Lazarus.   
  This demos are tested in Raspberry Pi 3B+. Every part is running in the environment normally. "lib" forld includes 'libportaudio.a', built in Raspberry Pi environment. If testing in other conditions, you need to download source code of portaudio and compile in specific environment.  
## Usage
  cd demo forld, use cmake to produce Makefile, and then use make commond to compile and build the target.  
## Content
### paGetDeviceInfo
  This demo used to get device informations in you hardware, displays detials including all devices, default input device and default output device.  
### paRecord
   his demo used to record audio and saved to a file, using some cmd below.  
'''  
Usage: Demo [OPTION]... [FILE]...  
  
-h,--help      show help information  
-D,--device    set device name  
-r,--rate      frame rate in Hz  
-f,--format    stream data format  
               U8 S8 S16_LE S24_LE S32_LE F32  
-c,--channel   channels to record  
-s,--second    time to record in second  
'''  

/*
Copyright (C) 2009 Grame

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __JackBoomerDriver__
#define __JackBoomerDriver__

#include "JackAudioDriver.h"
#include "JackPlatformPlug.h"
#include "ringbuffer.h"
//#include "JackThread.h"

namespace Jack
{

typedef jack_default_audio_sample_t jack_sample_t;

#define OSS_DRIVER_N_PARAMS	13
#define OSS_DRIVER_DEF_DEV	"/dev/dsp"
#define OSS_DRIVER_DEF_FS	48000
#define OSS_DRIVER_DEF_BLKSIZE	1024
#define OSS_DRIVER_DEF_NPERIODS	1
#define OSS_DRIVER_DEF_BITS	16
#define OSS_DRIVER_DEF_INS	2
#define OSS_DRIVER_DEF_OUTS	2

/*!
\brief The Boomer driver.
*/

class JackBoomerDriver : public JackAudioDriver, public JackRunnableInterface
{

    enum { kRead = 1, kWrite = 2, kReadWrite = 3 };

    private:
    
        int fInFD;
        int fOutFD;
        
        int fBits;
        int fSampleFormat;
        int fNperiods;
        unsigned int fSampleSize;
        int fRWMode;
        bool fExcl;
        bool fIgnoreHW;
        
        unsigned int fInputBufferSize;
        unsigned int fOutputBufferSize;
        
        void* fInputBuffer;
        void* fOutputBuffer;
        jack_ringbuffer_t** fRingBuffer;
        JackThread fThread;
         
        int OpenInput();
        int OpenOutput();
        int OpenAux();
        void CloseAux();
        void SetSampleFormat();
        void DisplayDeviceInfo();

    public:

        JackBoomerDriver(const char* name, const char* alias, JackLockedEngine* engine, JackSynchro* table)
                : JackAudioDriver(name, alias, engine, table),
                fInFD(-1), fOutFD(-1), fBits(0), 
                fSampleFormat(0), fNperiods(0), fRWMode(0), fExcl(false), fIgnoreHW(true),
                fInputBufferSize(0), fOutputBufferSize(0),
                fInputBuffer(NULL), fOutputBuffer(NULL),
                fRingBuffer(NULL), fThread(this)
       {}

        virtual ~JackBoomerDriver()
        {}

        int Open(jack_nframes_t frames_per_cycle,
                 int user_nperiods, 
                 jack_nframes_t rate,
                 bool capturing,
                 bool playing,
                 int chan_in,
                 int chan_out,
                 bool vmix,
                 bool monitor,
                 const char* capture_driver_name,
                 const char* playback_driver_name,
                 jack_nframes_t capture_latency,
                 jack_nframes_t playback_latency,
                 int bits,
                 bool ignorehwbuf);

        int Close();

        int Start();
        int Stop();

        int Read();
        int Write();

        // BufferSize can be changed
        bool IsFixedBufferSize()
        {
            return false;
        }

        int SetBufferSize(jack_nframes_t buffer_size);
        
        bool Init();
        bool Execute();

};

} // end of namespace

#endif

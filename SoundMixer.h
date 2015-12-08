#pragma once

#include <vector>

#define NOTE_TIE -1
// AGB has 60 FPS based processing
#define AGB_FPS 60
// for increased quality we process in subframes (including the base frame)
#define INTERFRAMES 4
// stereo, so 2 channels
#define N_CHANNELS 2

namespace agbplay
{
    struct ADSR
    {
        ADSR(uint8_t att, uint8_t dec, uint8_t sus, uint8_t rel);
        uint8_t att;
        uint8_t dec;
        uint8_t sus;
        uint8_t rel;
    };

    struct Note
    {
        Note(uint8_t midiKey, uint8_t velocity, int8_t length);
        uint8_t midiKey;
        uint8_t velocity;
        int8_t length;
    };

    class SoundChannel
    {
        public:
            SoundChannel(int8_t *samplePtr, Note note);
            ~SoundChannel();
            SetFreq(float freq);

            int8_t *samplePtr;
            float freq;
            float interPos;

            // DLinkedList of free channels
            SoundChannel *nextFree;
            SoundChannel *prevFree;

            // DLinkedList of used channels
            SoundChannel *nextUsed;
            SoundChannel *prevUsed;
        private:
            uint32_t sampLength;
            bool loopEnabled;
            ADSR env;
            Note note;
    };

    class CGBChannel
    {
        public: 
            CGBChannel();
            ~CGBChannel();
    };

    class SoundMixer
    {
        public:
            SoundMixer(uint32_t sampleRate);
            ~SoundMixer();
            SoundChannel *AllocateChannel();
            void *ProcessAndGetAudio();
            uint32_t GetBufferUnitCount();

        private:
            // channel management
            std::vector<SoundChannel> sndChannels;
            std::vector<CGBChannel> cgbChannels;
            SoundChannel *headFree;
            SoundChannel *headUsed;

            std::vector<float> sampleBuffer;
            uint32_t sampleRate;
            uint32_t samplesPerBuffer;
    };
}

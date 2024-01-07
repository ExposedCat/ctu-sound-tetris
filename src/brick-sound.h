#ifndef BRICKSOUND_H
#define BRICKSOUND_H
#include "main.h"

class SharedData;

class BrickSound : public AudioFilter {
public:
    BrickSound(const pAudioFilter& child, SharedData* data);

    error_type_t do_process(audio_buffer_t& buffer) override;

private:
    double time = 0.0;
    SharedData* data;
    double max_amplitude = 30000.0; // High amplitude for "boom" sound
    double fade_out_rate = 0.01; // Rate at which sound fades out
    double frequency = 100;
};

#endif

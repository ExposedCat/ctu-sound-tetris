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
    double move_frequencies[2] = {300, 600};
    double fall_frequencies[2] = {150, 200};
    double erase_frequencies[2] = {200, 350};
};

#endif

#ifndef BACKGROUND_SOUND_H
#define BACKGROUND_SOUND_H

#include "main.h"

class BackgroundSound : public AudioFilter {
   public:
    BackgroundSound(SharedData* data);

    error_type_t do_process(audio_buffer_t& buffer) override;

   private:
    double time = 0.0;
    SharedData* data;
    double frequencies[10] = {440, 523.25, 587.33, 659.25, 783.99,
                              880, 783.99, 659.25, 587.33, 523.25};
};

#endif
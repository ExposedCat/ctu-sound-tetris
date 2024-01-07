#include "main.h"

#include "background-sound.h"

BackgroundSound::BackgroundSound(SharedData* data)
    : AudioFilter(pAudioFilter()), data(data) {};

error_type_t BackgroundSound::do_process(audio_buffer_t& buffer) {
    for (auto& sample : buffer.data) {
        double time_shift = (data->time / data->time_step);
        double signal =
            sin(data->time * frequencies[(int)data->time % 10] * M_PI * 2);
        sample = static_cast<int16_t>(max_amplitude * signal * 10);
        data->time += data->time_step * data->speed;
    }
    buffer.valid_samples = buffer.data.size();
    return error_type_t::ok;
};
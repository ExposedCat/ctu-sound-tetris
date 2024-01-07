#include "brick-sound.h"

#include "main.h"

BrickSound::BrickSound(const pAudioFilter& child, SharedData* data)
    : AudioFilter(child), data(data) {}

error_type_t BrickSound::do_process(audio_buffer_t& buffer) {
    if (data->last_move_time == -1 ||
        data->time - data->last_move_time > 0.25) {
        return error_type_t::ok;
    }
    double time_shift = 0;
    for (auto& sample : buffer.data) {
        double time = data->time + time_shift - (int)data->time;
        double diff = time - data->last_move_time + (int)data->time;

        double frequency = diff < 0.12 ? 300 : 600;
        double signal = amplitude * (time * frequency * M_PI * 2);
        sample = static_cast<int16_t>(signal) / 10;

        time_shift += data->time_step * data->speed;
    }
    buffer.valid_samples = buffer.data.size();
    return error_type_t::ok;
};
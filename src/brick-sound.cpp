#include "main.h"

#include "brick-sound.h"

BrickSound::BrickSound(const pAudioFilter& child, SharedData* data)
    : AudioFilter(child), data(data) {}

error_type_t BrickSound::do_process(audio_buffer_t& buffer) {
    if(data->last_move_time != -1 && data->time - data->last_move_time <= 0.5){
        double amplitude = max_amplitude;
        for (auto& sample : buffer.data) {
            double new_signal = sin(data->time * 440 * M_PI * 2);
            int16_t new_sample = static_cast<int16_t>(numeric_limits<int16_t>::max() * new_signal);

            sample = new_sample;
            data->time += data->time_step * data->speed;
        }
        buffer.valid_samples = buffer.data.size();
        return error_type_t::ok;
    } else{
        return error_type_t::ok;
    }
};
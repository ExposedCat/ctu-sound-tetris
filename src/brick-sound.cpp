#include "main.h"

#include "brick-sound.h"

BrickSound::BrickSound(const pAudioFilter& child, SharedData* data)
    : AudioFilter(child), data(data) {}

error_type_t BrickSound::do_process(audio_buffer_t& buffer) {
    if(data->brick_moved){
        for (auto& sample : buffer.data) {
            // Generate the sound wave
            double signal = sin(data->time * frequency * M_PI);

            // Apply fade out effect
            max_amplitude *= exp(-data->time * fade_out_rate);

            // Update the sample
            sample = static_cast<int16_t>(max_amplitude * signal);

            // Update time
            data->time += data->time_step * data->speed;
        }
        buffer.valid_samples = buffer.data.size();
        return error_type_t::ok;
    } else{
        for (auto& sample : buffer.data)
			{
				sample = 0;
			}
        return error_type_t::ok;
    }
};
#include "background-sound.h"

#include "main.h"

BackgroundSound::BackgroundSound(SharedData* data)
    : AudioFilter(pAudioFilter()), data(data){};

error_type_t BackgroundSound::do_process(audio_buffer_t& buffer) {
    double time_shift = 0;
    for (auto& sample : buffer.data) {
        double time = data->time + time_shift;
        int frequency_index = (int)time % (data->gameover ? 6 : 10);
        double frequency = data->gameover
                               ? game_over_frequencies[frequency_index]
                               : frequencies[frequency_index];
        double signal = sin(time * frequency * M_PI * 2);
        sample = static_cast<int16_t>(bit8_amplitude * signal * 10);
        time_shift += data->time_step * data->speed;
    }
    buffer.valid_samples = buffer.data.size();
    return error_type_t::ok;
};
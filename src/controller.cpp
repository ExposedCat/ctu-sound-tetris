#include "controller.h"

Controller::Controller(const pAudioFilter& child, WindowData window,
                       SharedData* data)
    : SDLDevice(window.width, window.height,
                "Sound Tetris by Chipi & Chapa Corp."),
      AudioFilter(child),
      window(window),
      data(data) {
    start();
};

Controller::~Controller() { stop(); };

void Controller::draw_sequence() {
    // // Set up our display colors
    // const rgb_t enabled_color = {random_int(25, 255), random_int(25,
    // 255),
    //                              random_int(25, 255)};
    // const rgb_t disabled_color = {20, 20, 20};

    // // What's the size of each sequencer step (width) and of each
    // instrument
    // // row (height)
    // const int step_size = data_.size.width / steps_;
    // const int inst_size = data_.size.height / sound_stages_;

    // // How far we are through the current loop
    // double loop_fraction = time_ / loop_length_;

    // // Draw all rectangles representing each sequencer step + instrument
    // for (int i = 0; i < steps_; ++i) {
    //     for (int j = 0; j < sound_stages_; ++j) {
    //         const rectangle_t rect = {i * step_size, j * inst_size,
    //                                   step_size, inst_size};
    //         draw_rectangle(data_, rect,
    //                        sequence_[i * sound_stages_ + j]
    //                            ? enabled_color
    //                            : disabled_color);
    //     }
    // }

    // Draw a moving cursor to indicate current progress through the loop
    draw_line(
        data_,
        rectangle_t(static_cast<int>(data->time / 10.0 * data_.size.width), 0),
        rectangle_t(static_cast<int>(data->time / 10.0 * data_.size.width),
                    data_.size.height),
        rgb_t(255, 255, 0));
    blit(data_);
};

error_type_t Controller::do_process(audio_buffer_t& buffer) {
    if (is_stopped()) {
        return error_type_t::failed;
    }
    // Not touching the data, simply passing it through
    // But update graphics and control generators

    // Update our loop time (and loop it if appropriate)
    // time_ += buffer.valid_samples * 1.0 /
    //          convert_rate_to_int(buffer.params.rate);
    // if (time_ > loop_length_)
    //     time_ -= loop_length_;
    draw_sequence();

    // // Get our current step and check if we want any generators enabled
    // int cur_step = current_step();
    // if (cur_step != last_step_) {
    //     for (int i = 1; i < sound_stages_; ++i) {
    //         // If we want a generator enabled, enable it, otherwise it
    //         gets
    //         // disabled
    //         shared_ptr<ToggleableFilter> filter =
    //             dynamic_pointer_cast<ToggleableFilter>(get_child(i));
    //         filter->set_enabled(sequence_[cur_step * sound_stages_ + i]);
    //     }

    //     // We only enable/disable at step boundary to save some
    //     processing,
    //     // so we have to remember which step we just processed
    //     last_step_ = cur_step;
    // }

    return error_type_t::ok;
}

bool Controller::do_key_pressed(const int key, bool pressed) {
    if (!pressed) {
        return true;
    }
    switch (key) {
        case keys::key_q: {
            return false;
        }
        case KEY_ARROW_LEFT: {
            data->speed -= 0.5;
            break;
        }
        case KEY_ARROW_RIGHT: {
            data->speed += 0.5;
            break;
        }
    }

    return true;
}
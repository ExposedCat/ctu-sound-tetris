#include <iimavlib/AudioFilter.h>
#include <iimavlib/SDLDevice.h>
#include <iimavlib/Utils.h>
#include <iimavlib/keys.h>
#include <iimavlib/video_ops.h>
#include <iimavlib_high_api.h>

#include <cmath>
#include <iostream>
#include <mutex>
#include <random>

using namespace std;
using namespace iimavlib;

namespace {
const double max_amplitude = 255.0 / 2;

const int KEY_ARROW_LEFT = 276;

const int KEY_ARROW_RIGHT = 275;
}  // namespace

typedef struct SharedData {
    double speed;
    double time;
    double time_step;
} SharedData;

typedef struct WindowData {
    int width;
    int columns;
    int height;
} WindowData;

int random_int(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

class ToggleableFilter {
   public:
    void set_enabled(bool enabled) {
        unique_lock<mutex> lock(enabled_mutex_);
        enabled_ = enabled;
        if (enabled)
            reinitialize();
    }

    inline bool is_enabled() {
        unique_lock<mutex> lock(enabled_mutex_);
        return enabled_;
    }

   private:
    virtual void reinitialize() = 0;

    bool enabled_ = false;
    mutex enabled_mutex_;
};


// TODO: Sync sound speed with bricks falling speed
class BackgroundSound : public AudioFilter {
   public:
    BackgroundSound(SharedData* data)
        : AudioFilter(pAudioFilter()), data(data) {}

    error_type_t do_process(audio_buffer_t& buffer) override {
        for (auto& sample : buffer.data) {
            double time_shift = (data->time / data->time_step);
            double signal =
                sin(data->time * frequencies[(int)data->time % 10] * M_PI * 2);
            sample = static_cast<int16_t>(max_amplitude * signal * 10);
            data->time += data->time_step * data->speed;
        }
        buffer.valid_samples = buffer.data.size();
        return error_type_t::ok;
    }

   private:
    double time = 0.0;
    SharedData* data;
    double frequencies[10] = {440, 523.25, 587.33, 659.25, 783.99,
                              880, 783.99, 659.25, 587.33, 523.25};
};

class Controller : public SDLDevice, public AudioFilter {
   public:
    Controller(const pAudioFilter& child, WindowData window, SharedData* data)
        : SDLDevice(window.width, window.height,
                    "Sound Tetris by Chipi & Chapa Corp."),
          AudioFilter(child),
          window(window),
          data(data) {
        // sequence_.resize(window.columns, false);

        start();
    }

    ~Controller() { stop(); }

   private:
    WindowData window;
    SharedData* data;

    /// Video data
    iimavlib::video_buffer_t data_;
    /// Sequence data
    // vector<bool> sequence_;

    void draw_sequence() {
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
        draw_line(data_,
                  rectangle_t(static_cast<int>(data->time / 10.0 * data_.size.width), 0),
                  rectangle_t(static_cast<int>(data->time / 10.0 * data_.size.width),
                              data_.size.height),
                  rgb_t(255, 255, 0));
        blit(data_);
    }

    // inline int current_step() {
    //     return static_cast<int>(steps_ * time_ / loop_length_);
    // }

    /**
     * Overrides the parent method. This is where we perform the actual actions
     * of enabling/disabling generators. We also trigger a redraw here.
     */
    error_type_t do_process(audio_buffer_t& buffer) override {
        if (is_stopped())
            return error_type_t::failed;
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

    /**
     * Overrides the key press handling method. We have to handle a quit key
     * ourselves (or pass it to the parent class). Otherwise, when a specific
     * key is pressed, toggle a generator at the current sequencer step.
     */
    bool do_key_pressed(const int key, bool pressed) override {
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
};

int main(int argc, char** argv) try {
    audio_id_t device_id = PlatformDevice::default_device();

    if (argc > 1) {
        device_id = simple_cast<audio_id_t>(argv[1]);
    }

    WindowData window;
    window.width = 500;
    window.height = 1000;

    SharedData shared_data;
    shared_data.time = 0.0;
    shared_data.time_step =
        1.0 / convert_rate_to_int(sampling_rate_t::rate_44kHz);
    shared_data.speed = 1.0;

    auto sink = iimavlib::filter_chain<BackgroundSound>(&shared_data)
                    .add<Controller>(window, &shared_data)
                    .add<iimavlib::PlatformSink>(device_id)
                    .sink();

    sink->run();
} catch (exception& error) {
    using namespace iimavlib;
    logger[log_level::fatal]
        << "The application ended unexpectedly with an error: " << error.what();
}
#include "main.h"

#include "background-sound.h"
#include "brick-sound.h"
#include "controller.h"

int main(int argc, char** argv) try {
    audio_id_t device_id = PlatformDevice::default_device();

    if (argc > 1) {
        device_id = simple_cast<audio_id_t>(argv[1]);
    }

    WindowData window;
    window.width = 500;
    window.height = 1000;
    window.rows = 20;
    window.columns = 10;
    window.brick_width = window.width / window.columns;
    window.brick_height = window.height / window.rows;

    SharedData shared_data;
    shared_data.window = window;
    shared_data.time = 0.0;
    shared_data.time_step =
        1.0 / convert_rate_to_int(sampling_rate_t::rate_44kHz);
    shared_data.speed = 2.0;
    shared_data.bricks = {};
    shared_data.last_move_time = -1;

    auto sink = iimavlib::filter_chain<BackgroundSound>(&shared_data)
                    .add<Controller>(&shared_data)
                    .add<BrickSound>(&shared_data)
                    .add<iimavlib::PlatformSink>(device_id)
                    .sink();

    sink->run();
} catch (exception& error) {
    using namespace iimavlib;
    logger[log_level::fatal]
        << "The application ended unexpectedly with an error: " << error.what();
}
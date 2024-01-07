#include "main.h"

#include "background-sound.h"
#include "controller.h"

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
    shared_data.bricks = {};

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
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "main.h"

class Controller : public SDLDevice, public AudioFilter {
   public:
    Controller(const pAudioFilter& child, WindowData window, SharedData* data);

    ~Controller();

   private:
    WindowData window;
    SharedData* data;
    video_buffer_t video_buffer;

  void draw_sequence();

  error_type_t do_process(audio_buffer_t& buffer) override;

  Brick* get_active_brick();

  bool do_key_pressed(const int key, bool pressed) override;
};

#endif
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
    iimavlib::video_buffer_t data_;

  void draw_sequence();

  error_type_t do_process(audio_buffer_t& buffer) override;
  
  bool do_key_pressed(const int key, bool pressed) override;
};

#endif
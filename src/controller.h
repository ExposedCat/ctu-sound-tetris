#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "main.h"

class Controller : public SDLDevice, public AudioFilter {
   public:
    Controller(const pAudioFilter& child, SharedData* data);

    ~Controller();

   private:
    SharedData* data;
    video_buffer_t video_buffer;

  void redraw_screen();

  error_type_t do_process(audio_buffer_t& buffer) override;
  vector<vector<int>> create_matrix();
  void check_gameover(vector<vector<int>> filled_points);
  int check_complete_line(vector<vector<int>> filled_points);
  
  Brick* ensure_active_brick();

  bool do_key_pressed(const int key, bool pressed) override;
};

#endif
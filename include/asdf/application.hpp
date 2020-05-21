#pragma once

#include "base.hpp"
#include "string.hpp"

class App {
    struct Impl;
    Impl *impl = nullptr;

    uint32_t sample_rate = 44100;
    uint32_t buffer_frames = 256;
    uint32_t n_channels = 2;

public:
    App(const String &name);

    void run();
    void destroy();

    virtual void on_load(){};
    virtual void on_update(){};
    virtual void on_destroy(){};

    uint32_t get_sample_rate();
    void get_window_size(int *w, int *h);
    int get_mouse_state(int *x, int *y);

    virtual void audio_stream(
        float *output_buffer,
        float *input_buffer,
        uint32_t n_buffer_frames,
        float stream_time);
};

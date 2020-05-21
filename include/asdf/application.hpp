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

    virtual void load(){};

    virtual void update(){};

    virtual void audio_stream(
        float *output_buffer,
        float *input_buffer,
        uint32_t n_buffer_frames,
        float stream_time);

    virtual void finish(){};

    void run();

    void free();

    uint32_t get_sample_rate();
};

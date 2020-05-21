#include <stdio.h>
#include <RtAudio.h>
#include <asdf/application.hpp>
#include <math.h>

struct MyApp : public App
{
    float global_time = 0;

    MyApp() : App("Synth")
    {
    }

    virtual void load()
    {
        printf("Starting synth\n");
    }

    virtual void audio_stream(
        float *output_buffer,
        float *input_buffer,
        uint32_t n_buffer_frames,
        float stream_time)
    {
        float sample_rate = get_sample_rate();
        float time_step = 1.0f / sample_rate;

        for (uint32_t i = 0; i < n_buffer_frames; i += 1)
        {
            float v = 0.1f * sinf(global_time * 440.0f * 2 * M_PI);

            for (uint32_t j = 0; j < 2; j += 1)
            {
                *output_buffer++ = v;
            }

            global_time += time_step;
        }
    }
};

int main(int argc, const char *argv[])
{
    MyApp app;
    app.run();
    app.free();

    return 0;
}

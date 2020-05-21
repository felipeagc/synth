#include <asdf/application.hpp>

#include <stdio.h>
#include <SDL2/SDL.h>
#include <RtAudio.h>

static int stream_callback(
    void *output_buffer,
    void *input_buffer,
    uint32_t n_buffer_frames,
    double stream_time,
    RtAudioStreamStatus status,
    void *user_data)
{
    App *app = (App *)user_data;

    if (status)
    {
        fprintf(stderr, "Stream underflow detected!");
    }

    app->audio_stream(
        (float *)output_buffer,
        (float *)input_buffer,
        n_buffer_frames,
        (float)stream_time);

    return 0;
}

struct App::Impl
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    RtAudio *audio = nullptr;
};

App::App(const String &name)
{
    impl = new App::Impl;

    SDL_Init(SDL_INIT_VIDEO);

    impl->window = SDL_CreateWindow(
        name.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_SHOWN);

    if (!impl->window)
    {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        exit(1);
    }

    impl->renderer =
        SDL_CreateRenderer(impl->window, -1, SDL_RENDERER_SOFTWARE);

    if (!impl->renderer)
    {
        fprintf(stderr, "Could not create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    try
    {
        impl->audio = new RtAudio();
    } catch (RtAudioError &error)
    {
        error.printMessage();
        exit(1);
    }

    if (impl->audio->getDeviceCount() < 1)
    {
        fprintf(stderr, "No audio devices found!\n");
        exit(1);
    }
}

void App::audio_stream(
    float *output_buffer,
    float *input_buffer,
    uint32_t n_buffer_frames,
    float stream_time)
{
    for (uint32_t i = 0; i < n_buffer_frames; ++i)
    {
        output_buffer[i] = 0.01f;
    }
}

void App::run()
{
    RtAudio::StreamParameters parameters;
    parameters.deviceId = impl->audio->getDefaultOutputDevice();
    parameters.nChannels = this->n_channels;
    parameters.firstChannel = 0;

    try
    {
        impl->audio->openStream(
            &parameters,
            nullptr,
            RTAUDIO_FLOAT32,
            this->sample_rate,
            &this->buffer_frames,
            stream_callback,
            (void *)this);
        impl->audio->startStream();
    } catch (RtAudioError &e)
    {
        e.printMessage();
        exit(0);
    }

    this->load();

    bool should_close = false;
    while (!should_close)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_QUIT: {
                should_close = true;
                break;
            }

            default: break;
            }
        }

        this->update();

        SDL_RenderClear(impl->renderer);
        SDL_RenderPresent(impl->renderer);
    }

    try
    {
        impl->audio->stopStream();
    } catch (RtAudioError &e)
    {
        e.printMessage();
    }

    if (impl->audio->isStreamOpen())
    {
        impl->audio->closeStream();
    }

    this->finish();
}

void App::free()
{
    delete impl->audio;

    SDL_DestroyRenderer(impl->renderer);
    SDL_DestroyWindow(impl->window);
    SDL_Quit();

    delete impl;
}

uint32_t App::get_sample_rate()
{
    return this->sample_rate;
}

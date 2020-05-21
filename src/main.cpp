#include <stdio.h>
#include <SDL2/SDL.h>
#include <RtAudio.h>
#include <asdf/array.hpp>
#include <asdf/application.hpp>
#include <math.h>
#include <atomic>

struct Sound;

typedef float (*Oscillator)(float time, Sound *freq);

struct Sound
{
    float start_time;
    float duration;
    float freq;
    float amplitude;
    Oscillator osc;
};

struct SoundSlot
{
    Sound sound;
    bool active;
};

struct SoundManager
{
    float acc_time = 0.0f;

    SDL_mutex *sounds_mtx;
    Array<SoundSlot> sounds;

    SDL_mutex *available_slots_mtx;
    Array<size_t> available_slots;

    void init()
    {
        sounds_mtx = SDL_CreateMutex();
        available_slots_mtx = SDL_CreateMutex();
    }

    void add_sound(const Sound &sound)
    {
        SDL_LockMutex(sounds_mtx);

        SoundSlot *slot = nullptr;
        SDL_LockMutex(available_slots_mtx);
        if (available_slots.size() >= 1)
        {
            size_t slot_index = available_slots[available_slots.size() - 1];
            available_slots.pop_back();
            slot = &this->sounds[slot_index];
        }
        SDL_UnlockMutex(available_slots_mtx);

        if (!slot)
        {
            sounds.push_back(SoundSlot{});
            slot = &this->sounds[this->sounds.size() - 1];
        }

        slot->active = true;
        slot->sound = sound;

        SDL_UnlockMutex(sounds_mtx);
    }

    void remove_sound(size_t index)
    {
        SDL_LockMutex(sounds_mtx);
        SDL_LockMutex(available_slots_mtx);

        available_slots.push_back(index);
        sounds[index].active = false;

        SDL_UnlockMutex(available_slots_mtx);
        SDL_UnlockMutex(sounds_mtx);
    }

    float tick(float delta_time)
    {
        SDL_LockMutex(sounds_mtx);
        float amplitude = 0.0f;

        for (size_t i = 0; i < sounds.size(); ++i)
        {
            SoundSlot *slot = &sounds[i];
            if (!slot->active) continue;

            Sound *sound = &slot->sound;

            if (sound->start_time >= this->acc_time) continue;

            amplitude += sound->amplitude * sound->osc(this->acc_time, sound);

            sound->duration -= delta_time;
            if (sound->duration <= 0.0f)
            {
                remove_sound(i);
            }
        }
        SDL_UnlockMutex(sounds_mtx);

        this->acc_time += delta_time;
        return amplitude;
    }

    void destroy()
    {
        sounds.destroy();
        available_slots.destroy();
        SDL_DestroyMutex(available_slots_mtx);
        SDL_DestroyMutex(sounds_mtx);
    }
};

static inline float w(float v)
{
    return 2.0f * 3.14159f * v;
}

static inline float sine_wave(float time, Sound *sound)
{
    return sinf(w(time * sound->freq));
}

static inline float square_wave(float time, Sound *sound)
{
    return sinf(w(time * sound->freq)) > 0.0f ? 1.0f : -1.0f;
}

static inline float saw_wave(float time, Sound *sound)
{
    return ((sound->freq * fmod(time, 1.0f / sound->freq)) - 0.5f) * 2.0f;
}

struct MyApp : public App
{
    SoundManager sm;

    MyApp() : App("Synth")
    {
    }

    virtual void on_load()
    {
        this->sm.init();
        printf("Starting synth\n");

        this->sm.add_sound(Sound{
            .start_time = 0.0f,
            .duration = 2.0f,
            .freq = 220.0f,
            .amplitude = 0.1f,
            .osc = saw_wave,
        });

        this->sm.add_sound(Sound{
            .start_time = 2.0f,
            .duration = 2.0f,
            .freq = 330.0f,
            .amplitude = 0.1f,
            .osc = square_wave,
        });

        this->sm.add_sound(Sound{
            .start_time = 0.0f,
            .duration = 4.0f,
            .freq = 220.0f,
            .amplitude = 0.1f,
            .osc = sine_wave,
        });
    }

    virtual void on_destroy()
    {
        this->sm.destroy();
    }

    virtual void audio_stream(
        float *output_buffer,
        float *input_buffer,
        uint32_t n_buffer_frames,
        float stream_time)
    {
        float sample_rate = get_sample_rate();
        float delta_time = 1.0f / sample_rate;

        for (uint32_t i = 0; i < n_buffer_frames; i += 1)
        {
            float v = this->sm.tick(delta_time);

            for (uint32_t j = 0; j < 2; j += 1)
            {
                *output_buffer++ = v;
            }
        }
    }
};

int main(int argc, const char *argv[])
{
    MyApp app;
    app.run();
    app.destroy();

    return 0;
}

#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <spdlog/spdlog.h>

class AudioDevice {
   public:
    static AudioDevice* get_instance();

   private:
    AudioDevice();
    ~AudioDevice();
    static AudioDevice* instance;
    ALCdevice* device;
    ALCcontext* context;
    const ALCchar* default_device_name;
};

class AudioBuffer {
   public:
    static AudioBuffer* get_instance();
    ALuint add_sfx(const char* file_path);
    bool remove_sfx(const ALuint& buffer_id);

   private:
    AudioBuffer();
    ~AudioBuffer();
    static AudioBuffer* instance;
    std::vector<ALuint> buffer_ids;
};
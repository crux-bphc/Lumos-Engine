#include "include/audio.hpp"

AudioDevice* AudioDevice::instance = nullptr;

AudioDevice* AudioDevice::get_instance() {
    if (!instance) {
        instance = new AudioDevice();
    }
    return instance;
}

AudioDevice::AudioDevice() {
    this->device = alcOpenDevice(nullptr);  //. get default device, change
                                            // nullptr to name of device to use
    if (!this->device) {
        spdlog::error("Failed to open audio device");
    }

    this->context = alcCreateContext(this->device, nullptr);  // create context
    if (!this->context) {
        spdlog::error("Failed to create audio context");
    }

    if (!alcMakeContextCurrent(this->context)) {  // set active context
        spdlog::error("Failed to make audio context current");
    }

    this->default_device_name = nullptr;
    if (alcIsExtensionPresent(this->device, "ALC_ENUMERATE_ALL_EXT")) {
        this->default_device_name =
            alcGetString(this->device, ALC_ALL_DEVICES_SPECIFIER);
    }
    if (!this->default_device_name ||
        alcGetError(this->device) != AL_NO_ERROR) {
        this->default_device_name =
            alcGetString(this->device, ALC_DEVICE_SPECIFIER);
    }
    spdlog::info("Default device: {}", this->default_device_name);
}

AudioDevice::~AudioDevice() {
    if (!alcMakeContextCurrent(nullptr)) {
        spdlog::error("Failed to make current audio context null");
    }

    alcDestroyContext(this->context);
    if (this->context) {
        spdlog::error("Failed to destroy audio context");
    }

    if (!alcCloseDevice(this->device)) {
        spdlog::error("Failed to close audio device");
    }
}

AudioBuffer* AudioBuffer::instance = nullptr;
AudioBuffer* AudioBuffer::get_instance() {
    if (!instance) {
        instance = new AudioBuffer();
    }
    return instance;
}

AudioBuffer::AudioBuffer() {
    this->buffer_ids = std::vector<ALuint>();
}
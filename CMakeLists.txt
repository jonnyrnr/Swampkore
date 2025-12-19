cmake_minimum_required(VERSION 3.15)

project(KoreSwamp VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE as a subdirectory (assumes JUCE is installed or submoduled)
# Uncomment and adjust path if JUCE is available
# add_subdirectory(JUCE)

# If JUCE is installed system-wide, find it
find_package(JUCE CONFIG REQUIRED)

# Define the plugin
juce_add_plugin(KoreSwamp
    COMPANY_NAME "DJ Swamp"
    PLUGIN_MANUFACTURER_CODE DjSw
    PLUGIN_CODE KrSw
    FORMATS AU AUv3 Standalone
    PRODUCT_NAME "KORE SWAMP"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CATEGORIES Instrument Synth Sampler
    AU_MAIN_TYPE kAudioUnitType_MusicDevice
)

# Source files
target_sources(KoreSwamp
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/AudioEngine.cpp
        Source/Sampler.cpp
        Source/WavetableSynth.cpp
        Source/SwingProcessor.cpp
        Source/LoFiProcessor.cpp
        Source/VoiceManager.cpp
)

# Headers
target_include_directories(KoreSwamp
    PUBLIC
        Source
)

# JUCE modules
target_compile_definitions(KoreSwamp
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
)

target_link_libraries(KoreSwamp
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

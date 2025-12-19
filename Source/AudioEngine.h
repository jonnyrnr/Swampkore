#pragma once

#include <JuceHeader.h>
#include "Sampler.h"
#include "WavetableSynth.h"
#include "SwingProcessor.h"
#include "LoFiProcessor.h"
#include "VoiceManager.h"

/**
 * KORE SWAMP Audio Engine
 * 
 * Architecture:
 * - 10 tracks total
 * - Tracks 1-8: 24-bit PCM Sampler (144 Korg ER-1/DDD-1 legacy assets)
 * - Tracks 9-10: Monophonic Wavetable Synths (Korg Wave)
 * - 66% MPC Swing timing
 * - 12-bit lo-fi saturation/vinyl-dust parallel effect
 * - Polyphony: 96 stereo voices (normal) / 192 stereo voices (Terminal Mode)
 */
class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void releaseResources();
    
    // Track management
    static constexpr int NUM_TRACKS = 10;
    static constexpr int NUM_SAMPLER_TRACKS = 8;
    static constexpr int NUM_SYNTH_TRACKS = 2;
    
    // Polyphony modes
    enum class PolyphonyMode
    {
        Normal,     // 96 stereo voices
        Terminal    // 192 stereo voices
    };
    
    void setPolyphonyMode(PolyphonyMode mode);
    PolyphonyMode getPolyphonyMode() const { return polyphonyMode; }
    
    // Track access
    KorgSampler* getSamplerTrack(int trackIndex);
    WavetableSynth* getSynthTrack(int synthIndex);
    
    // Effect controls
    void setSwingAmount(float amount); // 0.0 to 1.0, default 0.66 (66% MPC Swing)
    void setLoFiAmount(float amount);  // 0.0 to 1.0
    
    // Voice management
    VoiceManager* getVoiceManager() { return &voiceManager; }
    
private:
    // Sample rate
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
    
    // Tracks
    std::array<std::unique_ptr<KorgSampler>, NUM_SAMPLER_TRACKS> samplerTracks;
    std::array<std::unique_ptr<WavetableSynth>, NUM_SYNTH_TRACKS> synthTracks;
    
    // Effects
    SwingProcessor swingProcessor;
    LoFiProcessor loFiProcessor;
    
    // Voice management
    VoiceManager voiceManager;
    PolyphonyMode polyphonyMode = PolyphonyMode::Normal;
    
    // Processing buffers
    juce::AudioBuffer<float> trackBuffer;
    juce::AudioBuffer<float> effectBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

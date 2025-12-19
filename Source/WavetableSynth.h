#pragma once

#include <JuceHeader.h>
#include <array>
#include <vector>

/**
 * Wavetable Synthesizer - Monophonic
 * 
 * Based on Korg Wave wavetables
 * Features:
 * - Monophonic synthesis
 * - Multiple wavetable slots
 * - Wavetable position morphing
 * - ADSR envelope
 * - Filter
 */
class WavetableSynth : public juce::Synthesiser
{
public:
    WavetableSynth();
    ~WavetableSynth() override;
    
    // Wavetable management
    static constexpr int WAVETABLE_SIZE = 2048;
    static constexpr int MAX_WAVETABLES = 64; // Korg wave count
    
    void loadKorgWavetables();
    void setCurrentWavetable(int index);
    void setWavetablePosition(float position); // 0.0 to 1.0
    
    // Synthesis parameters
    void setFilterCutoff(float cutoffHz);
    void setFilterResonance(float resonance);
    
    // ADSR
    void setAttack(float attackSeconds);
    void setDecay(float decaySeconds);
    void setSustain(float sustainLevel);
    void setRelease(float releaseSeconds);
    
    // Monophonic behavior
    void setGlideTime(float glideSeconds);
    
private:
    struct Wavetable
    {
        std::array<float, WAVETABLE_SIZE> data;
        juce::String name;
        bool loaded = false;
    };
    
    std::array<Wavetable, MAX_WAVETABLES> wavetables;
    int currentWavetableIndex = 0;
    float wavetablePosition = 0.0f;
    
    // ADSR parameters
    juce::ADSR::Parameters adsrParams;
    
    // Filter parameters
    float filterCutoff = 20000.0f;
    float filterResonance = 1.0f;
    
    // Glide parameters
    float glideTime = 0.0f;
    
    // Voice class for wavetable synth
    class WavetableVoice : public juce::SynthesiserVoice
    {
    public:
        WavetableVoice();
        
        bool canPlaySound(juce::SynthesiserSound*) override;
        void startNote(int midiNoteNumber, float velocity,
                      juce::SynthesiserSound*, int currentPitchWheelPosition) override;
        void stopNote(float velocity, bool allowTailOff) override;
        void pitchWheelMoved(int newPitchWheelValue) override;
        void controllerMoved(int controllerNumber, int newControllerValue) override;
        void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                            int startSample, int numSamples) override;
        
        void setADSRParameters(const juce::ADSR::Parameters& params);
        void setWavetable(const std::array<float, WAVETABLE_SIZE>* wavetable);
        void setWavetablePosition(float position);
        void setFilterParameters(float cutoff, float resonance);
        void setGlideTime(float glideSeconds);
        void prepareToPlay(double sampleRate);
        
    private:
        juce::ADSR adsr;
        const std::array<float, WAVETABLE_SIZE>* currentWavetable = nullptr;
        float wavetablePos = 0.0f;
        double phase = 0.0;
        double targetFrequency = 0.0;
        double currentFrequency = 0.0;
        float velocity = 0.0f;
        float glideRate = 0.0f;
        double sampleRate = 44100.0;
        
        // Filter state variables
        juce::IIRFilter filter;
        float filterCutoff = 20000.0f;
        float filterResonance = 1.0f;
        
        void updateFilter();
        
        JUCE_LEAK_DETECTOR(WavetableVoice)
    };
    
    // Sound class for wavetable synth
    class WavetableSound : public juce::SynthesiserSound
    {
    public:
        WavetableSound();
        
        bool appliesToNote(int midiNoteNumber) override;
        bool appliesToChannel(int midiChannel) override;
        
        JUCE_LEAK_DETECTOR(WavetableSound)
    };
    
    void initializeVoices();
    void generateKorgWaveforms();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableSynth)
};

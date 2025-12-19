#pragma once

#include <JuceHeader.h>
#include <array>

/**
 * Korg Sampler - 24-bit PCM Sampler
 * 
 * Supports loading and playback of 144 Korg ER-1/DDD-1 legacy assets
 * Features:
 * - 24-bit PCM audio playback
 * - Multiple sample slots
 * - ADSR envelope
 * - Velocity sensitivity
 */
class KorgSampler : public juce::Synthesiser
{
public:
    KorgSampler();
    ~KorgSampler() override;
    
    // Sample management
    static constexpr int MAX_SAMPLES = 144; // Support for 144 Korg legacy samples
    static constexpr int BITS_PER_SAMPLE = 24;
    
    bool loadSample(int slotIndex, const juce::File& audioFile);
    bool loadSample(int slotIndex, const void* data, size_t dataSize);
    void clearSample(int slotIndex);
    void clearAllSamples();
    
    // Sample info
    int getNumLoadedSamples() const;
    bool isSampleLoaded(int slotIndex) const;
    juce::String getSampleName(int slotIndex) const;
    
    // Playback control
    void triggerSample(int slotIndex, int midiNote, float velocity);
    void stopAllNotes();
    
    // ADSR controls
    void setAttack(float attackSeconds);
    void setDecay(float decaySeconds);
    void setSustain(float sustainLevel);
    void setRelease(float releaseSeconds);
    
private:
    struct SampleSlot
    {
        juce::AudioFormatManager formatManager;
        std::unique_ptr<juce::AudioFormatReader> reader;
        juce::AudioBuffer<float> sampleData;
        juce::String name;
        bool loaded = false;
    };
    
    std::array<SampleSlot, MAX_SAMPLES> sampleSlots;
    
    // ADSR parameters
    juce::ADSR::Parameters adsrParams;
    
    // Voice class for sampler
    class SamplerVoice : public juce::SynthesiserVoice
    {
    public:
        SamplerVoice();
        
        bool canPlaySound(juce::SynthesiserSound*) override;
        void startNote(int midiNoteNumber, float velocity,
                      juce::SynthesiserSound*, int currentPitchWheelPosition) override;
        void stopNote(float velocity, bool allowTailOff) override;
        void pitchWheelMoved(int newPitchWheelValue) override;
        void controllerMoved(int controllerNumber, int newControllerValue) override;
        void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                            int startSample, int numSamples) override;
        
        void setADSRParameters(const juce::ADSR::Parameters& params);
        
    private:
        juce::ADSR adsr;
        juce::AudioBuffer<float>* currentSample = nullptr;
        double sourceSamplePosition = 0.0;
        double pitchRatio = 1.0;
        float velocity = 0.0f;
        
        JUCE_LEAK_DETECTOR(SamplerVoice)
    };
    
    // Sound class for sampler
    class SamplerSound : public juce::SynthesiserSound
    {
    public:
        SamplerSound(juce::AudioBuffer<float>* sampleData, int midiNote);
        
        bool appliesToNote(int midiNoteNumber) override;
        bool appliesToChannel(int midiChannel) override;
        
        juce::AudioBuffer<float>* getSampleData() { return sampleData; }
        
    private:
        juce::AudioBuffer<float>* sampleData;
        int rootNote;
        
        JUCE_LEAK_DETECTOR(SamplerSound)
    };
    
    void initializeVoices();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KorgSampler)
};

#pragma once

#include <JuceHeader.h>
#include <random>

/**
 * Lo-Fi Processor - 12-bit Saturation and Vinyl Dust
 * 
 * Parallel effect processor that adds:
 * - 12-bit depth reduction (bit crushing)
 * - Saturation/distortion
 * - Vinyl dust/crackle simulation
 * 
 * Runs in parallel with the dry signal for blending
 */
class LoFiProcessor
{
public:
    LoFiProcessor();
    ~LoFiProcessor();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& wetBuffer);
    void reset();
    
    // Mix control (0.0 = dry, 1.0 = fully wet)
    void setWetAmount(float amount);
    float getWetAmount() const { return wetAmount; }
    
    // Bit depth reduction (1-24 bits, default 12)
    void setBitDepth(int bits);
    int getBitDepth() const { return bitDepth; }
    
    // Saturation amount (0.0 to 1.0)
    void setSaturationAmount(float amount);
    float getSaturationAmount() const { return saturationAmount; }
    
    // Vinyl dust density (0.0 to 1.0)
    void setVinylDustDensity(float density);
    float getVinylDustDensity() const { return vinylDustDensity; }
    
    // Vinyl crackle intensity (0.0 to 1.0)
    void setVinylCrackleIntensity(float intensity);
    float getVinylCrackleIntensity() const { return vinylCrackleIntensity; }
    
private:
    double sampleRate = 44100.0;
    float wetAmount = 0.5f; // Default 50% parallel mix
    
    // Bit crushing parameters
    int bitDepth = 12;
    float quantizationStep = 0.0f;
    
    // Saturation parameters
    float saturationAmount = 0.5f;
    
    // Vinyl simulation parameters
    float vinylDustDensity = 0.3f;
    float vinylCrackleIntensity = 0.2f;
    
    // Random generator for vinyl effects
    std::mt19937 randomGenerator;
    std::uniform_real_distribution<float> distribution;
    
    // Vinyl noise state
    float lastCrackleValue = 0.0f;
    int crackleCounter = 0;
    
    // Processing functions
    float applyBitReduction(float sample);
    float applySaturation(float sample);
    float generateVinylNoise();
    
    void updateQuantizationStep();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoFiProcessor)
};

#include "LoFiProcessor.h"
#include <chrono>

LoFiProcessor::LoFiProcessor()
    : distribution(0.0f, 1.0f)
{
    randomGenerator.seed(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    updateQuantizationStep();
}

LoFiProcessor::~LoFiProcessor()
{
}

void LoFiProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;
    reset();
}

void LoFiProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& wetBuffer)
{
    // Process wet buffer with lo-fi effects
    for (int channel = 0; channel < wetBuffer.getNumChannels(); ++channel)
    {
        auto* wetData = wetBuffer.getWritePointer(channel);
        
        for (int sample = 0; sample < wetBuffer.getNumSamples(); ++sample)
        {
            float inputSample = wetData[sample];
            
            // Apply bit reduction
            float processedSample = applyBitReduction(inputSample);
            
            // Apply saturation
            processedSample = applySaturation(processedSample);
            
            // Add vinyl noise
            processedSample += generateVinylNoise();
            
            wetData[sample] = processedSample;
        }
    }
    
    // Mix wet and dry signals in parallel
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* dryData = buffer.getWritePointer(channel);
        const auto* wetData = wetBuffer.getReadPointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            dryData[sample] = dryData[sample] * (1.0f - wetAmount) + wetData[sample] * wetAmount;
        }
    }
}

void LoFiProcessor::reset()
{
    lastCrackleValue = 0.0f;
    crackleCounter = 0;
}

void LoFiProcessor::setWetAmount(float amount)
{
    wetAmount = juce::jlimit(0.0f, 1.0f, amount);
}

void LoFiProcessor::setBitDepth(int bits)
{
    bitDepth = juce::jlimit(1, 24, bits);
    updateQuantizationStep();
}

void LoFiProcessor::setSaturationAmount(float amount)
{
    saturationAmount = juce::jlimit(0.0f, 1.0f, amount);
}

void LoFiProcessor::setVinylDustDensity(float density)
{
    vinylDustDensity = juce::jlimit(0.0f, 1.0f, density);
}

void LoFiProcessor::setVinylCrackleIntensity(float intensity)
{
    vinylCrackleIntensity = juce::jlimit(0.0f, 1.0f, intensity);
}

float LoFiProcessor::applyBitReduction(float sample)
{
    if (bitDepth >= 24)
        return sample; // No reduction
    
    // Quantize to reduced bit depth
    float quantized = std::floor(sample / quantizationStep + 0.5f) * quantizationStep;
    return juce::jlimit(-1.0f, 1.0f, quantized);
}

float LoFiProcessor::applySaturation(float sample)
{
    if (saturationAmount <= 0.0f)
        return sample;
    
    // Soft clipping saturation
    float drive = 1.0f + saturationAmount * 5.0f;
    float driven = sample * drive;
    
    // Tanh saturation curve
    float saturated = std::tanh(driven);
    
    // Blend with original
    return sample * (1.0f - saturationAmount) + saturated * saturationAmount;
}

float LoFiProcessor::generateVinylNoise()
{
    float noise = 0.0f;
    
    // Vinyl dust (random pops)
    if (distribution(randomGenerator) < vinylDustDensity * 0.001f)
    {
        noise += (distribution(randomGenerator) * 2.0f - 1.0f) * 0.3f;
    }
    
    // Vinyl crackle (sustained noise)
    crackleCounter++;
    if (crackleCounter > (int)(sampleRate * 0.01)) // Update every 10ms
    {
        crackleCounter = 0;
        lastCrackleValue = (distribution(randomGenerator) * 2.0f - 1.0f) * vinylCrackleIntensity * 0.05f;
    }
    
    noise += lastCrackleValue;
    
    return noise;
}

void LoFiProcessor::updateQuantizationStep()
{
    // Calculate quantization step for the given bit depth
    int levels = 1 << bitDepth;
    quantizationStep = 2.0f / levels;
}

#include "WavetableSynth.h"

WavetableSynth::WavetableSynth()
{
    // Set default ADSR
    adsrParams.attack = 0.01f;
    adsrParams.decay = 0.1f;
    adsrParams.sustain = 0.8f;
    adsrParams.release = 0.5f;
    
    generateKorgWaveforms();
    initializeVoices();
}

WavetableSynth::~WavetableSynth()
{
}

void WavetableSynth::initializeVoices()
{
    // Monophonic - only need 1 voice
    addVoice(new WavetableVoice());
    addSound(new WavetableSound());
    
    // Set initial parameters for voice
    if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(0)))
    {
        voice->setADSRParameters(adsrParams);
        voice->setWavetable(&wavetables[0].data);
        voice->setFilterParameters(filterCutoff, filterResonance);
        voice->setGlideTime(glideTime);
    }
}

void WavetableSynth::generateKorgWaveforms()
{
    // Generate classic Korg waveforms
    const float twoPi = 2.0f * juce::MathConstants<float>::pi;
    
    // Waveform 0: Sawtooth
    for (int i = 0; i < WAVETABLE_SIZE; ++i)
    {
        float phase = (float)i / WAVETABLE_SIZE;
        wavetables[0].data[i] = 2.0f * phase - 1.0f;
    }
    wavetables[0].name = "Sawtooth";
    wavetables[0].loaded = true;
    
    // Waveform 1: Square
    for (int i = 0; i < WAVETABLE_SIZE; ++i)
    {
        float phase = (float)i / WAVETABLE_SIZE;
        wavetables[1].data[i] = phase < 0.5f ? 1.0f : -1.0f;
    }
    wavetables[1].name = "Square";
    wavetables[1].loaded = true;
    
    // Waveform 2: Triangle
    for (int i = 0; i < WAVETABLE_SIZE; ++i)
    {
        float phase = (float)i / WAVETABLE_SIZE;
        wavetables[2].data[i] = 2.0f * std::abs(2.0f * phase - 1.0f) - 1.0f;
    }
    wavetables[2].name = "Triangle";
    wavetables[2].loaded = true;
    
    // Waveform 3: Sine
    for (int i = 0; i < WAVETABLE_SIZE; ++i)
    {
        float phase = (float)i / WAVETABLE_SIZE;
        wavetables[3].data[i] = std::sin(twoPi * phase);
    }
    wavetables[3].name = "Sine";
    wavetables[3].loaded = true;
    
    // Generate more complex waveforms for remaining slots
    for (int waveIndex = 4; waveIndex < MAX_WAVETABLES; ++waveIndex)
    {
        for (int i = 0; i < WAVETABLE_SIZE; ++i)
        {
            float phase = (float)i / WAVETABLE_SIZE;
            float value = 0.0f;
            
            // Add harmonics with varying ratios
            int numHarmonics = (waveIndex % 8) + 1;
            for (int h = 1; h <= numHarmonics; ++h)
            {
                float harmonic = std::sin(twoPi * phase * h);
                value += harmonic / (float)h;
            }
            
            wavetables[waveIndex].data[i] = value * 0.5f;
        }
        wavetables[waveIndex].name = "Korg_" + juce::String(waveIndex);
        wavetables[waveIndex].loaded = true;
    }
}

void WavetableSynth::loadKorgWavetables()
{
    // Already loaded in constructor
}

void WavetableSynth::setCurrentWavetable(int index)
{
    if (index >= 0 && index < MAX_WAVETABLES)
    {
        currentWavetableIndex = index;
        
        for (int i = 0; i < getNumVoices(); ++i)
        {
            if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(i)))
            {
                voice->setWavetable(&wavetables[index].data);
            }
        }
    }
}

void WavetableSynth::setWavetablePosition(float position)
{
    wavetablePosition = juce::jlimit(0.0f, 1.0f, position);
    
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(i)))
        {
            voice->setWavetablePosition(wavetablePosition);
        }
    }
}

void WavetableSynth::setFilterCutoff(float cutoffHz)
{
    filterCutoff = cutoffHz;
    
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(i)))
        {
            voice->setFilterParameters(filterCutoff, filterResonance);
        }
    }
}

void WavetableSynth::setFilterResonance(float resonance)
{
    filterResonance = resonance;
    
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(i)))
        {
            voice->setFilterParameters(filterCutoff, filterResonance);
        }
    }
}

void WavetableSynth::setAttack(float attackSeconds)
{
    adsrParams.attack = attackSeconds;
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(i)))
        {
            voice->setADSRParameters(adsrParams);
        }
    }
}

void WavetableSynth::setDecay(float decaySeconds)
{
    adsrParams.decay = decaySeconds;
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(i)))
        {
            voice->setADSRParameters(adsrParams);
        }
    }
}

void WavetableSynth::setSustain(float sustainLevel)
{
    adsrParams.sustain = sustainLevel;
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(i)))
        {
            voice->setADSRParameters(adsrParams);
        }
    }
}

void WavetableSynth::setRelease(float releaseSeconds)
{
    adsrParams.release = releaseSeconds;
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(i)))
        {
            voice->setADSRParameters(adsrParams);
        }
    }
}

void WavetableSynth::setGlideTime(float glideSeconds)
{
    glideTime = glideSeconds;
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WavetableVoice*>(getVoice(i)))
        {
            voice->setGlideTime(glideTime);
        }
    }
}

// WavetableVoice implementation
WavetableSynth::WavetableVoice::WavetableVoice()
{
}

bool WavetableSynth::WavetableVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<WavetableSound*>(sound) != nullptr;
}

void WavetableSynth::WavetableVoice::startNote(int midiNoteNumber, float velocityValue,
                                               juce::SynthesiserSound*, int /*currentPitchWheelPosition*/)
{
    velocity = velocityValue;
    targetFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    
    // Glide or instant pitch change
    if (glideRate > 0.0f && currentFrequency > 0.0)
    {
        // Glide to new frequency
    }
    else
    {
        currentFrequency = targetFrequency;
    }
    
    adsr.noteOn();
}

void WavetableSynth::WavetableVoice::stopNote(float /*velocityValue*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        clearCurrentNote();
        adsr.reset();
    }
}

void WavetableSynth::WavetableVoice::pitchWheelMoved(int /*newPitchWheelValue*/)
{
}

void WavetableSynth::WavetableVoice::controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/)
{
}

void WavetableSynth::WavetableVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                                     int startSample, int numSamples)
{
    if (currentWavetable == nullptr)
    {
        clearCurrentNote();
        return;
    }
    
    for (int i = startSample; i < startSample + numSamples; ++i)
    {
        float envValue = adsr.getNextSample();
        
        if (envValue <= 0.0f && !adsr.isActive())
        {
            clearCurrentNote();
            break;
        }
        
        // Apply glide
        if (currentFrequency != targetFrequency && glideRate > 0.0f)
        {
            float diff = targetFrequency - currentFrequency;
            currentFrequency += diff * glideRate;
        }
        
        // Generate wavetable sample
        double phaseIncrement = currentFrequency / sampleRate * WAVETABLE_SIZE;
        phase += phaseIncrement;
        
        while (phase >= WAVETABLE_SIZE)
            phase -= WAVETABLE_SIZE;
        
        // Interpolated wavetable lookup
        int index = (int)phase;
        float fraction = phase - index;
        int nextIndex = (index + 1) % WAVETABLE_SIZE;
        
        float sample1 = (*currentWavetable)[index];
        float sample2 = (*currentWavetable)[nextIndex];
        float sample = sample1 + fraction * (sample2 - sample1);
        
        // Apply envelope and velocity
        sample *= envValue * velocity;
        
        // Apply filter
        sample = filter.processSingleSampleRaw(sample);
        
        // Write to output
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addSample(channel, i, sample);
        }
    }
}

void WavetableSynth::WavetableVoice::setADSRParameters(const juce::ADSR::Parameters& params)
{
    adsr.setParameters(params);
}

void WavetableSynth::WavetableVoice::setWavetable(const std::array<float, WAVETABLE_SIZE>* wavetable)
{
    currentWavetable = wavetable;
}

void WavetableSynth::WavetableVoice::setWavetablePosition(float position)
{
    wavetablePos = position;
}

void WavetableSynth::WavetableVoice::setFilterParameters(float cutoff, float resonance)
{
    filterCutoff = cutoff;
    filterResonance = resonance;
    updateFilter();
}

void WavetableSynth::WavetableVoice::setGlideTime(float glideSeconds)
{
    if (glideSeconds > 0.0f)
    {
        glideRate = 1.0f / (glideSeconds * sampleRate);
    }
    else
    {
        glideRate = 0.0f;
    }
}

void WavetableSynth::WavetableVoice::prepareToPlay(double newSampleRate)
{
    sampleRate = newSampleRate;
    updateFilter();
}

void WavetableSynth::WavetableVoice::updateFilter()
{
    // Simple lowpass filter
    auto coefficients = juce::IIRCoefficients::makeLowPass(sampleRate, filterCutoff, filterResonance);
    filter.setCoefficients(coefficients);
}

// WavetableSound implementation
WavetableSynth::WavetableSound::WavetableSound()
{
}

bool WavetableSynth::WavetableSound::appliesToNote(int /*midiNoteNumber*/)
{
    return true;
}

bool WavetableSynth::WavetableSound::appliesToChannel(int /*midiChannel*/)
{
    return true;
}

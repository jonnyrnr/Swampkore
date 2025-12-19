#include "Sampler.h"

// KorgSampler implementation
KorgSampler::KorgSampler()
{
    // Set default ADSR
    adsrParams.attack = 0.01f;
    adsrParams.decay = 0.1f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 0.3f;
    
    initializeVoices();
}

KorgSampler::~KorgSampler()
{
    clearAllSamples();
}

void KorgSampler::initializeVoices()
{
    // Add voices for polyphony
    for (int i = 0; i < 16; ++i)
    {
        addVoice(new SamplerVoice());
    }
    
    // Add a default sound
    addSound(new SamplerSound(nullptr, 60));
}

bool KorgSampler::loadSample(int slotIndex, const juce::File& audioFile)
{
    if (slotIndex < 0 || slotIndex >= MAX_SAMPLES)
        return false;
    
    auto& slot = sampleSlots[slotIndex];
    
    // Register basic audio formats
    slot.formatManager.registerBasicFormats();
    
    // Create reader
    auto reader = slot.formatManager.createReaderFor(audioFile);
    if (reader == nullptr)
        return false;
    
    // Load sample data
    slot.sampleData.setSize(reader->numChannels, (int)reader->lengthInSamples);
    reader->read(&slot.sampleData, 0, (int)reader->lengthInSamples, 0, true, true);
    
    slot.name = audioFile.getFileNameWithoutExtension();
    slot.loaded = true;
    
    delete reader;
    return true;
}

bool KorgSampler::loadSample(int slotIndex, const void* data, size_t dataSize)
{
    if (slotIndex < 0 || slotIndex >= MAX_SAMPLES)
        return false;
    
    auto& slot = sampleSlots[slotIndex];
    
    slot.formatManager.registerBasicFormats();
    
    // Create memory input stream
    juce::MemoryInputStream memoryStream(data, dataSize, false);
    auto reader = slot.formatManager.createReaderFor(std::unique_ptr<juce::InputStream>(&memoryStream));
    
    if (reader == nullptr)
        return false;
    
    slot.sampleData.setSize(reader->numChannels, (int)reader->lengthInSamples);
    reader->read(&slot.sampleData, 0, (int)reader->lengthInSamples, 0, true, true);
    
    slot.name = "Sample_" + juce::String(slotIndex);
    slot.loaded = true;
    
    delete reader;
    return true;
}

void KorgSampler::clearSample(int slotIndex)
{
    if (slotIndex >= 0 && slotIndex < MAX_SAMPLES)
    {
        auto& slot = sampleSlots[slotIndex];
        slot.sampleData.setSize(0, 0);
        slot.name = "";
        slot.loaded = false;
    }
}

void KorgSampler::clearAllSamples()
{
    for (int i = 0; i < MAX_SAMPLES; ++i)
    {
        clearSample(i);
    }
}

int KorgSampler::getNumLoadedSamples() const
{
    int count = 0;
    for (const auto& slot : sampleSlots)
    {
        if (slot.loaded)
            ++count;
    }
    return count;
}

bool KorgSampler::isSampleLoaded(int slotIndex) const
{
    if (slotIndex >= 0 && slotIndex < MAX_SAMPLES)
        return sampleSlots[slotIndex].loaded;
    return false;
}

juce::String KorgSampler::getSampleName(int slotIndex) const
{
    if (slotIndex >= 0 && slotIndex < MAX_SAMPLES)
        return sampleSlots[slotIndex].name;
    return "";
}

void KorgSampler::triggerSample(int slotIndex, int midiNote, float velocity)
{
    if (slotIndex >= 0 && slotIndex < MAX_SAMPLES && sampleSlots[slotIndex].loaded)
    {
        noteOn(1, midiNote, velocity);
    }
}

void KorgSampler::stopAllNotes()
{
    allNotesOff(1, true);
}

void KorgSampler::setAttack(float attackSeconds)
{
    adsrParams.attack = attackSeconds;
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SamplerVoice*>(getVoice(i)))
        {
            voice->setADSRParameters(adsrParams);
        }
    }
}

void KorgSampler::setDecay(float decaySeconds)
{
    adsrParams.decay = decaySeconds;
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SamplerVoice*>(getVoice(i)))
        {
            voice->setADSRParameters(adsrParams);
        }
    }
}

void KorgSampler::setSustain(float sustainLevel)
{
    adsrParams.sustain = sustainLevel;
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SamplerVoice*>(getVoice(i)))
        {
            voice->setADSRParameters(adsrParams);
        }
    }
}

void KorgSampler::setRelease(float releaseSeconds)
{
    adsrParams.release = releaseSeconds;
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SamplerVoice*>(getVoice(i)))
        {
            voice->setADSRParameters(adsrParams);
        }
    }
}

// SamplerVoice implementation
KorgSampler::SamplerVoice::SamplerVoice()
{
}

bool KorgSampler::SamplerVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SamplerSound*>(sound) != nullptr;
}

void KorgSampler::SamplerVoice::startNote(int midiNoteNumber, float velocityValue,
                                          juce::SynthesiserSound* sound, int /*currentPitchWheelPosition*/)
{
    if (auto* samplerSound = dynamic_cast<SamplerSound*>(sound))
    {
        currentSample = samplerSound->getSampleData();
        velocity = velocityValue;
        sourceSamplePosition = 0.0;
        
        // Calculate pitch ratio based on MIDI note
        pitchRatio = std::pow(2.0, (midiNoteNumber - 60) / 12.0);
        
        adsr.noteOn();
    }
}

void KorgSampler::SamplerVoice::stopNote(float /*velocityValue*/, bool allowTailOff)
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

void KorgSampler::SamplerVoice::pitchWheelMoved(int /*newPitchWheelValue*/)
{
}

void KorgSampler::SamplerVoice::controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/)
{
}

void KorgSampler::SamplerVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                                int startSample, int numSamples)
{
    if (currentSample == nullptr || currentSample->getNumSamples() == 0)
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
        
        // Get sample with interpolation
        int index = (int)sourceSamplePosition;
        float fraction = sourceSamplePosition - index;
        
        if (index < currentSample->getNumSamples() - 1)
        {
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                int sourceChannel = std::min(channel, currentSample->getNumChannels() - 1);
                float sample1 = currentSample->getSample(sourceChannel, index);
                float sample2 = currentSample->getSample(sourceChannel, index + 1);
                float interpolated = sample1 + fraction * (sample2 - sample1);
                
                outputBuffer.addSample(channel, i, interpolated * envValue * velocity);
            }
            
            sourceSamplePosition += pitchRatio;
        }
        else
        {
            clearCurrentNote();
            break;
        }
    }
}

void KorgSampler::SamplerVoice::setADSRParameters(const juce::ADSR::Parameters& params)
{
    adsr.setParameters(params);
}

// SamplerSound implementation
KorgSampler::SamplerSound::SamplerSound(juce::AudioBuffer<float>* data, int midiNote)
    : sampleData(data), rootNote(midiNote)
{
}

bool KorgSampler::SamplerSound::appliesToNote(int /*midiNoteNumber*/)
{
    return true;
}

bool KorgSampler::SamplerSound::appliesToChannel(int /*midiChannel*/)
{
    return true;
}

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    // Initialize sampler tracks (1-8)
    for (int i = 0; i < NUM_SAMPLER_TRACKS; ++i)
    {
        samplerTracks[i] = std::make_unique<KorgSampler>();
    }
    
    // Initialize synth tracks (9-10)
    for (int i = 0; i < NUM_SYNTH_TRACKS; ++i)
    {
        synthTracks[i] = std::make_unique<WavetableSynth>();
        synthTracks[i]->loadKorgWavetables();
    }
    
    // Set default swing (66% MPC)
    swingProcessor.setSwingAmount(0.66f);
}

AudioEngine::~AudioEngine()
{
}

void AudioEngine::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    
    // Prepare all sampler tracks
    for (auto& sampler : samplerTracks)
    {
        sampler->setCurrentPlaybackSampleRate(sampleRate);
    }
    
    // Prepare all synth tracks
    for (auto& synth : synthTracks)
    {
        synth->setCurrentPlaybackSampleRate(sampleRate);
    }
    
    // Prepare effects
    swingProcessor.prepareToPlay(sampleRate, samplesPerBlock);
    loFiProcessor.prepareToPlay(sampleRate, samplesPerBlock);
    
    // Prepare buffers
    trackBuffer.setSize(2, samplesPerBlock);
    effectBuffer.setSize(2, samplesPerBlock);
    
    // Set initial polyphony
    int maxVoices = (polyphonyMode == PolyphonyMode::Normal) ? 96 : 192;
    voiceManager.setMode(polyphonyMode == PolyphonyMode::Normal ? 
                        VoiceManager::Mode::Normal : VoiceManager::Mode::Terminal);
}

void AudioEngine::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Clear output buffer
    buffer.clear();
    
    // Apply swing to MIDI messages
    swingProcessor.processBlock(buffer, midiMessages);
    
    // Process each sampler track
    for (auto& sampler : samplerTracks)
    {
        trackBuffer.clear();
        sampler->renderNextBlock(trackBuffer, midiMessages, 0, buffer.getNumSamples());
        
        // Mix into main buffer
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.addFrom(channel, 0, trackBuffer, channel, 0, buffer.getNumSamples());
        }
    }
    
    // Process each synth track
    for (auto& synth : synthTracks)
    {
        trackBuffer.clear();
        synth->renderNextBlock(trackBuffer, midiMessages, 0, buffer.getNumSamples());
        
        // Mix into main buffer
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.addFrom(channel, 0, trackBuffer, channel, 0, buffer.getNumSamples());
        }
    }
    
    // Apply lo-fi effect in parallel
    effectBuffer.makeCopyOf(buffer, true);
    loFiProcessor.processBlock(buffer, effectBuffer);
}

void AudioEngine::releaseResources()
{
    for (auto& sampler : samplerTracks)
    {
        sampler->allNotesOff(0, false);
    }
    
    for (auto& synth : synthTracks)
    {
        synth->allNotesOff(0, false);
    }
    
    swingProcessor.reset();
    loFiProcessor.reset();
}

void AudioEngine::setPolyphonyMode(PolyphonyMode mode)
{
    polyphonyMode = mode;
    voiceManager.setMode(mode == PolyphonyMode::Normal ? 
                        VoiceManager::Mode::Normal : VoiceManager::Mode::Terminal);
}

KorgSampler* AudioEngine::getSamplerTrack(int trackIndex)
{
    if (trackIndex >= 0 && trackIndex < NUM_SAMPLER_TRACKS)
        return samplerTracks[trackIndex].get();
    return nullptr;
}

WavetableSynth* AudioEngine::getSynthTrack(int synthIndex)
{
    if (synthIndex >= 0 && synthIndex < NUM_SYNTH_TRACKS)
        return synthTracks[synthIndex].get();
    return nullptr;
}

void AudioEngine::setSwingAmount(float amount)
{
    swingProcessor.setSwingAmount(juce::jlimit(0.0f, 1.0f, amount));
}

void AudioEngine::setLoFiAmount(float amount)
{
    loFiProcessor.setWetAmount(juce::jlimit(0.0f, 1.0f, amount));
}

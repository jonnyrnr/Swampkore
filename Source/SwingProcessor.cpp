#include "SwingProcessor.h"

SwingProcessor::SwingProcessor()
{
    updateTimingParameters();
}

SwingProcessor::~SwingProcessor()
{
}

void SwingProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;
    updateTimingParameters();
    reset();
}

void SwingProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (swingAmount <= 0.01f)
    {
        // No swing, pass through
        return;
    }
    
    // Create new MIDI buffer with swung timing
    juce::MidiBuffer swungMidi;
    
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        int samplePosition = metadata.samplePosition;
        
        // Calculate swing offset
        int swungPosition = samplePosition;
        
        if (message.isNoteOn() || message.isNoteOff())
        {
            // Determine if this note falls on a swing beat
            int beatPosition = (int)(currentSamplePosition + samplePosition) % samplesPerBeat;
            int gridPosition = beatPosition % samplesPerGrid;
            int gridNumber = (beatPosition / samplesPerGrid) % 2;
            
            // Apply swing to every other 16th note
            if (gridNumber == 1)
            {
                // This is a swing beat - delay it
                int delay = (int)(samplesPerGrid * swingAmount * 0.5f);
                swungPosition += delay;
                
                // Make sure we don't go past the buffer
                if (swungPosition >= buffer.getNumSamples())
                    swungPosition = buffer.getNumSamples() - 1;
            }
        }
        
        swungMidi.addEvent(message, swungPosition);
    }
    
    // Replace original MIDI buffer with swung version
    midiMessages.swapWith(swungMidi);
    
    // Update sample position
    currentSamplePosition += buffer.getNumSamples();
}

void SwingProcessor::reset()
{
    currentSamplePosition = 0.0;
    processedEvents.clear();
}

void SwingProcessor::setSwingAmount(float amount)
{
    swingAmount = juce::jlimit(0.0f, 1.0f, amount);
}

void SwingProcessor::setGridResolution(GridResolution resolution)
{
    gridResolution = resolution;
    updateTimingParameters();
}

void SwingProcessor::setTempo(double bpm)
{
    tempo = bpm;
    updateTimingParameters();
}

void SwingProcessor::updateTimingParameters()
{
    // Calculate samples per beat
    samplesPerBeat = (int)(sampleRate * 60.0 / tempo);
    
    // Calculate samples per grid division
    switch (gridResolution)
    {
        case GridResolution::Sixteenth:
            samplesPerGrid = samplesPerBeat / 4;
            break;
        case GridResolution::Eighth:
            samplesPerGrid = samplesPerBeat / 2;
            break;
        case GridResolution::Quarter:
            samplesPerGrid = samplesPerBeat;
            break;
    }
}

int SwingProcessor::calculateSwingDelay(int samplePosition)
{
    int gridNumber = (samplePosition / samplesPerGrid) % 2;
    
    if (gridNumber == 1)
    {
        // Apply swing delay
        return (int)(samplesPerGrid * swingAmount * 0.5f);
    }
    
    return 0;
}

bool SwingProcessor::isOnSwingBeat(int samplePosition)
{
    int gridNumber = (samplePosition / samplesPerGrid) % 2;
    return gridNumber == 1;
}

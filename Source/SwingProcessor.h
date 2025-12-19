#pragma once

#include <JuceHeader.h>
#include <vector>

/**
 * Swing Processor - 66% MPC Swing
 * 
 * Applies timing modulation to create swing feel
 * Default: 66% swing (MPC-style)
 * 
 * Swing works by delaying every other 16th note to create a triplet feel
 */
class SwingProcessor
{
public:
    SwingProcessor();
    ~SwingProcessor();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void reset();
    
    // Swing amount: 0.0 = straight, 0.5 = triplet, 0.66 = MPC swing, 1.0 = max shuffle
    void setSwingAmount(float amount);
    float getSwingAmount() const { return swingAmount; }
    
    // Set the grid resolution (e.g., 16th notes, 8th notes)
    enum class GridResolution
    {
        Sixteenth,
        Eighth,
        Quarter
    };
    
    void setGridResolution(GridResolution resolution);
    GridResolution getGridResolution() const { return gridResolution; }
    
    // Tempo (for swing timing calculations)
    void setTempo(double bpm);
    double getTempo() const { return tempo; }
    
private:
    double sampleRate = 44100.0;
    float swingAmount = 0.66f; // Default 66% MPC Swing
    GridResolution gridResolution = GridResolution::Sixteenth;
    double tempo = 120.0;
    
    // Timing calculation
    struct NoteEvent
    {
        int noteNumber;
        float velocity;
        int originalSamplePosition;
        int swungSamplePosition;
        bool isNoteOn;
    };
    
    std::vector<NoteEvent> processedEvents;
    
    int calculateSwingDelay(int samplePosition);
    bool isOnSwingBeat(int samplePosition);
    
    // Position tracking
    double currentSamplePosition = 0.0;
    int samplesPerBeat = 0;
    int samplesPerGrid = 0;
    
    void updateTimingParameters();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SwingProcessor)
};

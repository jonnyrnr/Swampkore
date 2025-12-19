#pragma once

#include <JuceHeader.h>
#include <array>
#include <vector>

/**
 * Voice Manager
 * 
 * Manages polyphony across all tracks
 * - Normal Mode: 96 stereo voices
 * - Terminal Mode: 192 stereo voices
 * 
 * Voice allocation strategy: oldest note stealing
 */
class VoiceManager
{
public:
    VoiceManager();
    ~VoiceManager();
    
    // Polyphony limits
    static constexpr int NORMAL_MODE_VOICES = 96;
    static constexpr int TERMINAL_MODE_VOICES = 192;
    
    enum class Mode
    {
        Normal,     // 96 voices
        Terminal    // 192 voices
    };
    
    void setMode(Mode mode);
    Mode getMode() const { return currentMode; }
    
    // Voice tracking
    int getMaxVoices() const;
    int getActiveVoiceCount() const { return activeVoiceCount; }
    int getAvailableVoices() const { return getMaxVoices() - activeVoiceCount; }
    
    // Voice allocation
    struct VoiceAllocation
    {
        int voiceId;
        int trackNumber;
        int midiNote;
        float velocity;
        bool active;
        uint64_t timestamp;
    };
    
    bool allocateVoice(int trackNumber, int midiNote, float velocity, int& outVoiceId);
    void releaseVoice(int voiceId);
    void releaseAllVoices();
    
    // Voice stealing
    bool needsVoiceStealing() const;
    int findVoiceToSteal();
    
    // Statistics
    struct Statistics
    {
        int totalVoicesAllocated = 0;
        int totalVoicesStolen = 0;
        int peakVoiceCount = 0;
        float averageVoiceUsage = 0.0f;
    };
    
    const Statistics& getStatistics() const { return stats; }
    void resetStatistics();
    
private:
    Mode currentMode = Mode::Normal;
    int activeVoiceCount = 0;
    
    // Voice allocation tracking
    std::vector<VoiceAllocation> voiceAllocations;
    int nextVoiceId = 0;
    uint64_t timestamp = 0;
    
    Statistics stats;
    
    void updateStatistics();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoiceManager)
};

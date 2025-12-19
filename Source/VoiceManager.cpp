#include "VoiceManager.h"

VoiceManager::VoiceManager()
{
    // Initialize with normal mode capacity
    voiceAllocations.reserve(TERMINAL_MODE_VOICES);
}

VoiceManager::~VoiceManager()
{
}

void VoiceManager::setMode(Mode mode)
{
    currentMode = mode;
    
    // If switching to lower voice count mode, release excess voices
    int maxVoices = getMaxVoices();
    if (activeVoiceCount > maxVoices)
    {
        // Release oldest voices to fit within limit
        while (activeVoiceCount > maxVoices)
        {
            int oldestVoiceId = findVoiceToSteal();
            if (oldestVoiceId >= 0)
                releaseVoice(oldestVoiceId);
            else
                break;
        }
    }
}

int VoiceManager::getMaxVoices() const
{
    return (currentMode == Mode::Normal) ? NORMAL_MODE_VOICES : TERMINAL_MODE_VOICES;
}

bool VoiceManager::allocateVoice(int trackNumber, int midiNote, float velocity, int& outVoiceId)
{
    // Check if we need to steal a voice
    if (activeVoiceCount >= getMaxVoices())
    {
        int stolenVoiceId = findVoiceToSteal();
        if (stolenVoiceId >= 0)
        {
            releaseVoice(stolenVoiceId);
            stats.totalVoicesStolen++;
        }
        else
        {
            return false; // Cannot allocate
        }
    }
    
    // Allocate new voice
    VoiceAllocation allocation;
    allocation.voiceId = nextVoiceId++;
    allocation.trackNumber = trackNumber;
    allocation.midiNote = midiNote;
    allocation.velocity = velocity;
    allocation.active = true;
    allocation.timestamp = timestamp++;
    
    voiceAllocations.push_back(allocation);
    activeVoiceCount++;
    
    outVoiceId = allocation.voiceId;
    
    stats.totalVoicesAllocated++;
    updateStatistics();
    
    return true;
}

void VoiceManager::releaseVoice(int voiceId)
{
    for (auto it = voiceAllocations.begin(); it != voiceAllocations.end(); ++it)
    {
        if (it->voiceId == voiceId && it->active)
        {
            it->active = false;
            activeVoiceCount--;
            
            // Remove from vector
            voiceAllocations.erase(it);
            updateStatistics();
            return;
        }
    }
}

void VoiceManager::releaseAllVoices()
{
    voiceAllocations.clear();
    activeVoiceCount = 0;
    updateStatistics();
}

bool VoiceManager::needsVoiceStealing() const
{
    return activeVoiceCount >= getMaxVoices();
}

int VoiceManager::findVoiceToSteal()
{
    if (voiceAllocations.empty())
        return -1;
    
    // Find oldest active voice
    uint64_t oldestTimestamp = UINT64_MAX;
    int oldestVoiceId = -1;
    
    for (const auto& allocation : voiceAllocations)
    {
        if (allocation.active && allocation.timestamp < oldestTimestamp)
        {
            oldestTimestamp = allocation.timestamp;
            oldestVoiceId = allocation.voiceId;
        }
    }
    
    return oldestVoiceId;
}

void VoiceManager::resetStatistics()
{
    stats = Statistics();
}

void VoiceManager::updateStatistics()
{
    // Update peak voice count
    if (activeVoiceCount > stats.peakVoiceCount)
    {
        stats.peakVoiceCount = activeVoiceCount;
    }
    
    // Update average voice usage
    if (stats.totalVoicesAllocated > 0)
    {
        stats.averageVoiceUsage = (float)activeVoiceCount / (float)getMaxVoices();
    }
}

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

/**
 * KORE SWAMP - Plugin Editor
 * 
 * Main UI for the AUv3 mobile workstation
 */
class KoreSwampAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    KoreSwampAudioProcessorEditor(KoreSwampAudioProcessor&);
    ~KoreSwampAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    KoreSwampAudioProcessor& audioProcessor;
    
    // UI Components
    juce::Label titleLabel;
    juce::Label subtitleLabel;
    
    // Track indicators
    struct TrackIndicator : public juce::Component
    {
        TrackIndicator(int trackNum, bool isSampler);
        void paint(juce::Graphics& g) override;
        
        int trackNumber;
        bool isSamplerTrack;
        bool isActive = false;
    };
    
    juce::OwnedArray<TrackIndicator> trackIndicators;
    
    // Control panel
    juce::Slider swingSlider;
    juce::Label swingLabel;
    
    juce::Slider loFiSlider;
    juce::Label loFiLabel;
    
    juce::ToggleButton terminalModeButton;
    
    // Voice count display
    juce::Label voiceCountLabel;
    
    // Status display
    juce::Label statusLabel;
    
    void updateVoiceCountDisplay();
    
    // Timer for UI updates
    class UIUpdateTimer : public juce::Timer
    {
    public:
        UIUpdateTimer(KoreSwampAudioProcessorEditor& editor) : owner(editor) {}
        void timerCallback() override { owner.updateVoiceCountDisplay(); }
    private:
        KoreSwampAudioProcessorEditor& owner;
    };
    
    UIUpdateTimer updateTimer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KoreSwampAudioProcessorEditor)
};

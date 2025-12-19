#include "PluginProcessor.h"
#include "PluginEditor.h"

KoreSwampAudioProcessorEditor::KoreSwampAudioProcessorEditor(KoreSwampAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), updateTimer(*this)
{
    // Set up title
    titleLabel.setText("// KORE SWAMP", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);
    
    subtitleLabel.setText("DJ Swamp Mobile Workstation", juce::dontSendNotification);
    subtitleLabel.setFont(juce::Font(16.0f));
    subtitleLabel.setJustificationType(juce::Justification::centred);
    subtitleLabel.setColour(juce::Label::textColourId, juce::Colours::lightgrey);
    addAndMakeVisible(subtitleLabel);
    
    // Create track indicators
    for (int i = 0; i < 10; ++i)
    {
        bool isSampler = i < 8;
        auto* indicator = new TrackIndicator(i + 1, isSampler);
        trackIndicators.add(indicator);
        addAndMakeVisible(indicator);
    }
    
    // Set up swing control
    swingLabel.setText("Swing (MPC)", juce::dontSendNotification);
    swingLabel.setJustificationType(juce::Justification::centred);
    swingLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(swingLabel);
    
    swingSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    swingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    swingSlider.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
    swingSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orangered);
    addAndMakeVisible(swingSlider);
    
    // Set up lo-fi control
    loFiLabel.setText("Lo-Fi 12-bit", juce::dontSendNotification);
    loFiLabel.setJustificationType(juce::Justification::centred);
    loFiLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(loFiLabel);
    
    loFiSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    loFiSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    loFiSlider.setColour(juce::Slider::thumbColourId, juce::Colours::lightblue);
    loFiSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::blue);
    addAndMakeVisible(loFiSlider);
    
    // Set up terminal mode button
    terminalModeButton.setButtonText("Terminal Mode (192v)");
    terminalModeButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    terminalModeButton.setColour(juce::ToggleButton::tickColourId, juce::Colours::red);
    addAndMakeVisible(terminalModeButton);
    
    // Voice count display
    voiceCountLabel.setJustificationType(juce::Justification::centred);
    voiceCountLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
    voiceCountLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    addAndMakeVisible(voiceCountLabel);
    
    // Status display
    statusLabel.setText("Ready", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centred);
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    addAndMakeVisible(statusLabel);
    
    // Start update timer
    updateTimer.startTimer(100); // Update at 10Hz
    updateVoiceCountDisplay();
    
    setSize(600, 800);
}

KoreSwampAudioProcessorEditor::~KoreSwampAudioProcessorEditor()
{
}

void KoreSwampAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background gradient
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    auto bounds = getLocalBounds();
    
    // Dark gradient background
    juce::ColourGradient gradient(
        juce::Colour(0xff0a0a0a), 0, 0,
        juce::Colour(0xff2a2a2a), 0, (float)bounds.getHeight(),
        false
    );
    g.setGradientFill(gradient);
    g.fillRect(bounds);
    
    // Draw accent lines
    g.setColour(juce::Colours::orange.withAlpha(0.3f));
    g.drawLine(50, 120, getWidth() - 50, 120, 2.0f);
    g.drawLine(50, getHeight() - 100, getWidth() - 50, getHeight() - 100, 2.0f);
}

void KoreSwampAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Title area
    titleLabel.setBounds(bounds.removeFromTop(60).reduced(20, 10));
    subtitleLabel.setBounds(bounds.removeFromTop(30).reduced(20, 0));
    
    bounds.removeFromTop(40); // Spacing
    
    // Track indicators area
    auto trackArea = bounds.removeFromTop(200).reduced(40, 0);
    int trackWidth = 50;
    int trackSpacing = 5;
    int totalWidth = (trackWidth + trackSpacing) * 10 - trackSpacing;
    int startX = (getWidth() - totalWidth) / 2;
    
    for (int i = 0; i < trackIndicators.size(); ++i)
    {
        int x = startX + i * (trackWidth + trackSpacing);
        trackIndicators[i]->setBounds(x, trackArea.getY(), trackWidth, trackArea.getHeight());
    }
    
    bounds.removeFromTop(40); // Spacing
    
    // Controls area
    auto controlsArea = bounds.removeFromTop(150).reduced(60, 0);
    int controlWidth = controlsArea.getWidth() / 2;
    
    auto swingArea = controlsArea.removeFromLeft(controlWidth);
    swingLabel.setBounds(swingArea.removeFromTop(30));
    swingSlider.setBounds(swingArea.reduced(20, 10));
    
    auto loFiArea = controlsArea;
    loFiLabel.setBounds(loFiArea.removeFromTop(30));
    loFiSlider.setBounds(loFiArea.reduced(20, 10));
    
    bounds.removeFromTop(20); // Spacing
    
    // Terminal mode button
    terminalModeButton.setBounds(bounds.removeFromTop(40).reduced(150, 0));
    
    bounds.removeFromTop(20); // Spacing
    
    // Voice count and status
    voiceCountLabel.setBounds(bounds.removeFromTop(30).reduced(20, 0));
    statusLabel.setBounds(bounds.removeFromTop(30).reduced(20, 0));
}

void KoreSwampAudioProcessorEditor::updateVoiceCountDisplay()
{
    auto* voiceManager = audioProcessor.getAudioEngine().getVoiceManager();
    int activeVoices = voiceManager->getActiveVoiceCount();
    int maxVoices = voiceManager->getMaxVoices();
    
    voiceCountLabel.setText(
        "Voices: " + juce::String(activeVoices) + " / " + juce::String(maxVoices),
        juce::dontSendNotification
    );
    
    // Update status
    auto polyphonyMode = audioProcessor.getAudioEngine().getPolyphonyMode();
    juce::String modeStr = (polyphonyMode == AudioEngine::PolyphonyMode::Terminal) 
        ? "Terminal Mode (192 voices)" 
        : "Normal Mode (96 voices)";
    
    statusLabel.setText(modeStr, juce::dontSendNotification);
}

// TrackIndicator implementation
KoreSwampAudioProcessorEditor::TrackIndicator::TrackIndicator(int trackNum, bool isSampler)
    : trackNumber(trackNum), isSamplerTrack(isSampler)
{
}

void KoreSwampAudioProcessorEditor::TrackIndicator::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Background
    g.setColour(juce::Colour(0xff333333));
    g.fillRoundedRectangle(bounds.toFloat(), 5.0f);
    
    // Border
    juce::Colour borderColour = isSamplerTrack ? juce::Colours::orange : juce::Colours::cyan;
    g.setColour(borderColour);
    g.drawRoundedRectangle(bounds.toFloat().reduced(1), 5.0f, 2.0f);
    
    // Track number
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(16.0f, juce::Font::bold));
    g.drawText(juce::String(trackNumber), bounds.removeFromTop(bounds.getHeight() / 2), 
               juce::Justification::centred);
    
    // Track type
    g.setFont(juce::Font(10.0f));
    g.setColour(borderColour);
    g.drawText(isSamplerTrack ? "SMPL" : "SYNTH", bounds, juce::Justification::centred);
}

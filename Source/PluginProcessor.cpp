#include "PluginProcessor.h"
#include "PluginEditor.h"

KoreSwampAudioProcessor::KoreSwampAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, juce::Identifier("KoreSwamp"), createParameterLayout())
{
    // Get parameter pointers
    swingAmountParam = parameters.getRawParameterValue("swing");
    loFiAmountParam = parameters.getRawParameterValue("lofi");
    terminalModeParam = parameters.getRawParameterValue("terminal");
}

KoreSwampAudioProcessor::~KoreSwampAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout KoreSwampAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "swing",
        "Swing Amount",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.66f, // Default 66% MPC Swing
        "Swing",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(int(value * 100.0f)) + "%"; }
    ));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lofi",
        "Lo-Fi Amount",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f, // Default 50%
        "Lo-Fi",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(int(value * 100.0f)) + "%"; }
    ));
    
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "terminal",
        "Terminal Mode",
        false, // Default off (96 voices)
        "Terminal Mode",
        [](bool value, int) { return value ? "192 Voices" : "96 Voices"; }
    ));
    
    return layout;
}

const juce::String KoreSwampAudioProcessor::getName() const
{
    return "KORE SWAMP";
}

bool KoreSwampAudioProcessor::acceptsMidi() const
{
    return true;
}

bool KoreSwampAudioProcessor::producesMidi() const
{
    return false;
}

bool KoreSwampAudioProcessor::isMidiEffect() const
{
    return false;
}

double KoreSwampAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KoreSwampAudioProcessor::getNumPrograms()
{
    return 1;
}

int KoreSwampAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KoreSwampAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String KoreSwampAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return "Default";
}

void KoreSwampAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void KoreSwampAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    audioEngine.prepareToPlay(sampleRate, samplesPerBlock);
}

void KoreSwampAudioProcessor::releaseResources()
{
    audioEngine.releaseResources();
}

bool KoreSwampAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Only support stereo output
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    return true;
}

void KoreSwampAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Update parameters
    audioEngine.setSwingAmount(*swingAmountParam);
    audioEngine.setLoFiAmount(*loFiAmountParam);
    
    bool terminalMode = *terminalModeParam > 0.5f;
    audioEngine.setPolyphonyMode(terminalMode ? AudioEngine::PolyphonyMode::Terminal 
                                              : AudioEngine::PolyphonyMode::Normal);
    
    // Process audio
    audioEngine.processBlock(buffer, midiMessages);
}

bool KoreSwampAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* KoreSwampAudioProcessor::createEditor()
{
    return new KoreSwampAudioProcessorEditor(*this);
}

void KoreSwampAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void KoreSwampAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KoreSwampAudioProcessor();
}

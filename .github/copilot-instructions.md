# Copilot Instructions for KORE SWAMP

## Project Overview

KORE SWAMP is a professional C++/JUCE-based AUv3 mobile workstation designed specifically for DJ Swamp. It combines vintage Korg hardware sampling with modern wavetable synthesis, signature MPC swing timing, and lo-fi character processing.

### Key Architecture Components

- **10-Track Audio Engine**:
  - Tracks 1-8: 24-bit PCM Sampler (144 Korg ER-1/DDD-1 legacy assets)
  - Tracks 9-10: Monophonic Wavetable Synthesizers (64 Korg-style wavetables)
- **Effects Processing**:
  - 66% MPC Swing (default, adjustable 0-100%)
  - 12-bit Lo-Fi Saturation & Vinyl Dust (parallel processing)
- **Polyphony System**:
  - Normal Mode: 96 stereo voices
  - Terminal Mode: 192 stereo voices

## Technology Stack

- **Language**: C++17
- **Framework**: JUCE Framework 6.0+
- **Build System**: CMake 3.15+
- **Target Platform**: iOS 12.0+ (AUv3 plugin)
- **Audio Format**: 24-bit internal processing, 44.1-96 kHz sample rate

## Build Instructions

### Prerequisites
```bash
# Clone JUCE from GitHub (recommended)
git clone https://github.com/juce-framework/JUCE.git
# OR download from https://juce.com/
```

### Building for iOS/AUv3
```bash
mkdir build
cd build
cmake .. -G Xcode -DCMAKE_SYSTEM_NAME=iOS
cmake --build . --config Release
```

### Building for Desktop (Testing)
```bash
mkdir build-desktop
cd build-desktop
cmake .. -G Xcode
cmake --build . --config Release
```

## Coding Conventions

### General Guidelines

1. **C++ Standard**: Use C++17 features and idioms
2. **JUCE Patterns**: Follow JUCE framework best practices and naming conventions
3. **Memory Management**: Use JUCE smart pointers and RAII patterns
4. **Naming**:
   - Classes: PascalCase (e.g., `AudioEngine`, `KorgSampler`)
   - Methods: camelCase (e.g., `prepareToPlay`, `processBlock`)
   - Member variables: camelCase (e.g., `currentSampleRate`, `polyphonyMode`)
   - Constants: UPPER_SNAKE_CASE or constexpr (e.g., `NUM_TRACKS`)

### JUCE-Specific Guidelines

1. **Audio Processing**:
   - Always implement `prepareToPlay`, `processBlock`, and `releaseResources`
   - Use `juce::AudioBuffer<float>` for audio data
   - Handle MIDI through `juce::MidiBuffer`
   - Avoid memory allocation in the audio thread

2. **Thread Safety**:
   - Audio processing is real-time critical
   - Use `juce::CriticalSection` or lock-free structures for thread safety
   - Never block the audio thread with file I/O or other slow operations

3. **Memory Management**:
   - Use `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` for classes
   - Prefer `std::unique_ptr` and `std::shared_ptr` for ownership
   - Use `juce::OwnedArray` and `juce::ReferenceCountedArray` where appropriate

4. **UI Development**:
   - Inherit from `juce::Component` for UI elements
   - Use `juce::Graphics` for custom drawing
   - Implement proper component sizing with `resized()` and `setBounds()`

### Audio DSP Best Practices

1. **Voice Management**:
   - Implement efficient voice stealing (oldest-note-first)
   - Track active voice count for polyphony limits
   - Clean up voices properly to prevent audio glitches

2. **Effects Processing**:
   - Use parallel processing for wet/dry mix (see `LoFiProcessor`)
   - Implement smooth parameter changes to avoid clicks
   - Consider SIMD optimizations for performance-critical code

3. **Sample Handling**:
   - Support 24-bit PCM for high-quality playback
   - Implement proper ADSR envelopes
   - Handle velocity sensitivity appropriately

## File Structure

```
Swampkore/
├── .github/
│   └── copilot-instructions.md  # This file
├── Source/
│   ├── AudioEngine.h/cpp        # Main audio engine coordination
│   ├── Sampler.h/cpp           # 24-bit PCM sampler implementation
│   ├── WavetableSynth.h/cpp    # Wavetable synthesis
│   ├── SwingProcessor.h/cpp    # MPC swing timing processor
│   ├── LoFiProcessor.h/cpp     # Lo-fi & vinyl effects
│   ├── VoiceManager.h/cpp      # Polyphony management
│   ├── PluginProcessor.h/cpp   # Main AUv3 plugin processor
│   └── PluginEditor.h/cpp      # UI editor
├── Assets/                      # Sample assets (Korg sounds)
├── CMakeLists.txt              # Build configuration
└── README.md
```

## Important Constraints

1. **iOS Optimization**: Code must run efficiently on mobile ARM processors
2. **Real-Time Safe**: Audio processing code must be real-time safe (no allocations, no locks)
3. **AUv3 Compatibility**: Must comply with AUv3 plugin requirements
4. **Memory Footprint**: Be mindful of memory usage with 144+ sample slots
5. **Latency**: Minimize latency for live performance use

## Testing Guidelines

1. **Manual Testing**: Test audio output in AUv3 host applications (GarageBand, AUM, Cubasis)
2. **Voice Count**: Verify polyphony limits (96/192 voices) work correctly
3. **Effects**: Test swing and lo-fi effects at various settings
4. **MIDI**: Verify MIDI note triggering and velocity response
5. **Performance**: Monitor CPU usage and voice stealing behavior

## Common Tasks

### Adding a New Effect
1. Create new processor class inheriting appropriate JUCE base
2. Implement `prepareToPlay`, `processBlock`, `releaseResources`
3. Add to `AudioEngine` effects chain
4. Add UI controls in `PluginEditor`
5. Ensure thread-safe parameter updates

### Adding New Samples
1. Place samples in `Assets/` directory
2. Update loading logic in `Sampler` class
3. Verify 24-bit PCM format support
4. Test memory usage with multiple samples loaded

### Modifying Voice Management
1. Update `VoiceManager` class
2. Test voice stealing algorithm
3. Verify polyphony limits
4. Check for audio glitches during voice transitions

## Documentation

- Main README: `KORE_SWAMP_README.md` - Complete technical documentation
- Quick Start: `QUICKSTART.md` - Build and deployment guide
- Implementation Summary: `IMPLEMENTATION_SUMMARY.md` - Development notes

## Contact & Support

This is a bespoke project for DJ Swamp. For questions or modifications, consult the Lead DSP Architect & Creative Technologist who created this system.

---

**Remember**: KORE SWAMP is designed for professional music production. Maintain high code quality, optimize for mobile performance, and preserve the vintage Korg character in all modifications.

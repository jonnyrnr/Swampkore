# KORE SWAMP - AUv3 Mobile Workstation

**Lead DSP Architect & Creative Technologist Project**  
*Bespoke for DJ Swamp*

## Overview

KORE SWAMP is a professional C++/JUCE-based AUv3 mobile workstation designed specifically for DJ Swamp. It combines vintage Korg hardware sampling with modern wavetable synthesis, signature MPC swing timing, and lo-fi character processing.

## Architecture

### 10-Track Audio Engine

- **Tracks 1-8**: 24-bit PCM Sampler
  - Support for 144 Korg ER-1/DDD-1 legacy assets
  - High-quality 24-bit playback
  - ADSR envelope per track
  - Velocity sensitivity
  
- **Tracks 9-10**: Monophonic Wavetable Synthesizers
  - 64 Korg-style wavetables
  - Wavetable morphing
  - Lowpass filter with resonance
  - Glide/portamento control
  - ADSR envelope

### Audio Processing

#### 66% MPC Swing (Default)
- Authentic MPC-style swing timing
- Adjustable from 0% to 100%
- Default: 66% for classic MPC feel
- Grid resolution: 16th notes (configurable to 8th or quarter notes)

#### 12-bit Lo-Fi Saturation & Vinyl Dust (Parallel)
- Bit depth reduction (1-24 bits, default 12-bit)
- Analog-style saturation/distortion
- Vinyl dust simulation (random pops)
- Vinyl crackle (sustained noise texture)
- Parallel processing for blend control

### Polyphony System

- **Normal Mode**: 96 stereo voices
- **Terminal Mode**: 192 stereo voices
- Intelligent voice stealing (oldest note first)
- Real-time voice usage monitoring

## Features

### Voice Management
- Dynamic voice allocation across all tracks
- Automatic voice stealing when limit reached
- Voice usage statistics and monitoring
- Switchable polyphony modes

### Sample Management
- Support for 144 sample slots per sampler track
- Multiple audio format support (WAV, AIFF, etc.)
- 24-bit PCM playback quality
- ADSR envelope per sample

### Wavetable Synthesis
- 64 preset Korg-inspired wavetables
- Monophonic synthesis for lead sounds
- Wavetable position morphing
- Filter and envelope controls

### Effects Processing
- Swing processor with tempo sync
- Lo-fi processor with parallel wet/dry mix
- 12-bit depth reduction
- Saturation and vinyl simulation

## Technical Specifications

### Audio
- Sample Rate: 44.1 kHz - 96 kHz
- Bit Depth: 24-bit internal processing
- Channels: Stereo output
- Latency: Optimized for mobile

### File Formats
- Plugin: AUv3 (iOS)
- Audio Formats: WAV, AIFF, MP3 (24-bit PCM recommended)
- State: XML-based preset system

### System Requirements
- iOS 12.0 or later
- AUv3-compatible host application
- Recommended: iPad Pro or newer for 192-voice mode

## Building

### Prerequisites
- CMake 3.15 or higher
- JUCE Framework 6.0 or higher
- Xcode 12.0 or higher (for iOS/AUv3)
- iOS SDK 12.0 or higher

### Build Instructions

1. **Install JUCE**
   ```bash
   # Clone JUCE if not already installed
   git clone https://github.com/juce-framework/JUCE.git
   ```

2. **Configure CMake**
   ```bash
   mkdir build
   cd build
   cmake .. -G Xcode
   ```

3. **Build with Xcode**
   - Open the generated Xcode project
   - Select your target device or simulator
   - Build and run

4. **Alternative: Command Line Build**
   ```bash
   cmake --build build --config Release
   ```

### iOS Deployment

For AUv3 deployment on iOS:
1. Configure code signing in Xcode
2. Set appropriate bundle identifier
3. Enable App Groups capability
4. Build for device
5. Archive and distribute

## Usage

### Loading Samples
1. Open the plugin in your AUv3 host
2. Navigate to sampler tracks (1-8)
3. Load samples into any of the 144 slots per track
4. Assign MIDI notes to trigger samples

### Wavetable Synthesis
1. Access synth tracks (9-10)
2. Select from 64 available wavetables
3. Adjust filter, envelope, and glide parameters
4. Play monophonic lines via MIDI

### Swing Control
- Adjust the Swing slider for timing feel
- Default 66% provides classic MPC groove
- 0% = straight timing, 100% = maximum shuffle

### Lo-Fi Effect
- Control wet/dry mix with Lo-Fi slider
- Parallel processing preserves original clarity
- 12-bit depth reduction adds vintage character
- Vinyl simulation adds warmth and texture

### Polyphony Modes
- Normal Mode: 96 voices for standard use
- Terminal Mode: 192 voices for complex arrangements
- Monitor voice count in the UI

## Project Structure

```
Swampkore/
├── CMakeLists.txt          # Build configuration
├── Source/
│   ├── AudioEngine.h/cpp   # Main audio engine (10 tracks)
│   ├── Sampler.h/cpp       # 24-bit PCM sampler
│   ├── WavetableSynth.h/cpp# Monophonic wavetable synth
│   ├── SwingProcessor.h/cpp # 66% MPC swing
│   ├── LoFiProcessor.h/cpp  # 12-bit lo-fi & vinyl
│   ├── VoiceManager.h/cpp   # Polyphony management
│   ├── PluginProcessor.h/cpp# Main plugin processor
│   └── PluginEditor.h/cpp   # UI editor
├── Assets/                  # Sample assets (144 Korg sounds)
└── README.md               # This file
```

## Architecture Details

### AudioEngine Class
Central hub coordinating all tracks, effects, and voice management.

### Track System
- 8 independent sampler tracks
- 2 independent synth tracks
- Each track has independent processing
- Mixed to stereo output

### Effects Chain
1. MIDI input → Swing Processor
2. Track synthesis/sampling
3. Lo-Fi Processor (parallel)
4. Stereo output

### Voice Allocation
- Global voice pool shared across all tracks
- Priority-based allocation
- Oldest-note-first stealing algorithm
- Configurable polyphony limits

## Performance Optimization

- Efficient voice stealing algorithm
- Minimal memory allocation during playback
- SIMD optimizations where applicable
- Optimized for ARM processors (iOS)

## Credits

**Developer**: Lead DSP Architect & Creative Technologist  
**Commissioned by**: DJ Swamp  
**Framework**: JUCE  
**Vintage Assets**: Korg ER-1/DDD-1 legacy sounds

## License

Proprietary - Developed exclusively for DJ Swamp

## Version History

### Version 1.0.0 (Initial Release)
- 10-track audio engine implementation
- 24-bit PCM sampler (8 tracks)
- Monophonic wavetable synth (2 tracks)
- 66% MPC swing processor
- 12-bit lo-fi saturation & vinyl dust
- 96/192 voice polyphony system
- AUv3 iOS support
- Professional UI with track visualization

## Support

For technical support or feature requests, please contact the development team.

---

**// KORE SWAMP** - Crafted with precision for DJ Swamp

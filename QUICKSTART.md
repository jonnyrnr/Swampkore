# KORE SWAMP - Quick Start Guide

## Installation & Setup

### Step 1: Prerequisites

Before building KORE SWAMP, ensure you have:

1. **Development Environment**
   - macOS 10.15 or later (for iOS development)
   - Xcode 12.0 or later
   - CMake 3.15 or later
   - iOS SDK 12.0 or later

2. **JUCE Framework**
   ```bash
   # Option 1: Install via Package Manager
   brew install juce
   
   # Option 2: Clone from GitHub
   cd ~/Development
   git clone https://github.com/juce-framework/JUCE.git
   ```

3. **Repository**
   ```bash
   git clone https://github.com/jonnyrnr/Swampkore.git
   cd Swampkore
   ```

### Step 2: Configure Build

#### For iOS/AUv3 Target

```bash
# Create build directory
mkdir build
cd build

# Generate Xcode project
cmake .. -G Xcode -DCMAKE_SYSTEM_NAME=iOS

# If JUCE is not in default location, specify path:
cmake .. -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DJUCE_DIR=~/Development/JUCE
```

#### For Standalone Desktop (Testing)

```bash
mkdir build-desktop
cd build-desktop
cmake .. -G Xcode
```

### Step 3: Build the Project

#### Using Xcode GUI

1. Open `build/KoreSwamp.xcodeproj` in Xcode
2. Select your target device or simulator
3. Configure code signing:
   - Project settings → Signing & Capabilities
   - Select your development team
4. Build: Product → Build (⌘B)
5. Run: Product → Run (⌘R)

#### Using Command Line

```bash
# Build Release configuration
cmake --build build --config Release

# Or for Debug
cmake --build build --config Debug
```

### Step 4: Deploy to iOS Device

1. Connect your iOS device via USB
2. Select device in Xcode
3. Build and run (⌘R)
4. Trust developer profile on device if prompted

### Step 5: Use in AUv3 Host

1. Open your favorite AUv3-compatible host:
   - GarageBand
   - AUM (Audio Mixer)
   - Cubasis
   - BeatMaker 3
   - Auria Pro
   
2. Add KORE SWAMP as an AUv3 instrument
3. Start making music!

## Quick Feature Overview

### Track Layout

```
Track 1-8:  Sampler Tracks (24-bit PCM)
            └─ 144 sample slots across all tracks
            └─ ER-1/DDD-1 Korg legacy sounds

Track 9-10: Wavetable Synths (Monophonic)
            └─ 64 Korg-style wavetables
            └─ Filter, ADSR, Glide controls
```

### Default Settings

- **Swing**: 66% (MPC classic feel)
- **Lo-Fi**: 50% wet/dry mix
- **Bit Depth**: 12-bit (lo-fi effect)
- **Polyphony**: 96 voices (Normal Mode)
- **Sample Rate**: 44.1 kHz

### Controls

1. **Swing Knob**
   - Range: 0-100%
   - Default: 66% (MPC style)
   - Adjust for tighter or looser groove

2. **Lo-Fi Knob**
   - Range: 0-100%
   - Controls parallel wet/dry mix
   - Adds 12-bit character + vinyl dust

3. **Terminal Mode Button**
   - OFF: 96 stereo voices
   - ON: 192 stereo voices
   - Use for complex arrangements

### MIDI Control

- **Note On/Off**: Trigger samples and synths
- **Velocity**: Controls sample/synth amplitude
- **MIDI Channel**: All channels supported
- **Pitch Bend**: Affects synth tracks (9-10)

## Loading Samples

### Method 1: Drag & Drop (Future Feature)
- Drag audio files onto track slots
- Supports WAV, AIFF formats

### Method 2: File Browser (Future Feature)
- Click on sample slot
- Navigate file system
- Select 24-bit WAV or AIFF

### Method 3: Programmatic (Current)
```cpp
// Access sampler track
auto* track1 = audioEngine.getSamplerTrack(0);

// Load sample
juce::File sampleFile("/path/to/kick.wav");
track1->loadSample(0, sampleFile);
```

## Performance Tips

### Optimizing Voice Count

- Start with 96 voices (Normal Mode)
- Switch to Terminal Mode only when needed
- Monitor voice count in UI
- Complex arrangements may need voice stealing

### Memory Management

- Load only necessary samples
- Use shorter samples when possible
- Mono samples use less memory than stereo
- 144 samples can use significant RAM

### Latency Reduction

- Use lowest buffer size your device supports
- Close background apps
- Enable airplane mode during performance
- Use wired MIDI if possible

## Troubleshooting

### Build Issues

**Problem**: CMake can't find JUCE
```bash
# Solution: Specify JUCE path explicitly
cmake .. -DJUCE_DIR=/path/to/JUCE
```

**Problem**: Code signing fails
- Solution: Configure development team in Xcode
- Project → Signing & Capabilities

**Problem**: iOS deployment fails
- Solution: Check iOS deployment target (≥12.0)
- Verify device is in developer mode

### Audio Issues

**Problem**: No sound output
- Check AUv3 host routing
- Verify track is not muted
- Check device volume

**Problem**: Crackling/dropouts
- Increase buffer size
- Close other apps
- Reduce polyphony count

**Problem**: High CPU usage
- Reduce active voices
- Use Normal Mode (96 voices)
- Optimize lo-fi effect settings

## Next Steps

1. **Load Your Samples**
   - Populate Assets/ directory with Korg ER-1/DDD-1 samples
   - Organize by category (kicks, snares, hats, etc.)

2. **Create Patches**
   - Experiment with wavetable synths
   - Adjust ADSR envelopes
   - Set up filter sweeps

3. **Dial In Effects**
   - Find your sweet spot for swing amount
   - Balance lo-fi effect to taste
   - Try different combinations

4. **Make Music!**
   - KORE SWAMP is ready for production
   - Built for DJ Swamp's workflow
   - Embrace the vintage Korg character

## Support & Resources

- **Documentation**: See KORE_SWAMP_README.md
- **Assets Guide**: See Assets/README.md
- **JUCE Framework**: https://juce.com/
- **CMake**: https://cmake.org/

## Credits

Designed and developed specifically for DJ Swamp by a Lead DSP Architect & Creative Technologist.

---

**// KORE SWAMP** - *The Swamp is Calling*

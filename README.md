# Swampkore

KORE SWAMP - AUv3 Mobile Workstation for DJ Swamp

A professional C++/JUCE-based AUv3 mobile workstation combining vintage Korg hardware sampling with modern wavetable synthesis, signature MPC swing timing, and lo-fi character processing.

## Quick Links

- [Full Documentation](KORE_SWAMP_README.md) - Complete architecture overview and technical specifications
- [Quick Start Guide](QUICKSTART.md) - Installation and setup instructions
- [Implementation Summary](IMPLEMENTATION_SUMMARY.md) - Detailed implementation status

## Overview

**KORE SWAMP** is a 10-track audio engine featuring:
- **Tracks 1-8**: 24-bit PCM Sampler with 144 Korg ER-1/DDD-1 legacy assets
- **Tracks 9-10**: Monophonic Wavetable Synthesizers with 64 Korg-style wavetables
- **66% MPC Swing** (default, adjustable)
- **12-bit Lo-Fi Saturation** with vinyl dust simulation
- **96/192 Stereo Voice Polyphony** (Normal/Terminal modes)

## Key Features

✅ Professional 10-track audio engine  
✅ 24-bit PCM sampling (8 tracks)  
✅ Monophonic wavetable synthesis (2 tracks)  
✅ Authentic MPC swing timing  
✅ Lo-fi character processing  
✅ Intelligent voice management  
✅ AUv3 plugin format for iOS  

## Building

```bash
# Install JUCE framework
git clone https://github.com/juce-framework/JUCE.git

# Configure and build
mkdir build
cd build
cmake .. -G Xcode
cmake --build . --config Release
```

See [QUICKSTART.md](QUICKSTART.md) for detailed build instructions.

## Credits

**Developer**: Lead DSP Architect & Creative Technologist  
**Commissioned by**: DJ Swamp  
**Framework**: JUCE  
**Platform**: iOS (AUv3)

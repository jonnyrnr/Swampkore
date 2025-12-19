# KORE SWAMP Sample Assets

## Directory Structure

This directory is designed to hold the 144 Korg ER-1/DDD-1 legacy sample assets for use with the KORE SWAMP sampler tracks.

### Organization

```
Assets/
├── KorgER1/           # Korg ER-1 drum sounds
│   ├── Kicks/        # Kick drum samples
│   ├── Snares/       # Snare drum samples
│   ├── Hats/         # Hi-hat samples
│   ├── Toms/         # Tom drum samples
│   ├── Claps/        # Clap samples
│   └── Percussion/   # Other percussion
│
└── KorgDDD1/         # Korg DDD-1 drum sounds
    ├── Kicks/
    ├── Snares/
    ├── Hats/
    ├── Cymbals/
    ├── Toms/
    └── Effects/
```

### Sample Requirements

- **Format**: WAV or AIFF (24-bit PCM recommended)
- **Sample Rate**: 44.1 kHz or 48 kHz
- **Bit Depth**: 24-bit for optimal quality
- **Channels**: Mono or Stereo
- **Total Slots**: 144 samples across all 8 sampler tracks (18 per track average)

### Loading Samples

Samples can be loaded programmatically or via the UI:

1. **Programmatic Loading**:
   ```cpp
   auto* samplerTrack = audioEngine.getSamplerTrack(0);
   juce::File sampleFile("path/to/sample.wav");
   samplerTrack->loadSample(slotIndex, sampleFile);
   ```

2. **Via UI** (when implemented):
   - Drag and drop samples onto track slots
   - Browse file system to load samples
   - Manage 144 slots across 8 tracks

### Sample Naming Convention

Recommended naming format:
```
[Instrument]_[Type]_[Variation]_[Note].wav

Examples:
- Kick_808_Hard_C1.wav
- Snare_ER1_Tight_D1.wav
- HiHat_Closed_Vintage_F#1.wav
```

### Korg ER-1 Legacy Sounds

The Korg ER-1 (Electribe R) was known for:
- Analog-modeled drum sounds
- Aggressive, edgy character
- Synthetic kick drums
- Sharp snares and hi-hats

### Korg DDD-1 Legacy Sounds

The Korg DDD-1 (Dynamic Digital Drums) featured:
- Early digital drum samples
- Punchy, compressed character
- Classic 80s drum machine sounds
- Lo-fi digital warmth

## Asset Management

### Track Assignment

- **Track 1**: Kicks and low percussion
- **Track 2**: Snares and claps
- **Track 3**: Hi-hats (closed)
- **Track 4**: Hi-hats (open) and cymbals
- **Track 5**: Toms and timbales
- **Track 6**: Percussion and effects
- **Track 7**: Additional sounds
- **Track 8**: Special effects and layers

### Memory Considerations

- Each sample loaded consumes RAM
- 144 samples @ 24-bit stereo can use significant memory
- Optimize sample length to conserve resources
- Consider mono samples for simpler sounds

### MIDI Mapping

Standard MIDI note layout:
- C0 - B1: Low sounds (kicks, toms)
- C2 - B3: Mid sounds (snares, claps)
- C4 - B5: High sounds (hats, cymbals)
- C6+: Effects and special sounds

## Copyright Notice

This directory should contain only legally acquired samples:
- Licensed Korg content
- Original recordings
- Public domain samples
- Properly licensed third-party content

Do not include copyrighted material without proper authorization.

## Future Enhancements

- Automatic sample discovery and loading
- Sample preview functionality
- Waveform display in UI
- Sample pitch and time manipulation
- Multi-sample velocity layers
- Round-robin sample rotation

---

**Note**: This is a placeholder directory. Populate with your own legally acquired Korg ER-1 and DDD-1 samples or compatible alternatives.

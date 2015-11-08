/*
  ==============================================================================

    AdditiveSynth.h
    Created: 5 Nov 2015 4:34:04pm
    Author:  Francesco

  ==============================================================================
*/

#ifndef ADDITIVESYNTH_H_INCLUDED
#define ADDITIVESYNTH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class AdditiveSound : public SynthesiserSound
{
public:
  AdditiveSound() {}
  bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
  bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

struct LFO
{
  float currentPhase;
  float phaseDelta;

  typedef enum
  {
    Sine,
    RisingSawtooth,
    FallingSawtooth,
    Triangle,
    Square
  } Waveform;

  Waveform waveform;
};

class AdditiveVoice : public SynthesiserVoice
{
public:
  AdditiveVoice(DreamscopeAudioProcessor* processor)
    : currentPhase(0), phaseDelta(0), level(0), baseFreq(0), currentTime(0), processor(processor)
  {
    memset(stoppedLevel, 0, 64 * sizeof(float));
    memset(adsrLevels, 0, 64 * sizeof(float));
    wavetable = processor->wavetable;

    samplingFrequency = getSampleRate();

    maxAttackTime = 10 * samplingFrequency;
    maxDecayTime = 5 * samplingFrequency;
    maxReleaseTime = 10 * samplingFrequency;
    maxSustainTime = 20 * samplingFrequency;
  }

  bool canPlaySound(SynthesiserSound* sound) override
  {
    return dynamic_cast<AdditiveSound*> (sound) != nullptr;
  }

  void startNote(int midiNoteNumber, float velocity,
    SynthesiserSound* /*sound*/,
    int /*currentPitchWheelPosition*/) override
  {
    midiNote = midiNoteNumber;
    currentPhase = 0.0;
    currentTime = 0.0;
    level = velocity * 0.15;

    baseFreq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    double cyclesPerSample = baseFreq / getSampleRate();
    samplingFrequency = getSampleRate();

    maxAttackTime = 10 * samplingFrequency;
    maxDecayTime = 5 * samplingFrequency;
    maxReleaseTime = 10 * samplingFrequency;
    maxSustainTime = 20 * samplingFrequency;

    phaseDelta = cyclesPerSample;
    stopped = false;
    playing = true;
  }

  void stopNote(float /*velocity*/, bool allowTailOff) override
  {
    if (allowTailOff)
    {
      stopped = true;
      playing = true;
      memcpy(stoppedLevel, adsrLevels, 64 * sizeof(float));
      currentTime = 0.0;
    }
    else
    {
      // we're being told to stop playing immediately, so reset everything..
      playing = false;
      clearCurrentNote();
      phaseDelta = 0.0;
      currentTime = 0.0;
      currentPhase = 0.0;
    }
  }

  void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
  {
    if (!playing)
    {
      return;
    }

    if (phaseDelta != 0.0)
    {
      while (--numSamples >= 0)
      {
        float currentSample = 0;
        for (int i = 0; i < 64; i++)
        {
          int n = i + 1;
          if (phaseDelta * n >= 0.5)
          {
            // By eliminating all the frequencies above the Nyquist frequency
            // we also eliminate aliasing. And we save CPU cycles. Woo!
            goto exit;
          }

          float harmonicLevel = getADSRValue(processor->attackTimes[i],
            processor->attackLevels[i],
            processor->decayTimes[i],
            processor->decayLevels[i],
            processor->sustainTimes[i],
            processor->releaseTimes[i], currentTime, stoppedLevel[i]);
          adsrLevels[i] = harmonicLevel;
          currentSample += getWavetableValue(currentPhase * n) * level * harmonicLevel;
        }
      exit:

        for (int i = outputBuffer.getNumChannels(); --i >= 0;)
          outputBuffer.addSample(i, startSample, currentSample);

        currentPhase += phaseDelta;
        if (currentPhase > 1)
        {
          currentPhase -= 1.0;
        }
        currentTime += 1;
        ++startSample;
      }
    }
  }

  void pitchWheelMoved(int newValue) override
  {
    if (!stopped)
    {
      int lowerPitch = -2;
      int higherPitch = +2;
      float lowerPitchFreq = MidiMessage::getMidiNoteInHertz(midiNote + lowerPitch);
      float higherPitchFreq = MidiMessage::getMidiNoteInHertz(midiNote + higherPitch);

      int offset = newValue - 0x1fff;
      if (offset < 0)
      {
        float newFreq = lerp(baseFreq, lowerPitchFreq, -((float)offset / 0x1fff));
        phaseDelta = newFreq / samplingFrequency;
      }
      else
      {
        float newFreq = lerp(baseFreq, higherPitchFreq, ((float)offset / 0x1fff));
        phaseDelta = newFreq / samplingFrequency;
      }
    }
  }

  void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
  {
    // not implemented for the purposes of this demo!
  }

private:
  double currentPhase, phaseDelta, level, baseFreq;
  double samplingFrequency, currentTime;
  float adsrLevels[64];
  bool stopped;
  bool playing;
  float stoppedLevel[64];
  float* wavetable;
  int midiNote;

  DreamscopeAudioProcessor* processor;

  inline float lerp(float a, float b, float w)
  {
    return (1 - w) * a + w * b;
  }

  // Times in seconds
  float maxAttackTime;
  float maxDecayTime;
  float maxReleaseTime;
  float maxSustainTime;

  float getADSRValue(float atkT, float atkL, float decT, float decL, float susT, float relT, float t, float lastValue)
  {
    float attackTime = maxAttackTime * atkT;
    float decayTime = maxDecayTime * decT;
    float sustainTime = (susT == 1 ? INFINITY : susT * maxSustainTime);
    float releaseTime = maxReleaseTime * relT;
    if (stopped)
    {
      if (relT == 0)
      {
        return 0;
      }

      float pos = t / releaseTime;
      if (pos > 1)
      {
        return 0;
      }

      return lerp(lastValue, 0, pos);
    }

    if (t < attackTime)
    {
      // Attack phase
      float pos = t / attackTime;
      return atkL * pos;
    }
    else if (t < attackTime + decayTime)
    {
      // Decay phase
      float t2 = (t - attackTime);
      float pos = t2 / decayTime;
      return lerp(atkL, decL, pos);
    }
    else if (t < attackTime + decayTime + sustainTime)
    {
      // Sustain phase
      return decL;
    }
    else
    {
      // Release phase
      if (releaseTime == 0)
      {
        return 0;
      }

      float t2 = t - (attackTime + decayTime + sustainTime);
      float pos = t2 / releaseTime;

      return lerp(decL, 0, pos);
    }
  }

  inline float getWavetableValue(float phase)
  {
    return wavetable[((int)(phase * 2048) % 2048)];
  }
};

#endif  // ADDITIVESYNTH_H_INCLUDED

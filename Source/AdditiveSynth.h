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

class AdditiveVoice : public SynthesiserVoice
{
public:
  AdditiveVoice(DreamscopeAudioProcessor* processor)
    : currentPhase(0), phaseDelta(0), level(0), baseFreq(0), currentTime(0), processor(processor)
  {
    memset(stoppedLevel, 0, 64 * sizeof(float));
    memset(adsrLevels, 0, 64 * sizeof(float));
  }

  bool canPlaySound(SynthesiserSound* sound) override
  {
    return dynamic_cast<AdditiveSound*> (sound) != nullptr;
  }

  void startNote(int midiNoteNumber, float velocity,
    SynthesiserSound* /*sound*/,
    int /*currentPitchWheelPosition*/) override
  {
    currentPhase = 0.0;
    currentTime = 0.0;
    level = velocity * 0.15;
    tailOff = 0.0;

    baseFreq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    double cyclesPerSample = baseFreq / getSampleRate();
    samplingFrequency = getSampleRate();

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
          float harmonicLevel;
          if (!stopped)
          {
            harmonicLevel = getADSRValue(processor->attackTimes[i],
              processor->attackLevels[i],
              processor->decayTimes[i],
              processor->decayLevels[i],
              processor->sustainTimes[i],
              processor->releaseTimes[i], currentTime);
            adsrLevels[i] = harmonicLevel;
          }
          else
          {
            harmonicLevel = getADSRValueStopped(stoppedLevel[i], processor->releaseTimes[i], currentTime);
          }
          currentSample += getWavetableValue(currentPhase * n) * level * harmonicLevel;
        }

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

  void pitchWheelMoved(int /*newValue*/) override
  {
    // not implemented for the purposes of this demo!
  }

  void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
  {
    // not implemented for the purposes of this demo!
  }

private:
  double currentPhase, phaseDelta, level, tailOff, baseFreq;
  double samplingFrequency, currentTime;
  float adsrLevels[64];
  bool stopped;
  bool playing;
  float stoppedLevel[64];

  DreamscopeAudioProcessor* processor;

  inline float lerp(float a, float b, float w)
  {
    return (1 - w) * a + w * b;
  }

  float getADSRValue(float atkT, float atkL, float decT, float decL, float susT, float relT, float t)
  {
    // Times in seconds
    float maxAttackTime = 10 * samplingFrequency;
    float maxDecayTime = 5 * samplingFrequency;
    float maxReleaseTime = 10 * samplingFrequency;

    float attackTime = maxAttackTime * atkT;
    float decayTime = maxDecayTime * decT;
    float sustainTime = abs((susT / (susT - 1)));
    float releaseTime = maxReleaseTime * relT;

    // For sustain times we use the function
    //             |    x    |
    // length(x) = | ------- |
    //             | (x - 1) |
    // So that for an x of 1.0, we get infinite sustain

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
      float t2 = t - (attackTime + decayTime + sustainTime);
      float pos = t2 / releaseTime;
      if (releaseTime == 0)
        return 0;

      return lerp(decL, 0, pos);
    }
  }

  float getADSRValueStopped(float lastValue, float relT, float timeFromStop)
  {
    if (relT == 0)
    {
      return 0;
    }

    float maxReleaseTime = 10 * samplingFrequency;
    float releaseTime = maxReleaseTime * relT;

    float pos = timeFromStop / releaseTime;
    if (pos > 1)
    {
      return 0;
    }

    return lerp(lastValue, 0, pos);
  }

  float getWavetableValue(float phase)
  {
    int index = ((int)(phase * 2048) % 2047);
    return processor->wavetable[index];
  }
};

#endif  // ADDITIVESYNTH_H_INCLUDED

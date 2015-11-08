/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AdditiveSynth.h"


//==============================================================================
DreamscopeAudioProcessor::DreamscopeAudioProcessor()
{
  attackTimes = (float*)malloc(64 * sizeof(float));
  memset(attackTimes, 0, 64 * sizeof(float));
  attackLevels = (float*)malloc(64 * sizeof(float));
  memset(attackLevels, 0, 64 * sizeof(float));
  decayTimes = (float*)malloc(64 * sizeof(float));
  memset(decayTimes, 0, 64 * sizeof(float));
  decayLevels = (float*)malloc(64 * sizeof(float));
  memset(decayLevels, 0, 64 * sizeof(float));
  sustainTimes = (float*)malloc(64 * sizeof(float));
  memset(sustainTimes, 0, 64 * sizeof(float));
  releaseTimes = (float*)malloc(64 * sizeof(float));
  memset(releaseTimes, 0, 64 * sizeof(float));

  wavetable = (float*)malloc(2048 * sizeof(float));
  for (int i = 0; i < 2048; i++)
  {
    double phase = (double)i / 2048;
    wavetable[i] = (float)sin(double_Pi * 2 * phase);
  }

  const int numVoices = 16;

  // Add some voices...
  for (int i = numVoices; --i >= 0;)
    synth.addVoice(new AdditiveVoice(this));

  // ..and give the synth a sound to play
  synth.addSound(new AdditiveSound());

}

DreamscopeAudioProcessor::~DreamscopeAudioProcessor()
{
  free(attackTimes);
  free(attackLevels);
  free(decayTimes);
  free(decayLevels);
  free(sustainTimes);
  free(releaseTimes);
  free(wavetable);
}

//==============================================================================
const String DreamscopeAudioProcessor::getName() const
{
  return JucePlugin_Name;
}

const String DreamscopeAudioProcessor::getInputChannelName(int channelIndex) const
{
  return String(channelIndex + 1);
}

const String DreamscopeAudioProcessor::getOutputChannelName(int channelIndex) const
{
  return channels[channelIndex];
}

bool DreamscopeAudioProcessor::isInputChannelStereoPair(int index) const
{
  return true;
}

bool DreamscopeAudioProcessor::isOutputChannelStereoPair(int index) const
{
  return true;
}

bool DreamscopeAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool DreamscopeAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool DreamscopeAudioProcessor::silenceInProducesSilenceOut() const
{
  return false;
}

double DreamscopeAudioProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int DreamscopeAudioProcessor::getNumPrograms()
{
  return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int DreamscopeAudioProcessor::getCurrentProgram()
{
  return 0;
}

void DreamscopeAudioProcessor::setCurrentProgram(int index)
{
}

const String DreamscopeAudioProcessor::getProgramName(int index)
{
  return "hello world";
}

void DreamscopeAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void DreamscopeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  synth.setCurrentPlaybackSampleRate(sampleRate);
  keyboardState.reset();

  reset();
}

void DreamscopeAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
  keyboardState.reset();
}

void DreamscopeAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{

  const int numSamples = buffer.getNumSamples();
  keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

  // and now get our synth to process these midi events and generate its output.
  synth.renderNextBlock(buffer, midiMessages, 0, numSamples);

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // I've added this to avoid people getting screaming feedback
  // when they first compile the plugin, but obviously you don't need to
  // this code if your algorithm already fills all the output channels.
  /*for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
      buffer.clear (i, 0, numSamples);*/

      // This is the place where you'd normally do the guts of your plugin's
      // audio processing...
      /*for (int channel = 0; channel < getNumInputChannels(); ++channel)
      {
          float* channelData = buffer.getWritePointer (channel);

          // ..do something to the data...
      }*/
}

//==============================================================================
bool DreamscopeAudioProcessor::hasEditor() const
{
  return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DreamscopeAudioProcessor::createEditor()
{
  return new DreamscopeAudioProcessorEditor(*this);
}

//==============================================================================
void DreamscopeAudioProcessor::getStateInformation(MemoryBlock& destData)
{
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void DreamscopeAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new DreamscopeAudioProcessor();
}

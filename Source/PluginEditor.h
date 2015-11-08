/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MultiSliderComponent.h"


//==============================================================================
/**
*/
class DreamscopeAudioProcessorEditor : public AudioProcessorEditor,
                                       public MultiSliderComponent::Listener
{
public:
  DreamscopeAudioProcessorEditor(DreamscopeAudioProcessor&);
  ~DreamscopeAudioProcessorEditor();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;

  void sliderValueChanged(MultiSliderComponent* c, int n, float value) override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  DreamscopeAudioProcessor& processor;
  TabbedComponent* tabs;
  MultiSliderComponent* attackTimeSliders;
  MultiSliderComponent* attackLevelSliders;
  MultiSliderComponent* decayTimeSliders;
  MultiSliderComponent* decayLevelSliders;
  MultiSliderComponent* sustainTimeSliders;
  MultiSliderComponent* releaseTimeSliders;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DreamscopeAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED

/*
  ==============================================================================

    LFOEditorComponent.cpp
    Created: 6 Nov 2015 2:59:58pm
    Author:  Francesco

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOEditorComponent.h"

//==============================================================================
LFOEditorComponent::LFOEditorComponent()
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  sliders = new MultiSliderComponent(64, 11);
  adsrEditor = new ADSREditor();
  addAndMakeVisible(sliders);
  addAndMakeVisible(adsrEditor);
}

LFOEditorComponent::~LFOEditorComponent()
{ }

void LFOEditorComponent::paint(Graphics& g)
{ }

void LFOEditorComponent::resized()
{
  int width = getWidth();
  int height = getHeight();
  adsrEditor->setBounds(0, 0, width, height / 2);
  sliders->setBounds(0, height / 2, width, height / 2);
}

ADSREditor::ADSRValues LFOEditorComponent::getADSRValues()
{
  return adsrEditor->getADSRValues();
}

float* LFOEditorComponent::getSliderValues()
{
  float values[64];
  for (int i = 0; i < 64; i++)
  {
    values[i] = sliders->getSliderValue(i);
  }
  return values;
}

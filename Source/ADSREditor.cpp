/*
  ==============================================================================

    ADSREditor.cpp
    Created: 6 Nov 2015 5:45:40pm
    Author:  Francesco

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ADSREditor.h"

//==============================================================================
ADSREditor::ADSREditor()
{
  atkTimeKnob = new Slider(Slider::SliderStyle::Rotary, Slider::NoTextBox);
  atkLvlKnob = new Slider(Slider::SliderStyle::Rotary, Slider::NoTextBox);
  decTimeKnob = new Slider(Slider::SliderStyle::Rotary, Slider::NoTextBox);
  decLvlKnob = new Slider(Slider::SliderStyle::Rotary, Slider::NoTextBox);
  susTimeKnob = new Slider(Slider::SliderStyle::Rotary, Slider::NoTextBox);
  relTimeKnob = new Slider(Slider::SliderStyle::Rotary, Slider::NoTextBox);
  adsrViewer = new ADSRViewer();

  atkTimeLabel = new Label(String::empty, "Attack Time");
  atkLvlLabel = new Label(String::empty, "Attack Level");
  decTimeLabel = new Label(String::empty, "Decay Time");
  decLvlLabel = new Label(String::empty, "Decay Level");
  susTimeLabel = new Label(String::empty, "Sustain Time");
  relTimeLabel = new Label(String::empty, "Release Time");

  addAndMakeVisible(atkTimeKnob);
  addAndMakeVisible(atkLvlKnob);
  addAndMakeVisible(decTimeKnob);
  addAndMakeVisible(decLvlKnob);
  addAndMakeVisible(susTimeKnob);
  addAndMakeVisible(relTimeKnob);
  addAndMakeVisible(adsrViewer);

  addAndMakeVisible(atkTimeLabel);
  addAndMakeVisible(atkLvlLabel);
  addAndMakeVisible(decTimeLabel);
  addAndMakeVisible(decLvlLabel);
  addAndMakeVisible(susTimeLabel);
  addAndMakeVisible(relTimeLabel);

  atkTimeKnob->addListener(this);
  atkTimeKnob->setRange(0, 1);
  atkTimeLabel->attachToComponent(atkTimeKnob, false);

  atkLvlKnob->addListener(this);
  atkLvlKnob->setRange(0, 1);
  atkLvlLabel->attachToComponent(atkLvlKnob, false);

  decTimeKnob->addListener(this);
  decTimeKnob->setRange(0, 1);
  decTimeLabel->attachToComponent(decTimeKnob, false);

  decLvlKnob->addListener(this);
  decLvlKnob->setRange(0, 1);
  decLvlLabel->attachToComponent(decLvlKnob, false);

  susTimeKnob->addListener(this);
  susTimeKnob->setRange(0, 1);
  susTimeLabel->attachToComponent(susTimeKnob, false);

  relTimeKnob->addListener(this);
  relTimeKnob->setRange(0, 1);
  relTimeLabel->attachToComponent(relTimeKnob, false);
}

ADSREditor::~ADSREditor()
{
}

void ADSREditor::paint(Graphics& g)
{

}

void ADSREditor::resized()
{
  int height = getHeight();
  int width = getWidth();

  int hDistance = 70;
  int vDistance = 70;

  adsrViewer->setBounds(0, 0, width / 2, height);

  atkTimeKnob->setBounds(width / 2 + hDistance * 0, 20 + vDistance * 0, 70, 40);
  atkLvlKnob ->setBounds(width / 2 + hDistance * 0, 20 + vDistance * 1, 70, 40);

  decTimeKnob->setBounds(width / 2 + hDistance * 1, 20 + vDistance * 0, 70, 40);
  decLvlKnob->setBounds(width / 2 + hDistance * 1, 20 + vDistance * 1, 70, 40);

  susTimeKnob->setBounds(width / 2 + hDistance * 2, 20 + vDistance * 0, 70, 40);
  relTimeKnob->setBounds(width / 2 + hDistance * 2, 20 + vDistance * 1, 70, 40);
}

void ADSREditor::sliderValueChanged(Slider* slider)
{
  adsrViewer->setValues(atkTimeKnob->getValue(),
    atkLvlKnob->getValue(),
    decTimeKnob->getValue(),
    decLvlKnob->getValue(),
    susTimeKnob->getValue(),
    relTimeKnob->getValue());
}

ADSREditor::ADSRValues ADSREditor::getADSRValues()
{
  ADSREditor::ADSRValues vals;
  vals.AttackTime = atkTimeKnob->getValue();
  vals.AttackLevel = atkLvlKnob->getValue();
  vals.DecayTime = decTimeKnob->getValue();
  vals.DecayLevel = decLvlKnob->getValue();
  vals.SustainTime = susTimeKnob->getValue();
  vals.ReleaseTime = relTimeKnob->getValue();

  return vals;
}

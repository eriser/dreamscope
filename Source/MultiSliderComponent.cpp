/*
  ==============================================================================

    MultiSliderComponent.cpp
    Created: 6 Nov 2015 1:10:27pm
    Author:  Francesco

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MultiSliderComponent.h"
#include <string>

//==============================================================================
MultiSliderComponent::MultiSliderComponent(int numSliders, int sliderWidth, bool log)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  listener = nullptr;
  this->numSliders = numSliders;
  this->sliderWidth = sliderWidth;
  sliderValues = std::vector<float>(numSliders);
  sliders = OwnedArray<Slider>();
  for (int i = 0; i < numSliders; i++)
  {
    Slider* slider = new Slider(Slider::SliderStyle::LinearBarVertical, Slider::NoTextBox);
    addAndMakeVisible(slider);

    slider->setName(String(i));
    slider->setRange(0, 1);
    slider->setBounds(sliderWidth * i + 2, 5, 10, getHeight() - 10);
    slider->addListener(this);
    if (log)
    {
      slider->setSkewFactorFromMidPoint(0.1);
    }
    sliders.add(slider);
  }
}

MultiSliderComponent::~MultiSliderComponent()
{

}

void MultiSliderComponent::paint(Graphics& g)
{

}

void MultiSliderComponent::resized()
{
  for (int i = 0; i < numSliders; i++)
  {
    sliders[i]->setBounds(i * sliderWidth + 2, 5, 10, getHeight() - 10);
  }
}

void MultiSliderComponent::sliderValueChanged(Slider* slider)
{
  int i = std::stoi(slider->getName().toStdString());
  float val = (float)slider->getValue();
  sliderValues[i] = val;
  if (listener != nullptr)
  {
    listener->sliderValueChanged(this, i, val);
  }
}

float MultiSliderComponent::getSliderValue(int n)
{
  if (n > numSliders)
  {
    throw new std::exception("Argument out of range");
  }
  return sliderValues[n];
}

void MultiSliderComponent::setSliderValues(float values[])
{
  for (int i = 0; i < numSliders; i++)
  {
    sliders[i]->setValue(values[i], NotificationType::dontSendNotification);
  }
}

void MultiSliderComponent::setListener(Listener* listener)
{
  this->listener = listener;
}
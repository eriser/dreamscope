/*
  ==============================================================================

    MultiSliderComponent.h
    Created: 6 Nov 2015 1:10:27pm
    Author:  Francesco

  ==============================================================================
*/

#ifndef MULTISLIDERCOMPONENT_H_INCLUDED
#define MULTISLIDERCOMPONENT_H_INCLUDED

#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MultiSliderComponent : public Component,
                             public Slider::Listener
{
public:
    MultiSliderComponent(int numSliders, int sliderWidth, bool log = false);
    ~MultiSliderComponent();

    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    
    float getSliderValue(int n);
    void setSliderValues(float[]);

    virtual class Listener
    {
    public:
      virtual ~Listener() {};
      virtual void sliderValueChanged(MultiSliderComponent* c, int n, float value) = 0;
    };

    void setListener(Listener* listener);

private:
  std::vector<float> sliderValues;
  OwnedArray<Slider> sliders;
  Listener* listener;
  int numSliders;
  int sliderWidth;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiSliderComponent)
};


#endif  // MULTISLIDERCOMPONENT_H_INCLUDED

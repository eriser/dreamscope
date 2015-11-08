/*
  ==============================================================================

    ADSREditor.h
    Created: 6 Nov 2015 5:45:40pm
    Author:  Francesco

  ==============================================================================
*/

#ifndef ADSREDITOR_H_INCLUDED
#define ADSREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ADSRViewer.h"

//==============================================================================
/*
*/
class ADSREditor : public Component,
                   public Slider::Listener
{
public:
    ADSREditor();
    ~ADSREditor();

    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;

    struct ADSRValues
    {
      float AttackTime, AttackLevel, DecayTime, DecayLevel, SustainTime, ReleaseTime;
    };

    ADSRValues getADSRValues();

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSREditor)
  Slider* atkTimeKnob;
  Slider* atkLvlKnob;
  Slider* decTimeKnob;
  Slider* decLvlKnob;
  Slider* susTimeKnob;
  Slider* relTimeKnob;

  Label* atkTimeLabel;
  Label* atkLvlLabel;
  Label* decTimeLabel;
  Label* decLvlLabel;
  Label* susTimeLabel;
  Label* relTimeLabel;

  ADSRViewer* adsrViewer;
};


#endif  // ADSREDITOR_H_INCLUDED

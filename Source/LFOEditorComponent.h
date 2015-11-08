/*
  ==============================================================================

    LFOEditorComponent.h
    Created: 6 Nov 2015 2:59:58pm
    Author:  Francesco

  ==============================================================================
*/

#ifndef LFOEDITORCOMPONENT_H_INCLUDED
#define LFOEDITORCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MultiSliderComponent.h"
#include "ADSREditor.h"

//==============================================================================
/*
*/
class LFOEditorComponent : public Component
{
public:
  LFOEditorComponent();
  ~LFOEditorComponent();

  void paint(Graphics&) override;
  void resized() override;

  ADSREditor::ADSRValues getADSRValues();
  float* getSliderValues();

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOEditorComponent)
  MultiSliderComponent* sliders;
  ADSREditor* adsrEditor;
};


#endif  // LFOEDITORCOMPONENT_H_INCLUDED

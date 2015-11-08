/*
  ==============================================================================

    ADSRViewer.h
    Created: 6 Nov 2015 5:45:26pm
    Author:  Francesco

  ==============================================================================
*/

#ifndef ADSRVIEWER_H_INCLUDED
#define ADSRVIEWER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ADSRViewer : public Component
{
public:
  ADSRViewer();
  ~ADSRViewer();

  void paint(Graphics&) override;
  void resized() override;

  void setValues(float atkTime, float atkLvl, float decTime, float decLvl, float susTime, float relTime);

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRViewer)
    float atkTime, atkLvl, decTime, decLvl, susTime, relTime;
};


#endif  // ADSRVIEWER_H_INCLUDED

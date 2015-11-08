/*
  ==============================================================================

    ADSRViewer.cpp
    Created: 6 Nov 2015 5:45:26pm
    Author:  Francesco

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ADSRViewer.h"

//==============================================================================
ADSRViewer::ADSRViewer()
{ }

ADSRViewer::~ADSRViewer()
{ }

void ADSRViewer::setValues(float atkTime, float atkLvl, float decTime, float decLvl, float susTime, float relTime)
{
  this->atkTime = atkTime;
  this->atkLvl = atkLvl;
  this->decTime = decTime;
  this->decLvl = decLvl;
  this->susTime = susTime;
  this->relTime = relTime;

  repaint();
}

void ADSRViewer::paint(Graphics& g)
{
  float k = 1 / (atkTime + decTime + susTime + relTime);
  if (isinf(k))
  {
    k = 1;
  }
  int width = getWidth();
  int height = getHeight();
  float atkWidth = atkTime * k * width;
  float decWidth = decTime * k * width;
  float susWidth = susTime * k * width;
  float relWidth = relTime * k * width;

  Path p;
  p.startNewSubPath(0, height);
  p.lineTo(atkWidth, height - (height * atkLvl));
  p.lineTo(atkWidth + decWidth, height - (height * decLvl));
  p.lineTo(atkWidth + decWidth + susWidth, height - (height * decLvl));
  p.lineTo(width, height);

  g.setColour(Colours::aqua.withAlpha(0.5f));
  g.strokePath(p, PathStrokeType(4));
}

void ADSRViewer::resized()
{ }

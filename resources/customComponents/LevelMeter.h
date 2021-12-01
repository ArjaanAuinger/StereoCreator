/*
 ==============================================================================
 Author: Thomas Deppisch
 
 Copyright (c) 2020 - Austrian Audio GmbH
 www.austrian.audio
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/*
*/
class LevelMeter : public Component
{
public:

    //float levelMeterCoeff;
    Image arrayImageMetering = juce::ImageCache::getFromMemory(BinaryData::Metering_png, BinaryData::Metering_pngSize);

    LevelMeter()
    {
        colour = Colours::black;
    }
    ~LevelMeter()
    {
    }

    void paint (Graphics& g) override
    {
        
        auto bounds = getLocalBounds();
        float labelWidth = bounds.getWidth();
        float labelHeight = labelWidth;
        auto labelBounds = bounds.removeFromBottom(labelHeight);
        g.setColour(Colours::white);
        g.setFont(getLookAndFeel().getTypefaceForFont(Font(labelHeight)));
        g.setFont (labelHeight);
        g.drawText(labelText, labelBounds, Justification::centred);
        
      
        
        if (arrayImageMetering.isValid())
        {
            const int frames = 17;
            //const int frameId = (((minDb - normalizedMeterHeight) / ((minDb * -1.0) / frames))*-1)-1;
            //normalizedMeterHeight = 0.99;
            const int frameId = frames * normalizedMeterHeight;
            DBG(frameId);


            int imgWidth = arrayImageMetering.getWidth();
            int imgHeight = arrayImageMetering.getHeight() / frames;
            g.drawImage(arrayImageMetering, 0, 0, imgWidth, bounds.getHeight(), 0, frameId * imgHeight+2, imgWidth, imgHeight);
        }

        /*

        else
        {
            static const float textPpercent = 0.35f;
            juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);

            g.setColour(juce::Colours::cadetblue);

            g.drawFittedText(juce::String("No Image"), text_bounds.getSmallestIntegerContainer(), juce::Justification::horizontallyCentred | juce::Justification::centred, 1);
        }
        */



    }

    void resized() override
    {
    }
    
    void setLevel(float newLevel)
    {
        float levelDb = Decibels::gainToDecibels(newLevel, minDb);
        normalizedMeterHeight = (minDb - levelDb) / minDb;
        levelMeterCoeff = newLevel;
        repaint();
    }
    
    void setColour(Colour newColour)
    {
        colour = newColour;
    }
    
    void setLabelText(juce::String newText)
    {
        labelText = newText;
        repaint();
    }
    
private:
    float levelMeterCoeff = 0.0f;
    float normalizedMeterHeight = 0.0f;
    Colour colour;
    juce::String labelText = "";
    const float minDb = -60.0f;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LevelMeter)
};

#include "async_button.h"

#include <Arduino.h>
#include <functional>

AsyncButton::AsyncButton(int pin, std::function<void()> press, std::function<void()> longPress) {
    m_pin = pin;
    m_longPress = longPress;
    m_press = press;
}

void AsyncButton::begin() {
    pinMode(m_pin, INPUT_PULLUP);
    ButtonManager.addButton(this);
}

void ButtonManager_t::link(std::vector<AsyncButton*> buttons, std::function<void()> onPress, int maxDelta)
{
    ButtonLinkStruct link{buttons, onPress, maxDelta};
    buttonLinks[currentMaxLinkId] = link;
    for(auto button : buttons)
    {
        button->linkIds.push_back(currentMaxLinkId);
        button->linkIds=sortVectorOfIntsThatAreLinkIdsByTheNumberOfButtonsInTheLinks(button->linkIds);
    }
    
    currentMaxLinkId++;
}

void ButtonManager_t::addButton(AsyncButton* button)
{
    buttons[currentMaxButtonId] = button;
    currentMaxButtonId++;
}

void ButtonManager_t::check() 
{
    for(const auto &[id, btnPtr] :buttons)
    {
        bool pinIsHigh=digitalRead(btnPtr->m_pin)==HIGH?true:false;
        if (pinIsHigh == HIGH) 
        {
            btnPtr->pressed = false;
        } 
        else 
        {
            btnPtr->pressedForMillis = millis() - btnPtr->pressStartInMillies;
            if (btnPtr->pressed == false&&btnPtr->wasHighBefore) 
            {
                btnPtr->pressStartInMillies = millis();
                btnPtr->pressedForMillis=0;
                btnPtr->pressed = true;
            }
        }

        if ( btnPtr->pressedForMillis >= btnPtr->debounceInMillis) 
        {
            
            if (btnPtr->pressedForMillis < btnPtr->millisForLongPress&&btnPtr->pressed==false) 
            { 
                btnPtr->m_press();
            } 
            else if(btnPtr->pressed == true&&btnPtr->pressedForMillis >= btnPtr->millisForLongPress)
            {
                bool wasMultiplePress=true;
                for(auto linkId : btnPtr->linkIds)
                {
                    for(auto button: buttonLinks[linkId].buttonPtrs)
                    {   
                        if(button->millisForLongPress-button->pressedForMillis<=buttonLinks[linkId].maxDelta&&button->pressed==true)
                        {
                            wasMultiplePress=true;                        
                        }
                        else
                        {
                            wasMultiplePress=false;
                        }

                        if(wasMultiplePress==false) break;
                    }
                    if(wasMultiplePress==true) {
                    buttonLinks[linkId].onPress();
                    for(auto button: buttonLinks[linkId].buttonPtrs)
                    {
                        button->pressed=false;
                         button->pressedForMillis=0;
                    } 
                    break;}
                }
                if(wasMultiplePress==false)
                {
                    btnPtr->m_longPress();
                    btnPtr->pressed = false;
                }
            }
        }

        if (btnPtr->pressed == false) 
        {
            btnPtr->pressedForMillis = 0;
        }

        btnPtr->wasHighBefore=pinIsHigh;
    }

}

std::vector<unsigned int> ButtonManager_t::sortVectorOfIntsThatAreLinkIdsByTheNumberOfButtonsInTheLinks(std::vector<unsigned int> linkIds)
{
    std::sort(linkIds.begin(), linkIds.end(),[&](unsigned int linkId1,unsigned int linkId2){return( buttonLinks[linkId1].buttonPtrs.size()>buttonLinks[linkId2].buttonPtrs.size());});
    return linkIds;
}
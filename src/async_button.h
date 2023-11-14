#ifndef ASYNC_BUTTON_HPP
#define ASYNC_BUTTON_HPP

#include <functional>
#include<map>
#include <vector>


class ButtonManager_t;
class AsyncButton {
    public:
        AsyncButton(int pin, std::function<void()> press , std::function<void()> longPress);
        void begin();
        friend class ButtonManager_t;
    protected:
        std::vector<unsigned int> linkIds;
        int m_pin;
        std::function<void()> m_longPress, m_press;
        bool pressed = false,wasHighBefore=true;
        unsigned long millisForLongPress = 300, debounceInMillis = 3, pressedForMillis = 0, pressStartInMillies=0;
};

struct ButtonLinkStruct{
    ButtonLinkStruct() : maxDelta(0) {}
    ButtonLinkStruct(std::vector<AsyncButton*> buttonPtrs, std::function<void()> onPress,int maxDelta)
    {
        this->buttonPtrs = buttonPtrs;
        this->onPress = onPress;
        this->maxDelta = maxDelta;
    }
    std::vector<AsyncButton*> buttonPtrs;
    std::function<void()> onPress;
    int maxDelta;
};

class ButtonManager_t {
    public:
        void link(std::vector<AsyncButton*> buttons, std::function<void()> onPress, int maxDelta=40);
        void check();
        friend class Button;
    protected:
        void addButton(AsyncButton* button);
    	std::map<unsigned int, AsyncButton*> buttons;
        std::map<unsigned int, ButtonLinkStruct> buttonLinks;
        unsigned int currentMaxButtonId=0,currentMaxLinkId=0, asyncId;
        std::vector<unsigned int>  sortVectorOfIntsThatAreLinkIdsByTheNumberOfButtonsInTheLinks(std::vector<unsigned int> linkIds);
};

inline ButtonManager_t ButtonManager;

#endif
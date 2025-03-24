
#ifndef UISCENECONTAINER
#define UISCENECONTAINER


#include <unordered_map>
#include <string>
#include <vita2d.h>
#include <vector>
#include <memory>
#include <stack>
#include <any>
#include "TextArea.h"

class UISceneContainer{
    private:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<TextArea>> uiElements;
    std::string currFocusKey;
    std::stack<std::string> currFocusStack;

    protected:
    int tests;
    public:
    UISceneContainer(std::string name);
    void AddUIElement(std::shared_ptr<TextArea> newElement,bool elementPullsFocus);
    void RemoveUIElement(std::string name);
    
    void NavigateUI(UIDIRECTION dir);
    void ElementLoseFocus();
    void ElementGainFocus(std::string);
    void Input(SceCtrlData ctrl);
    void Render();
    TextArea* GetElement(std::string name);

};

#endif

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
    std::string GetName();
    UISceneContainer(std::string name);
    ~UISceneContainer();
    void AddUIElement(std::shared_ptr<TextArea> newElement,bool elementPullsFocus);
    void RemoveUIElement(std::string name);
    TextArea* GetElement(std::string name);
    void NavigateUI(UIDIRECTION dir);
    void ElementLoseFocus();
    void ElementGainFocus(std::string, bool isPopUp = false);
    void Input(SceCtrlData ctrl);
    void Render();

};

#endif
#include "UISceneContainer.h"

#include "DebugLogging.h"

///refernces
///uiElements[currFocusKey]->

UISceneContainer::UISceneContainer(std::string name): currFocusKey(""){

}
void UISceneContainer::AddUIElement(std::shared_ptr<TextArea> newElement,bool elementPullsFocus){
    logToFile(newElement->GetName().c_str());
    //check if element exists in this sceneContainer and warn dev that its failed!
    uiElements.emplace(newElement->GetName(), newElement);
    logToFile( ("potato1.1: size="+ std::to_string(uiElements.size())).c_str());
    if (elementPullsFocus == true){
        ElementGainFocus(newElement->GetName());
    }
    if (!currFocusKey.empty()) {
        logToFile(("currFocusKey set to: " + currFocusKey).c_str());
    } else {
        logToFile("currFocusKey is null. No focus element set.");
    }
}
void UISceneContainer::RemoveUIElement(std::string name){

}
void UISceneContainer::ElementLoseFocus(){
    if (!currFocusStack.empty()){
        logToFile(("potato2.3 currFocusKey set to: " + currFocusKey).c_str());
        uiElements[currFocusStack.top()]->LoseFocus();
        // logToFile("potato2.4");

        currFocusStack.pop();
        // logToFile("potato2.4.1");
        if (!currFocusStack.empty()){
            currFocusKey = currFocusStack.top();
        }
        // /logToFile("potato2.4.2");
    }
}
void UISceneContainer::ElementGainFocus(std::string focusElementName){
    // logToFile("potato2.5");
    if (!currFocusStack.empty()){
        uiElements[currFocusStack.top()]->LoseFocus();
    }
    currFocusKey = focusElementName;
    currFocusStack.push(currFocusKey);
    uiElements[currFocusKey]->TakeFocus();
    // logToFile("potato2.6");
    if (!currFocusKey.empty()) {
        logToFile(("currFocusKey set to: " + uiElements[currFocusKey]->GetName()).c_str());
    } else {
        logToFile("currFocusKey is null. No focus element set.");
    }
    // logToFile("potato2.7");
}
TextArea* UISceneContainer::GetElement(std::string elementName){
    TextArea* element = uiElements[elementName].get();
    return element;
}

void UISceneContainer::NavigateUI(UIDIRECTION dir){
    
    //logToFile(("potato2 currFocusKey set to: " + currFocusKey).c_str());
    //logToFile(("potato2.1: moving in dir:" + std::to_string(dir)).c_str());
    if (uiElements.find(uiElements[currFocusKey]->GetUIConnection(dir)) != uiElements.end()){
        //logToFile(("potato2.1 moving to: " + uiElements[currFocusKey]->GetUIConnection(dir)).c_str());
        std::weak_ptr<TextArea> weakFocus = uiElements.at(uiElements[currFocusKey]->GetUIConnection(dir));
        if (auto newFocus = weakFocus.lock()) {
            //logToFile(("potato2.1 currFocusKey set to: " + currFocusKey).c_str());

            if (newFocus->GetFocusable()){
                ElementLoseFocus();
                ElementGainFocus(newFocus->GetName());
                //logToFile(("potato2.2 currFocusKey set to: " + currFocusKey).c_str());
            }
        }
    } 
    else {
        logToFile("Error: The element no longer exists or was removed! or never existed in the first place maybe");
    }
}



void UISceneContainer::Input(SceCtrlData ctrl){

    uiElements[currFocusKey]->Input(ctrl);
    

}

void UISceneContainer::Render(){
    //logToFile("potato4.1");
    for (const auto& pair: uiElements){
        //logToFile("potato4.2");
        //logToFile(("Object type: " + typeid(pair.second).name()).c_str());
        //logToFile( ("potato4.2.1: second="+ pair.first).c_str());
        pair.second->Render();
        //logToFile("potato4.2.2");

    }
}

#ifndef TEXTAREA_H
#define TEXTAREA_H

#include <string>
#include <vita2d.h>
#include <vector>
#include <array>
#include "DebugLogging.h"
#include <psp2/ctrl.h>
#include "InputManager.h"
#include "ObjectOptions.h"

/*
    used to create basic text boxes.



*/
enum UIDIRECTION { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };
class UISceneContainer;

class TextArea{
    private:

    std::vector<std::string> textLines;

    protected:
    UISceneContainer* ownerScene;
    int x,y,width,height;
	std::string name;
    int maxVisibleLines;
    int scrollOffset;
	vita2d_pgf *font;
    int maxCharOnLine;
    bool autoScroll;
    int autoScrollDirection;
    bool selectable;
    bool focused;
    bool focusable;
    bool disabled;
    //0 = north
    std::array<std::string,4> connectedUINavigationNeighbors;

    
    public:
#pragma region GetSetters
    /// @brief
    /// Get and setters
    int GetMaxCharOnLine()const;
    int GetMaxVisibleLines()const;
    int GetWidth()const;
    int GetHeight()const;
    std::string GetName()const;
    bool GetFocusable()const;
    bool GetIsFocused()const;
    bool IsDisabled()const;

#pragma endregion
    


//constructor
    TextArea();
    TextArea(int x, int y, int w, int h, const std::string &name, const std::string & initialText);
    TextArea(int x, int y, int w, int h, const std::string &name ,const std::string &initialText,UISceneContainer* owner);
    
    //destuctor
    ~TextArea();

    void Initilization(bool aScroll,int autoScrollDir, bool intrctv,bool focusTake);

    void InitlizeUIConnection(UIDIRECTION dir, std::string uiName);
    std::string GetUIConnection(UIDIRECTION dir)const;
    virtual void LoseFocus();
    virtual void TakeFocus();

    void Scroll(int direction);
    std::vector<std::string> Text_split(const std::string &s, char delimiter);
    void AddText(const std::string &newline);
    
    virtual void Input(SceCtrlData ctrl);
    virtual void Render()const;


};
class SelectableTextItem;

class TextArea_SelectableItems : public TextArea{
    private:
    std::vector<SelectableTextItem> textItems;
    SelectableTextItem* selectedTextItem = nullptr;
    std::vector<PlayerChoice> playerChoices;
    int focusedIndex;    
    mutable int displayedLines;
    public:
    TextArea_SelectableItems(int x, int y, int w, int h, const std::string &name, const std::string & initialText);
    TextArea_SelectableItems(int x, int y, int w, int h, const std::string &name, const std::string & initialText,UISceneContainer* owner);
    void AddTextItem(const std::string &newline);
    void AddTextItemsGroup(std::vector<PlayerChoice> playerChoices);
    void ClearSelectableTextItems();
    void FocusedScroll(int direction);
    void AdjustScroll();
    void Render() const;
    void Input(SceCtrlData ctrl);
    bool Focused();
    void Interact();
    void LoseFocus();
    void TakeFocus();
};
class SelectableTextItem{
    private:
    std::vector<std::string> displayText;
    bool focused;
    bool selected;
    TextArea_SelectableItems* ownerTextArea;
    int numberOfLines = 0;
    public:
    SelectableTextItem(std::string textLine, TextArea_SelectableItems* ownerTextArea);
    bool Focused();
    bool Selected();
    virtual void LoseFocus();
    virtual void TakeFocus();
    int GetNumberOfLines() const;
    void AddTextItem(const std::string &newline);
    void Input(SceCtrlData ctrl);
    void Render(int x, int y, int yOffSet,int hight, vita2d_pgf *font) const;
};

#endif
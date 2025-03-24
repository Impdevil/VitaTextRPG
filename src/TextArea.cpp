
#include "TextArea.h"
#include <sstream>
#include "UISceneContainer.h"



            ////// a reminder of how to do debugging
            //logToFile( ("potato3.1: i="+ std::to_string(i)+" textItems Size()= "+ std::to_string(textItems.size())).c_str());

///////
/*

*/
////////
#pragma region TextArea
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	



TextArea::TextArea(){

}


TextArea::TextArea(int x, int y, int w, int h, const std::string &name ,const std::string &initialText)
    :x(x),y(y),width(w),height(h), name(name){
    disabled = false;
    scrollOffset = 0;
    textLines.clear();
    if (!initialText.empty()){
        textLines.push_back(initialText);
    }
    textLines.reserve(256);
    font = vita2d_load_default_pgf();
    //calulate the font hight, allows new fonts to be used later
    int fontHeight = vita2d_pgf_text_height(font,1.0f,"l");
    maxVisibleLines = static_cast<int>((height-20-fontHeight*2)/fontHeight);
    int charWidth = vita2d_pgf_text_width(font,1.0f,"G");
    maxCharOnLine = static_cast<int>((width*1.5)/charWidth);
}



TextArea::TextArea(int x, int y, int w, int h, const std::string &name ,const std::string &initialText,UISceneContainer* owner)
    :x(x),y(y),width(w),height(h), name(name),ownerScene(owner){
    disabled = false;
    scrollOffset = 0;
    textLines.clear();
    if (!initialText.empty()){
        textLines.push_back(initialText);
    }
    textLines.reserve(256);
    font = vita2d_load_default_pgf();
    //calulate the font hight, allows new fonts to be used later
    int fontHeight = vita2d_pgf_text_height(font,1.0f,"l");
    maxVisibleLines = static_cast<int>((height-20-fontHeight*2)/fontHeight);
    int charWidth = vita2d_pgf_text_width(font,1.0f,"G");
    maxCharOnLine = static_cast<int>((width*1.5)/charWidth);
}


void TextArea::Initilization(bool aScroll, int autoScrollDir,bool intrctv,bool focusTake){
    logToFile(("Initilization of: " + GetName()).c_str());
    autoScroll = aScroll;
    selectable = intrctv;
    focusable = focusTake;
    autoScrollDirection=autoScrollDir;
    return;
}
void TextArea::InitlizeUIConnection(UIDIRECTION dir, std::string uiName){
    if (!uiName.empty()){
        connectedUINavigationNeighbors[dir] = uiName;
    }
}
std::string TextArea::GetUIConnection(UIDIRECTION dir)const{
    if (!connectedUINavigationNeighbors[dir].empty())
    {
        return connectedUINavigationNeighbors[dir];
    }
    else return "";
}


void TextArea::LoseFocus(){
    focused = false;
}
void TextArea::TakeFocus(){
    focused= true;

}



int TextArea::GetMaxCharOnLine()const    {return maxCharOnLine;}
int TextArea::GetMaxVisibleLines()const  {return maxVisibleLines;}
int TextArea::GetWidth()const            {return width;}
int TextArea::GetHeight()const           {return height;}
std::string TextArea::GetName()const     {return name;}
bool TextArea::GetIsFocused()const                 {return focused;}
bool TextArea::GetFocusable()const                 {return focusable;}
bool TextArea::IsDisabled()const                 {return disabled;}




TextArea::~TextArea(){
    vita2d_free_pgf(font);
}

void TextArea::Render()const{
    //logToFile("potato4.4");
    if (!IsDisabled()){
        if (focused){
            vita2d_draw_rectangle(x-1,y-1,width+1,height+1,RGBA8(255,255,255,255));
        }
        vita2d_draw_rectangle(x,y,width,height,RGBA8(50,50,50,255));
        for (int i = 0; i < maxVisibleLines; ++i)
        {
            int lineIdx = scrollOffset+i;
            if (lineIdx >= (int)textLines.size()){
                if (textLines.size() > maxVisibleLines){
                    vita2d_pgf_draw_text(font,x+(width/2),y+height-15, RGBA8(255,255,255,255), 1.0f, "MORE...");
                }
                break;
            }
            vita2d_pgf_draw_text(font,x+10,y+20 +i*20, RGBA8(255,255,255,255), 1.0f, textLines[lineIdx].c_str());
        }
        //logToFile("potato4.4");
        }
}

void TextArea::Input(SceCtrlData ctrl){
    
    // logToFile("potato3.2");
    if (GetIsFocused() and !IsDisabled()){
        // logToFile("potato3.3");
        if(GetFocusable()){
            logToFile("potato3.4");
            InputManager::getInstance()->GetKeyState(SCE_CTRL_UP);
            if(ctrl.buttons & SCE_CTRL_TRIANGLE){
                Scroll(-1);
            }
            if(ctrl.buttons & SCE_CTRL_CIRCLE){
                Scroll(1);
            }      
            if(ctrl.buttons & SCE_CTRL_SQUARE){
                AddText("A mysterious shadow moved swiftly through the forest under the moonlight.");
                AddText("A wooden sign swings softly in the rain, reflecting on fire light from inside the taverns open door.");
            }  
            if (InputManager::getInstance()->GetKeyState(SCE_CTRL_UP)==BUTTONSTATE::JUSTPRESSED) {
                ownerScene->NavigateUI(NORTH);
            }
            if (InputManager::getInstance()->GetKeyState(SCE_CTRL_DOWN)==BUTTONSTATE::JUSTPRESSED) {
                ownerScene->NavigateUI(SOUTH);
            }
            if (InputManager::getInstance()->GetKeyState( SCE_CTRL_LEFT)==BUTTONSTATE::JUSTPRESSED) {
                ownerScene->NavigateUI(WEST);
            }
            if (InputManager::getInstance()->GetKeyState( SCE_CTRL_RIGHT)==BUTTONSTATE::JUSTPRESSED ) {
                ownerScene->NavigateUI(EAST);
            }
        }
        else{
            /// shift to next element
        }
    }
    // logToFile("potato3.5");
}
/// @brief 
//noninteractive add text
/// @param newline 
void TextArea::AddText(const std::string &newline){
    if (textLines.size() < textLines.capacity()){

        int autoScrollDistance =0;
        //logToFile(("textarea " + name + ":newText = " + newline).c_str());
        if (newline.length() > maxCharOnLine){
        
            std::vector<std::string> newLines = Text_split(newline,' ');
            std::string resultLines=""; 

            
            for(int j = 0; j < newLines.size();++j){
                if (static_cast<int>(newLines[j].length() + resultLines.length() + 1) < maxCharOnLine){
                    if (resultLines == ""){
                        resultLines = newLines[j];
                    }
                    else{
                        resultLines += " " + newLines[j];
                    }
                }
                else{
                    std::string currLine = resultLines;
                    if (!currLine.empty()){
                        textLines.push_back(currLine);
                        autoScrollDistance++;
                        resultLines="";
                        resultLines = newLines[j];
                    }
                    else{
                        resultLines="";
                        resultLines = newLines[j];
                    }
                }
                if (j ==  newLines.size()-1)
                {
                    std::string currLine = resultLines;
                    if (!currLine.empty()){
                        textLines.push_back(currLine);
                        autoScrollDistance++;
                    }
                }
            }
        }
        else{
            textLines.push_back(newline);
        }
        if (autoScroll){
            Scroll(autoScrollDirection*autoScrollDistance);
        }    
    }
    
}

void TextArea::Scroll(int direction){
    scrollOffset += direction;
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > (int) textLines.size()-maxVisibleLines){
        scrollOffset = std::max(0,(int)textLines.size() - maxVisibleLines);
    }
}

std::vector<std::string> TextArea::Text_split(const std::string &s, char delimiter){
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;
    while(getline(ss,item,delimiter)){
        result.push_back(item);
    }
    return result;
}


#pragma endregion





#pragma region TextArea_SelectableItems
///TextArea_SelectableItems
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
TextArea_SelectableItems::TextArea_SelectableItems(int x, int y, int w, int h, const std::string &name ,const std::string &initialText)
    : TextArea(x,y,w,h,name,initialText){
    textItems.clear();
    if (!initialText.empty()){
        SelectableTextItem newItem = SelectableTextItem(initialText,this);
        textItems.push_back(newItem);
    }
    textItems.reserve(256);

}


TextArea_SelectableItems::TextArea_SelectableItems(int x, int y, int w, int h, const std::string &name ,const std::string &initialText,UISceneContainer* owner)
: TextArea(x,y,w,h,name,initialText,owner){
    textItems.clear();
    if (!initialText.empty()){
        SelectableTextItem newItem = SelectableTextItem(initialText,this);
        textItems.push_back(newItem);
    }
    textItems.reserve(256);

}
void TextArea_SelectableItems::AddTextItem(const std::string &newline){
    SelectableTextItem newItem = SelectableTextItem(newline,this);
    textItems.push_back(newItem);
    if (textItems.size() == 0){
        textItems[0].TakeFocus();
    }
}


void TextArea_SelectableItems::LoseFocus(){
    focused = false;
    if (textItems.size() > 0){
        textItems[focusedIndex].LoseFocus();
    }
}
void TextArea_SelectableItems::TakeFocus(){
    focused= true;
    focusedIndex = 0;
    if (textItems.size() >0){
        textItems[focusedIndex].TakeFocus();
    }
}


void TextArea_SelectableItems::FocusedScroll(int direction){
    scrollOffset += direction;
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > (int) textItems.size()-maxVisibleLines){
        scrollOffset = std::max(0,(int)textItems.size() - maxVisibleLines);
    }
}
void TextArea_SelectableItems::Render() const{
    if (!IsDisabled()){
        if (focused){
            vita2d_draw_rectangle(x-1,y-1,width+2,height+1,RGBA8(255,255,255,255));
        }
        vita2d_draw_rectangle(x,y,width,height,RGBA8(50,50,50,255));
        int renderTextLineOffset = 0;
        int renderedLines = 0;
        displayedLines=0;
        for (int i = scrollOffset; i < textItems.size(); ++i)
        {
            int itemLines = textItems[i].GetNumberOfLines();
            if (renderedLines + itemLines > maxVisibleLines){
                vita2d_pgf_draw_text(font,x+(width/2),y+height-1, RGBA8(255,255,255,255), 1.0f, "more...");
                break;
            }
            textItems[i].Render(x,y,y+renderedLines*20 + renderTextLineOffset,height,font);
            renderedLines += itemLines;
            renderTextLineOffset += (itemLines-1)*20;
            displayedLines ++;
        }
    }
}

void TextArea_SelectableItems::Input(SceCtrlData ctrl){
    // logToFile("potato3.2 TextArea_SelectableItems");
    if (GetIsFocused() and !IsDisabled()){
        // logToFile("potato3.3 TextArea_SelectableItems");
        if(GetFocusable()){
            logToFile("potato3.4 TextArea_SelectableItems");

            if(ctrl.buttons & SCE_CTRL_TRIANGLE){
                FocusedScroll(-1);
            }
            if(ctrl.buttons & SCE_CTRL_CIRCLE){
                FocusedScroll(1);
            }      
            if(ctrl.buttons & SCE_CTRL_SQUARE){
                AddTextItem("A shadow moved swiftly through the forest.");
                AddTextItem("A wooden sign swings softly in the rain, reflecting on fire light from inside the taverns open door.");
            }
            if(ctrl.buttons & SCE_CTRL_SELECT){
                AddTextItem("A mysterious shadow moved swiftly through the forest under the moonlight.");
                AddTextItem("A wooden sign swings softly in the rain, reflecting on fire light from inside the taverns open door.");
                
            }
            if (InputManager::getInstance()->GetKeyState(SCE_CTRL_UP)==BUTTONSTATE::JUSTPRESSED) {
                if (focusedIndex <= 0)ownerScene->NavigateUI(NORTH);
                else{
                    if (textItems.size() >0){
                        if (focusedIndex > 0){
                            textItems[focusedIndex].LoseFocus();
                            focusedIndex--;
                            //FocusedScroll(-1);
                            textItems[focusedIndex].TakeFocus();
                        }
                    }
                }
            }
            if (InputManager::getInstance()->GetKeyState(SCE_CTRL_DOWN)==BUTTONSTATE::JUSTPRESSED) {
                if (focusedIndex > textItems.size())ownerScene->NavigateUI(SOUTH);
                else{ 
                    if (textItems.size() >0){
                        if (focusedIndex < textItems.size()-1){
                            textItems[focusedIndex].LoseFocus();
                            focusedIndex++;
                            //FocusedScroll(1);
                            textItems[focusedIndex].TakeFocus();
                        }
                    }
                }
            }
            if (InputManager::getInstance()->GetKeyState( SCE_CTRL_LEFT)==BUTTONSTATE::JUSTPRESSED) {
                ownerScene->NavigateUI(WEST);
            }
            if (InputManager::getInstance()->GetKeyState( SCE_CTRL_RIGHT)==BUTTONSTATE::JUSTPRESSED ) {
                ownerScene->NavigateUI(EAST);
            }
        }
        else{
            /// shift to next element
        }
    }
    // logToFile("potato3.5");
    AdjustScroll();
}



void TextArea_SelectableItems::AdjustScroll(){
    if (textItems.size() >0){
        if (focusedIndex < scrollOffset){
            scrollOffset = focusedIndex;
        }else if (focusedIndex >= scrollOffset + displayedLines-1){
            scrollOffset = focusedIndex-displayedLines-1+textItems[focusedIndex].GetNumberOfLines();
        }
    }
}
#pragma endregion

#pragma region SelectableTextItem
///SelectableTextItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool focused = false;
bool selected = false;


SelectableTextItem::SelectableTextItem(std::string textLine,TextArea_SelectableItems* ownerTextArea)
:ownerTextArea(ownerTextArea){
    AddTextItem(textLine);
    //displayText.reserve(4);
    LoseFocus();
}

bool SelectableTextItem::Focused(){
    return focused;
}
bool SelectableTextItem::Selected(){
    return selected;
}
void SelectableTextItem::LoseFocus(){
    focused = false;
}
void SelectableTextItem::TakeFocus(){
    focused = true;
}
/// @brief 
//noninteractive add text
/// @param newline 
void SelectableTextItem::AddTextItem(const std::string &newline){

    logToFile(("SelectableTextItem: newText = " + newline).c_str());
        if (newline.length() > ownerTextArea -> GetMaxCharOnLine()){

            std::vector<std::string> newLines = ownerTextArea -> Text_split(newline,' ');
            std::string resultLines=""; 
            
            
            for(int j = 0; j < newLines.size();++j){
                if (static_cast<int>(newLines[j].length() + resultLines.length() + 1) < ownerTextArea -> GetMaxCharOnLine()){
                    if (resultLines.empty()){
                        resultLines = "● " + newLines[j];
                    }
                    else{
                        resultLines += " " + newLines[j];
                    }
                }
                else{
                    std::string currLine = resultLines;
                    if (!currLine.empty()){
                        displayText.push_back(currLine);
                        resultLines.clear();
                        resultLines = newLines[j];
                    }
                    else if (!newLines[j].empty()){
                        resultLines.clear();
                        resultLines = newLines[j];
                    }
                }
                if (j ==  newLines.size()-1)
                {
                    std::string currLine = resultLines;
                    if (!currLine.empty()){
                        displayText.push_back(currLine);
                    }
                }
            }
        }
        else{
            displayText.push_back("● " +newline);
        }
}

void SelectableTextItem::Render(int x, int y, int yOffSet,int height, vita2d_pgf *font) const{
    for (int i = 0; i < (int)displayText.size(); i++){
        if (yOffSet+20+i*20 > y + height)break;
        if (yOffSet+20+i*20 > y + height - 20 and i != displayText.size())
        {

            vita2d_pgf_draw_text(font,x+10,yOffSet+20+i*20, RGBA8(255,255,255,255), 1.0f, "...");
            break;
        }
        vita2d_pgf_draw_text(font,x+10,yOffSet+20+i*20, RGBA8(255,255,255,255), 1.0f, displayText[i].c_str());

        
    }
    if (focused){
        //logToFile("potato4.3.4");
        vita2d_pgf_draw_text(font,x+ownerTextArea->GetWidth() ,yOffSet+20, RGBA8(255,255,255,255), 1.0f, "<");
        //logToFile("potato4.3.5");
    }
    //logToFile("potato4.3.6");
}

int SelectableTextItem::GetNumberOfLines()const
{
    return displayText.size();
}


#pragma endregion




#ifndef Input_Manager_H
#define Input_Manager_H

#include <unordered_map>
#include <string>
#include <psp2/ctrl.h>
#include <memory>

enum BUTTONSTATE{JUSTPRESSED,HELD,JUSTRELEASED,RELEASED};


struct ButtonState{
    BUTTONSTATE buttonState = BUTTONSTATE::RELEASED;
    float buttonHeldFor =0;
};

class InputManager{
    private:
    SceCtrlData ctrl;
    std::unordered_map<uint32_t,ButtonState> buttonStates;
    static InputManager* instance;


    
    public:
    ~InputManager();
    InputManager();
    static InputManager* getInstance();
    void ProcessInput();
    BUTTONSTATE GetKeyState(uint32_t ButtonID);
};



#endif
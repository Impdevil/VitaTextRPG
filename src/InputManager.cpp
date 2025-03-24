#include "InputManager.h"
#include "DebugLogging.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager(){
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
// Select button
    buttonStates [SCE_CTRL_SELECT] = ButtonState();
//: Start button
    buttonStates [SCE_CTRL_START] = ButtonState();
//: D-pad Up
    buttonStates [SCE_CTRL_UP] = ButtonState();
//: D-pad Right
    buttonStates [SCE_CTRL_RIGHT] = ButtonState();
//: D-pad Down
    buttonStates [SCE_CTRL_DOWN] = ButtonState();
//: D-pad Left
    buttonStates [SCE_CTRL_LEFT] = ButtonState();
//: Left Trigger (L)
    buttonStates [SCE_CTRL_LTRIGGER] = ButtonState();
//: Right Trigger (R)
    buttonStates [SCE_CTRL_RTRIGGER] = ButtonState();
//: Triangle button
    buttonStates [SCE_CTRL_TRIANGLE] = ButtonState();
//: Circle button
    buttonStates [SCE_CTRL_CIRCLE] = ButtonState();
//: Cross button
    buttonStates [SCE_CTRL_CROSS] = ButtonState(); 
//: Square button
    buttonStates [SCE_CTRL_SQUARE] = ButtonState();  
}
InputManager::~InputManager() {
    delete instance;
    instance = nullptr; 
}

InputManager* InputManager::getInstance() {
    if (!instance) {
        instance= new InputManager();
    }
    return instance;
}

void InputManager::ProcessInput(){
    sceCtrlPeekBufferPositive(0,&ctrl,1);
    for (auto& pair : buttonStates) {
        int32_t button =pair.first;
        //logToFile(("buttonpressed" + std::to_string(button)).c_str());
        ButtonState& state = pair.second;

		if (ctrl.buttons & button){
            if(state.buttonState == BUTTONSTATE::RELEASED)
            {
                state.buttonState = BUTTONSTATE::JUSTPRESSED;
            }
            else if (state.buttonState == BUTTONSTATE::JUSTPRESSED)
            {
                state.buttonState = BUTTONSTATE::HELD;
            }
		}
        else{
            state.buttonState = BUTTONSTATE::RELEASED;
        }
    }
}
BUTTONSTATE InputManager::GetKeyState(uint32_t buttonID){
    BUTTONSTATE currentState  = buttonStates.at(buttonID).buttonState;
    return currentState;
}




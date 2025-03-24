#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544
#define LOG_AREA_X 20
#define LOG_AREA_Y 20
#define LOG_AREA_WIDTH_ 920
#define LOG_AREA_HEIGHT 480


#include <memory>
#include "DebugLogging.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <psp2/display.h>
#include <psp2/ctrl.h>
#include <string>

#include <vita2d.h>
#include "TextArea.h"
#include "UISceneContainer.h"
#include "InputManager.h"
using namespace std;


UISceneContainer MainViewScene= UISceneContainer("MainViewScene");

void initialize(){

	InputManager::getInstance();
	logToFile("potato");
	vita2d_init();
	vita2d_set_clear_color(RGBA8(20,20,20,255));
}
void cleanup(){
	vita2d_fini();
	vita2d_free_pgf(vita2d_load_default_pgf());

}
void createGameScene(){
	std::shared_ptr<TextArea> logArea = std::make_shared<TextArea>(24,18,617,288,"MainTextBox","",&MainViewScene);
	logArea->Initilization(true,1,false,true);

	std::shared_ptr<TextArea_SelectableItems> playerChoiceBox = std::make_shared<TextArea_SelectableItems>(24,334,617,184,"PlayerChoiceBox","",&MainViewScene);
	playerChoiceBox->Initilization(false,1,true,true);
	



	std::shared_ptr<TextArea> healthBox = std::make_shared<TextArea>(669,18,268,150,"HealthBox","",&MainViewScene);
	healthBox->Initilization(true,1,false,true);


	logArea->InitlizeUIConnection(UIDIRECTION::EAST,healthBox->GetName());
	logArea->InitlizeUIConnection(UIDIRECTION::SOUTH,playerChoiceBox->GetName());

	healthBox->InitlizeUIConnection(UIDIRECTION::WEST,logArea->GetName());

	playerChoiceBox->InitlizeUIConnection(UIDIRECTION::NORTH,logArea->GetName());
	playerChoiceBox->InitlizeUIConnection(UIDIRECTION::EAST,healthBox->GetName());

	MainViewScene.AddUIElement(logArea,false);
	MainViewScene.AddUIElement(healthBox,false);
	MainViewScene.AddUIElement(playerChoiceBox,true);
}

int main(){
	initialize();
	createGameScene();
	

	SceCtrlData ctrl;
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	bool running = true;
	while(running){
		sceCtrlPeekBufferPositive(0,&ctrl,1);
		if (ctrl.buttons & SCE_CTRL_START){
			running = false;
		}
		// logToFile("potato3");
		MainViewScene.Input(ctrl);

		InputManager::getInstance()->ProcessInput();
        // logToFile("potato4");

		vita2d_start_drawing();
		vita2d_clear_screen();
		MainViewScene.Render();
		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	cleanup();
	return 0;
}





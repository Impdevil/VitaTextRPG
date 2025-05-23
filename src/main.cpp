#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544
#define LOG_AREA_X 20
#define LOG_AREA_Y 20
#define LOG_AREA_WIDTH_ 920
#define LOG_AREA_HEIGHT 480


#include <memory>
#include "Systems/DebugLogging.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <psp2/display.h>
#include <psp2/ctrl.h>
#include <string>

#include <vita2d.h>
#include "UISystems/TextArea.h"
#include "UISystems/UISceneContainer.h"
#include "UISystems/UIManager.h"
#include "UISystems/InputManager.h"
#include "DungeonElements/DungeonManager.h"
#include "DungeonElements/DungeonRoom.h"


UISceneContainer MainViewScene= UISceneContainer("MainViewScene");
DungeonManager basicDungeon = DungeonManager("basicDungeon");

void initialize(){
	UIManager::GetInstance();
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
	UIManager::GetInstance().AddSceneContainer(std::make_shared<UISceneContainer>(MainViewScene));
}

void makeBasicDungeon(){
	DungeonRoom* newRoom = new DungeonRoom(std::string("e1"),std::string("Entrance Room"),std::string("A bright and airy room lit by candles."),std::string("the candles look like they have been burning for a little while."));
	DungeonDoor* newDoor = new DungeonDoor("e1d1","wooden Door", "A wooden door hangs from its hinges.", "It looks like its about to fall from the hinges, and the slight smell of mildew");

	basicDungeon.AddStartingRoom(newRoom);
	newRoom->GenerateObjectOptions(static_cast<uint32_t>(ObjectOptions::Inspect));
	newDoor->SetObjectOption(static_cast<uint32_t>(ObjectOptions::PassThrough) | static_cast<uint32_t>(ObjectOptions::Inspect));
	newRoom->AddDungeonDoor(newDoor);

	DungeonRoom* newRoom1 = new DungeonRoom("e2","Bed Room","A bed sits in the corner of the room",std::string("The bed isnt made, some of the covers lay on the floor, either someone was in a rush or they're lazy."));

	basicDungeon.AddDungeonFeature(newDoor);
	basicDungeon.AddDungeonRoom(newRoom1);
	newRoom1->AddDungeonDoor(newDoor);


	newRoom->GetDungeonDoor("e1d1")->SetRoomConnection(newRoom,newRoom1);
	newRoom1->GenerateObjectOptions(static_cast<uint32_t>(ObjectOptions::Inspect));



	newRoom->SetDungeonManager(&basicDungeon);
	newRoom1->SetDungeonManager(&basicDungeon);
	newDoor->SetDungeonManager(&basicDungeon);	
	DungeonFeature* room2Feature = new DungeonFeature("e2f1","Bed covers","The bed covers seem like they are moving.",std::string("A goblin hides under the bed covers!"));
	room2Feature->SetDungeonManager(&basicDungeon);
	basicDungeon.AddDungeonFeature(room2Feature);
	dynamic_cast<DungeonObject*>(newRoom1)->AddDungeonFeature(room2Feature);
	room2Feature->SetDiscoverible(false);
	room2Feature->SetObjectOption(static_cast<uint32_t>(ObjectOptions::Inspect) | static_cast<uint32_t>(ObjectOptions::Open));


}

void makeDBDungeon(){
	if (!basicDungeon.LoadDungeonFromDatabase("ux0:downloads/TextAdventure.sqlite","The Test Dungeon"))
	{
		logToFile("Failed to load dungeon from database.");
		makeBasicDungeon();
	}
	else{
		logToFile("Dungeon loaded from database successfully.");
	}
	//basicDungeon.UpdateRoomUI();
}

int main(){
	initialize();
	createGameScene();
	//makeBasicDungeon();
	makeDBDungeon();

	SceCtrlData ctrl;
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	bool running = true;
	basicDungeon.UpdateRoomUI();
	while(running){
		sceCtrlPeekBufferPositive(0,&ctrl,1);
		if (ctrl.buttons & SCE_CTRL_START){
			running = false;
		}
		// logToFile("potato3");
		MainViewScene.Input(ctrl);

		InputManager::getInstance()->ProcessInput();
        //logToFile("main4");

		vita2d_start_drawing();
		vita2d_clear_screen();
		MainViewScene.Render();
		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	cleanup();
	return 0;
}





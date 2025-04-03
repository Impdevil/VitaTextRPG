#include "DungeonManager.h"


DungeonManager::DungeonManager(std::string id ):id(id),PlayerCurrentRoomID(""){

}
std::string DungeonManager::GetID(){    return id;     }

std::shared_ptr<UISceneContainer>  DungeonManager::GetUIContainerMainViewScene(){
    logToFile("GetUIContainer: uiContainer is being accessed.");
    return UIManager::GetInstance().GetSceneContainer("MainViewScene");
};
TextArea*  DungeonManager::GetUIContainerMainViewSceneMainTextBox(){
    logToFile("GetUIContainer: uiContainer is being accessed.");
    return UIManager::GetInstance().GetSceneContainer("MainViewScene")->GetElement("MainTextBox");
};
TextArea_SelectableItems*  DungeonManager::GetUIContainerMainViewScenePlayerChoiceBox(){
    logToFile("GetUIContainer: uiContainer is being accessed.");
    return dynamic_cast<TextArea_SelectableItems*>(UIManager::GetInstance().GetSceneContainer("MainViewScene")->GetElement("PlayerChoiceBox"));

};

void DungeonManager::AddDungeonRoom(DungeonRoom* newdungeonRoom){
    dungeonElements.emplace(newdungeonRoom->GetID(),newdungeonRoom);
}
void DungeonManager::AddStartingRoom(DungeonRoom* newdungeonRoom){
    PlayerCurrentRoomID = newdungeonRoom->GetID();
    dungeonElements.emplace(newdungeonRoom->GetID(),newdungeonRoom);
}
void DungeonManager::RemoveDungeonRoom(std::string id){
    dungeonElements.erase(id);
}
void DungeonManager::AddDungeonFeature(DungeonFeature* newFeature){
    dungeonElements.emplace(newFeature->GetID(),newFeature);
}



void DungeonManager::MoveToRoom(std::string roomID){
    PlayerCurrentRoomID = roomID;
    GetRoomDescription();
    GetRoomAvalibleOptions();
    UpdateRoomUI();
}

void DungeonManager::TraverseRoom(std::string doorID){
    GetUIContainerMainViewScenePlayerChoiceBox()->ClearSelectableTextItems();

    std::string newRoomId = dynamic_cast<DungeonDoor*>(dungeonElements.at(doorID))->GetOtherRoom(dynamic_cast<DungeonRoom*>(dungeonElements.at(PlayerCurrentRoomID)))->GetID();
    PlayerCurrentRoomID = newRoomId;
    logToFile(("Traversing to new room: " + PlayerCurrentRoomID).c_str());
    if (GetUIContainerMainViewScene()) {
        logToFile("Traversing UI container is valid.");
        UpdateRoomUI();
    } else {
        logToFile("Error: Traversing UI container is null.");
    }
}


std::string DungeonManager::GetRoomDescription(){
    logToFile(("Current Room ID: " + PlayerCurrentRoomID).c_str());
    logToFile(("Dungeon Elements Size: " + std::to_string(dungeonElements.size())).c_str());
    if (dungeonElements.find(PlayerCurrentRoomID) == dungeonElements.end()){
        logToFile("PlayerCurrentRoomID not found in dungeonElements.");
        return "Error: Room not found.";
    }
    logToFile(("Current Room Description: " + (dungeonElements.at(PlayerCurrentRoomID))->GetObjectDescription()).c_str());
    
    return dynamic_cast<DungeonRoom*>(dungeonElements.at(PlayerCurrentRoomID))->GetRoomEntryDescription();

}
std::vector<PlayerChoice> DungeonManager::GetRoomAvalibleOptions(){
   return dungeonElements.at(PlayerCurrentRoomID)->GetObjectOptions();
}




// void DungeonManager::AttachNewUI(std::shared_ptr<UISceneContainer> container){
//     uiContainer = container;
//     if (uiContainer) {
//         logToFile(("Attached UI Container: " + uiContainer->GetName()).c_str());
//     } else {
//         logToFile("Failed to attach UI Container: uiContainer is null.");
//     }
// }




void DungeonManager::UpdateRoomUI(){
    GetUIContainerMainViewScenePlayerChoiceBox()->ClearSelectableTextItems();
    std::string roomDescription = GetRoomDescription();
    logToFile(("Updating UI, current room is: " + PlayerCurrentRoomID).c_str());
    logToFile(("UIContainer is " + GetUIContainerMainViewSceneMainTextBox()->GetName()).c_str());
    logToFile(roomDescription.c_str());


    GetUIContainerMainViewSceneMainTextBox()->AddText(roomDescription);
    GetUIContainerMainViewScenePlayerChoiceBox()->AddTextItemsGroup(GetRoomAvalibleOptions());
}
void DungeonManager::UpdateRoomOptions(){
    GetUIContainerMainViewScenePlayerChoiceBox()->ClearSelectableTextItems();
    logToFile(("Updating UI, current room is: " + PlayerCurrentRoomID).c_str());
    logToFile(("UIContainer is " + GetUIContainerMainViewSceneMainTextBox()->GetName()).c_str());

    GetUIContainerMainViewScenePlayerChoiceBox()->AddTextItemsGroup(GetRoomAvalibleOptions());
}
void DungeonManager::SendDataToUI(const std::string& data) {
    try{
        logToFile(("Sending data to UI: " + data).c_str());
        logToFile("potatoDMSDTUI1");
        GetUIContainerMainViewSceneMainTextBox();
        logToFile("potatoDMSDTUI2");
        if (!GetUIContainerMainViewSceneMainTextBox()) {
            logToFile("Error: SendDataToUI UI container is null.");
            return;
        }
        logToFile("potatoDMSDTUI3");
        auto* mainTextBox = GetUIContainerMainViewSceneMainTextBox();
        if (mainTextBox) {
            logToFile("MainTextBox found in UI container.");
            mainTextBox->AddText(data);

        } else {
                logToFile("Error: MainTextBox not found in UI container.");
        }

    }
    catch (const std::exception& e) {
        logToFile(("Exception: " + std::string(e.what())).c_str());
    }
    catch (...) {
        logToFile("Unknown exception occurred.");
    }   
}
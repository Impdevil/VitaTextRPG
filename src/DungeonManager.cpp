#include "DungeonManager.h"


DungeonManager::DungeonManager()
:uiContainer(nullptr),PlayerCurrentRoomID(""){

}
void DungeonManager::AddDungeonRoom(DungeonRoom* newdungeonRoom){
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
    std::string newRoomId = dynamic_cast<DungeonDoor*>(dungeonElements.at(doorID))->GetOtherRoom(dynamic_cast<DungeonRoom*>(dungeonElements.at(PlayerCurrentRoomID)))->GetID();
    PlayerCurrentRoomID = newRoomId;
    GetRoomDescription();
    GetRoomAvalibleOptions();
    UpdateRoomUI();
}


std::string DungeonManager::GetRoomDescription(){
    //return dungeonElements.at(PlayerCurrentRoomID)->GetObjectOptions();
    return std::string("potato");
}
void DungeonManager::GetRoomAvalibleOptions(){
    dungeonElements.at(PlayerCurrentRoomID)->GetObjectOptions();
}
void DungeonManager::AttachNewUI(UISceneContainer* container){
    uiContainer = container;
}
void DungeonManager::UpdateRoomUI(){
    uiContainer->GetElement("MainTextBox")->AddText(GetRoomDescription());
    //GetRoomAvalibleOptions(PlayerCurrentRoomID);
}
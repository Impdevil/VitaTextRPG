
#include "DungeonRoom.h"


#pragma region Dungeon Room

// std::string id;

// std::string roomDescription;
// unordered_map <int,DungeonDoor> doorways;

DungeonRoom::DungeonRoom(std::string roomID, std::string name, const std::string& Description, std::string inspectionString)
:DungeonObject(roomID,name, Description,inspectionString){

}


bool DungeonRoom::hasValidConnections()const{
    if (doorways.size() > 0){
        return true;
    }
    else return false;
}
DungeonDoor* DungeonRoom::GetDungeonDoor(std::string id){
    return doorways.at(id);
}
void DungeonRoom::AddDungeonDoor(DungeonDoor* newdoorway){

    doorways.emplace(newdoorway->GetID(),newdoorway);
}
DungeonFeature* DungeonRoom::GetDungeonFeature(std::string id){
    return doorways.at(id);
}
void DungeonRoom::AddDungeonFeature(DungeonFeature* newFeature){
    dungeonFeature.emplace(newFeature->GetID(),newFeature);
}

std::string DungeonRoom::GetRoomEntryDescription(){
    std::string roomEntryDescription="";
    ///string setup Room description, discoverable features, doors. 
    roomEntryDescription = GetObjectDescription();
    for(auto& pair:dungeonFeature){
        if (pair.second->GetDiscoverible()){
            roomEntryDescription += pair.second->GetObjectDescription();
        }
    }
    for(auto& pair:doorways){
        if (pair.second->GetDiscoverible()){
            roomEntryDescription += pair.second->GetObjectDescription();
        }
    }
    return roomEntryDescription;
}





std::vector<PlayerChoice> DungeonRoom::GetObjectOptions(){
    std::vector<PlayerChoice> newPlayerChoices =DungeonObject::GetObjectOptions();
    
    std::vector<PlayerChoice> dungeonFeaturesOptions;
    for(auto& pair:dungeonFeature){
        if (pair.second->GetDiscoverible()){
            dungeonFeaturesOptions = pair.second->GetObjectOptions();
            newPlayerChoices.insert(newPlayerChoices.end(), dungeonFeaturesOptions.begin(),dungeonFeaturesOptions.end());
        }
    }

    std::vector<PlayerChoice> dungeonDoorwaysOptions;
    for(auto& pair:doorways){
        if (pair.second->GetDiscoverible()){
            dungeonDoorwaysOptions = pair.second->GetObjectOptions();
            newPlayerChoices.insert(newPlayerChoices.end(), dungeonDoorwaysOptions.begin(),dungeonDoorwaysOptions.end());
        }
    }
    return newPlayerChoices;
}

std::string DungeonRoom::interactWithObject(ObjectOptions optionChoice){
    std::string returnString;
    switch (optionChoice) {
        case ObjectOptions::Inspect:
            returnString = GetInspectionDescription();
            break;
        default:
            //logToFile("This action is not possible for the door!");
            break;
    }
    return returnString;
}
#pragma endregion






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Dungeon Door

// int id;
// std::string DoorDescription;
// DungeonRoom* roomConnection;
// bool lockState


DungeonDoor::DungeonDoor(std::string doorID, std::string name, const std::string& Description,std::string inspectionString)
: DungeonFeature(doorID ,name,Description, inspectionString), connectionType(DoorConnectionType::NormalDoor),
roomConnectionA(nullptr), roomConnectionB(nullptr), lockState(DoorState::Closed){}


void DungeonDoor::SetRoomConnection(DungeonRoom* connectedRoomA,DungeonRoom* connectedRoomB){
    roomConnectionA = connectedRoomA;
    roomConnectionB = connectedRoomB;
}
DungeonRoom* DungeonDoor::GetConnectedRoomA(){
    return roomConnectionA;
}
DungeonRoom* DungeonDoor::GetConnectedRoomB(){
    return roomConnectionB;
}
DungeonRoom* DungeonDoor::GetOtherRoom(DungeonRoom* fromRoom){
    if (GetConnectedRoomA() == fromRoom) return GetConnectedRoomB();
    if (GetConnectedRoomB() == fromRoom) return GetConnectedRoomA();
    return nullptr;

}
bool DungeonDoor::isLocked(){
    if (lockState == DoorState::Locked)
        return true;
    else return false;
}
void DungeonDoor::SetDoorState(DoorState state){
    lockState = state;
}
std::string DungeonDoor::interactWithObject(ObjectOptions optionChoice){
    std::string returnString;
    switch (optionChoice) {
        case ObjectOptions::Inspect:
            GetInspectionDescription();
            break;
        default:
            //logToFile("This action is not possible for the door!");
            break;
    }
    return returnString;
}
std::vector<PlayerChoice> DungeonDoor::GenerateObjectOptions(uint32_t options){
    std::vector<PlayerChoice> newPlayerChoices =DungeonObject::GenerateObjectOptions(options);

    if(DungeonObject::isOptionSet(objectOptions, ObjectOptions::PassThrough))
    {
        PlayerChoice passThrough{
            ObjectOptions::PassThrough,
            "Go through " + GetName(),
            true,
            0,
            this
        };
        newPlayerChoices.push_back(passThrough);
    }
    playerChoices = newPlayerChoices;
    return newPlayerChoices;
}

#pragma endregion

#include "DungeonRoom.h"


#pragma region Dungeon Room

// std::string id;

// std::string roomDescription;
// unordered_map <int,DungeonDoor> doorways;

DungeonRoom::DungeonRoom(std::string roomID, std::string name, const std::string& Description, std::string inspectionString)
:DungeonObject(id(roomID),name(name), Description(Description),inspectionString(inspectionString){

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
    newdoorway
    doorways.emplace(newdoorway->GetID(),newdoorway)

}
DungeonFeature* DungeonRoom::GetDungeonFeature(int id){
    return doorways.at(id);
}
void DungeonRoom::AddDungeonFeature(DungeonFeature* newFeature){
    dungeonFeature.emplace(newFeature->GetID(),newFeature)
}

std::string DungeonRoom::GetRoomEntryDescription(){
    std::string roomEntryDescription="";
    ///string setup Room description, discoverable features, doors. 
    roomEntryDescription = GetObjectDescription();
    for(auto&:dungeonFeature){
        if (pair.second.GetDiscoverible()){
            roomEntryDescription += pair.second.GetObjectDescription();
        }
    }
    for(auto&:doorways){
        if (pair.second.GetDiscoverible()){
            roomEntryDescription += pair.second.GetObjectDescription();
        }
    }
}

std::string DungeonRoom::GetRoomInspectDescription(){
    //room inspection description, undiscoverable features description.
    roomEntryDescription = GetInspectionDescription();

}



virtual std::Vector<ObjectOptions::PlayerChoice> DungeonRoom::GetObjectOptions(){
    std::vector<ObjectOptions::PlayerChoice> newPlayerChoices =DungeonObject::GetObjectOptions();
    
    std::vector<ObjectOptions::PlayerChoice> dungeonFeaturesOptions;
    for(auto&:dungeonFeature){
        if (pair.second.GetDiscoverible()){
            dungeonFeaturesOptions = second.GetObjectOptions();
            newPlayerChoices.insert(newPlayerChoices.end(), dungeonFeaturesOptions.start(),dungeonFeaturesOptions.end());
        }
    }
    std::vector<ObjectOptions::PlayerChoice> dungeonDoorwaysOptions;

    for(auto&:doorways){
        if (pair.second.GetDiscoverible()){
            dungeonDoorwaysOptions = second.GetObjectOptions();
            newPlayerChoices.insert(newPlayerChoices.end(), dungeonDoorwaysOptions.start(),dungeonDoorwaysOptions.end());
        }
    }
    return newPlayerChoices
}

std::string DungeonRoom::interactWithObject(ObjectOptions::ObjectOptions optionChoice){
    switch (option) {
        case ObjectOptions::Inspect:
                GetInspectionDescription();
            break;
        default:
            logToFile("This action is not possible for the door!");
            break;
    }
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
:DungeonFeature (id(doorID) ,name(name),Description(Description), inspectionString(inspectionString)), connectionType(DoorConnectionType::NormalDoor),
roomConnectionA(nullptr), roomConnectionB(nullptr), lockState(DoorState::Closed){}
{
    
}

void SetRoomConnection(DungeonRoom* connectedRoomA,DungeonRoom* connectedRoomB){
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
    if (GetConnectedRoomA == fromRoom) return GetConnectedRoomB();
    if (GetConnectedRoomB == fromRoom) return GetConnectedRoomA();
    return nullptr;

}
bool DungeonDoor::isLocked(){
    if (lockState == DoorState::Locked)
        return true;
}
void DungeonDoor::SetdoorState(DoorState state){
    lockState = state
}
std::string DungeonDoor::interactWithObject(ObjectOptions::ObjectOptions optionChoice){
    switch (option) {
        case ObjectOptions::Inspect:
                GetInspectionDescription();
            break;
        default:
            logToFile("This action is not possible for the door!");
            break;
    }
}
virtual std::Vector<ObjectOptions::PlayerChoice> DungeonDoor::GenerateObjectOptions(ObjectOptions:ObjectOptions options){
    std::vector<ObjectOptions::PlayerChoice> newPlayerChoices =DungeonObject::GenerateObjectOptions();
    objectOptions = options;
    if(isOptionSet(objectOptions, ObjectOptions::PassThrough))
    {
        ObjectOptions::PlayerChoice PassThrough{
            ObjectOptions::PassThrough,
            "Go through " + GetName(),
            true,
            0,
            this
        };
        newPlayerChoices.push_back(PassThrough);

    }
    return newPlayerChoices;
}

#pragma endregion
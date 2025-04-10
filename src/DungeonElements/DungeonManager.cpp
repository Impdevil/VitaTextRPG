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



bool DungeonManager::LoadDungeonFromDatabase(std::string databasePath, std::string dungeonName){
    DatabaseConnecter db(databasePath);
    std::string query = "SELECT * FROM Dungeon WHERE dungeonName = '" + dungeonName + "';";
    logToFile(("Querying database for dungeon name." + dungeonName).c_str());
    auto results = db.FetchQueryResults(query);
    //logToFile("Fetching results from database.");
    if (results.empty()) {
        logToFile("No results found for the given dungeon name.");
        return false;
    }
    std::string dungeonID = results[0].at("dungeonDBID");
    //logToFile(("Dungeon ID: " + dungeonID).c_str());

    query = "SELECT * FROM DungeonRooms WHERE dungeonDBID = '" + dungeonID + "';";
    results = db.FetchQueryResults(query);

    if (results.empty()) {
        logToFile("No room results found for the given dungeon ID.");
        return false;
    }

    for (const auto& row : results) {
        std::string roomID =  ("r"+ row.at("roomIDDB")).c_str();
        std::string roomName = row.at("roomName");
        std::string roomDescription = row.at("descriptionText");
        std::string inspectionString = row.at("inspectionText");

        DungeonRoom* newRoom = new DungeonRoom(roomID, roomName, roomDescription, inspectionString);
        newRoom->GenerateObjectOptions(static_cast<uint32_t>(ObjectOptions::Inspect));
        newRoom->SetDungeonManager(this);
        if (row.at("entranceRoom") == "1") {
            logToFile(("Starting room found: " + roomID).c_str());
            AddStartingRoom(newRoom);
        } else {
            logToFile(("Adding room: " + roomID).c_str());
            AddDungeonRoom(newRoom);
        }

    }


    query = "SELECT * FROM DoorTable WHERE dungeonDBID = '" + dungeonID + "';";
    results = db.FetchQueryResults(query);
    if (results.empty()) {
        logToFile("No  door results found for the given dungeon ID.");
        return false;
    }   
    //logToFile("Door results found for the given dungeon ID.");
    for (const auto& row : results) {
        //logToFile(("New door to add: " + row.at("doorIDDB")).c_str());
        std::string doorID = ("d"+row.at("doorIDDB")).c_str();
        std::string doorName = row.at("doorName");

        std::string doorDescription = row.at("descriptionText");
        std::string doorinspectionString = row.at("inspectionText");

        bool discoverible = row.at("hidden") == "0" ? true : false;

        DungeonRoom* connectA = GetDungeonRoom(("r"+row.at("dungeonRoomDBID_from")).c_str());

        DungeonRoom* connectB = GetDungeonRoom(("r"+row.at("dungeonRoomDBID_to")).c_str());

        DungeonDoor* newDoor = new DungeonDoor(doorID,doorName ,doorDescription, doorinspectionString);

        newDoor->SetDungeonManager(this);
        newDoor->SetDiscoverible(discoverible);
        newDoor->GenerateObjectOptions(static_cast<uint32_t>(ObjectOptions::PassThrough) | static_cast<uint32_t>(ObjectOptions::Inspect));

        newDoor->SetRoomConnection(connectA, connectB); 
        connectA->AddDungeonDoor(newDoor);
        connectB->AddDungeonDoor(newDoor);
        AddDungeonFeature(newDoor);
        //logToFile(("Adding door: " + doorID).c_str());

    }
    //logToFile("Door results added to dungeon.");
    query = "SELECT * FROM Feature WHERE dungeonDBID = '" + dungeonID + "';";
    results = db.FetchQueryResults(query);
    if (results.empty()) {
        logToFile("No feature results found for the given dungeon ID. dungeon may have no features");
        return true;
    }
    // else{
    //     logToFile("Feature results found for the given dungeon ID.");
    // }
    for (const auto& row : results) {
        std::string featureID = ("f" + row.at("featureIDDB")).c_str();
        std::string featureName = row.at("featureName");
        std::string featureDescription = row.at("descriptionText");
        //logToFile(("New feature to add: " + featureID).c_str());

        std::string inspectionString = row.at("inspectionText");
        bool discoverible = row.at("hidden") == "0" ? true : false;
        std::string parentObject= row.at("featureParentID");
        //logToFile(("New feature to add: " + featureID).c_str());
        DungeonFeature* newFeature = new DungeonFeature(featureID, featureName, featureDescription, inspectionString);
        newFeature->SetDungeonManager(this);
        newFeature->SetDiscoverible(false);
        newFeature->GenerateObjectOptions(static_cast<uint32_t>(ObjectOptions::Inspect) | static_cast<uint32_t>(ObjectOptions::Open));
        //logToFile(("New feature to add: " + featureID).c_str());

       
        GetDungeonObject(parentObject)->AddDungeonFeature(newFeature);
        //logToFile(("New feature to add: " + featureID).c_str());

        AddDungeonFeature(newFeature);
    }
    return true;
}


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


DungeonRoom* DungeonManager::GetDungeonRoom( std::string roomID){
    auto it = dungeonElements.find(roomID);
    if (it != dungeonElements.end()) {
        logToFile("Found room in dungeonElements.");
        logToFile(it->second->GetID().c_str());
        return dynamic_cast<DungeonRoom*>(it->second);
    }
    logToFile("No room found in dungeonElements.");
    return nullptr;
}
DungeonDoor* DungeonManager::GetDungeonDoor( std::string doorID){
    auto it = dungeonElements.find(doorID);
    if (it != dungeonElements.end()) {
        logToFile("Found door in dungeonElements.");
        logToFile(it->second->GetID().c_str());
        return dynamic_cast<DungeonDoor*>(it->second);
    }
    logToFile("No door found in dungeonElements.");
    return nullptr;
}
DungeonFeature* DungeonManager::GetDungeonFeature( std::string featureID){
    auto it = dungeonElements.find(featureID);
    if (it != dungeonElements.end()) {
        logToFile("Found Feature in dungeonElements.");
        logToFile(it->second->GetID().c_str());
        return dynamic_cast<DungeonFeature*>(it->second);
    }
    logToFile("No Feature found in dungeonElements.");
    return nullptr;
}

DungeonObject* DungeonManager::GetDungeonObject(std::string objectID){
    auto it = dungeonElements.find(objectID);
    if (it != dungeonElements.end()) {
        logToFile("Found object in dungeonElements.");
        logToFile(it->second->GetID().c_str());
        return it->second;
    }
    logToFile("No object found in dungeonElements.");
    return nullptr;
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
#ifndef DUNGEONMANAGER_H
#define DUNGEONMANAGER_H

#include <vector>
#include <unordered_map>
#include "VirDungeonObject.h"
#include "DungeonRoom.h"
#include "../UISystems/UISceneContainer.h"
#include "../Systems/DebugLogging.h"
#include "../UISystems/UIManager.h"
#include <memory>
#include <cassert>
#include "../Systems/DatabaseConnecter.h"



class DungeonManager{
    private:
    std::unordered_map<std::string, DungeonObject*> dungeonElements;
    std::string PlayerCurrentRoomID;
    std::string id;
    public:
    DungeonManager(std::string id);
    std::string GetID();


    void AddDungeonRoom(DungeonRoom* newdungeonRoom);
    void AddStartingRoom(DungeonRoom* newdungeonRoom);
    void RemoveDungeonRoom(std::string id);

    DungeonRoom* GetDungeonRoom( std::string roomID);
    DungeonDoor* GetDungeonDoor( std::string doorID);
    DungeonFeature* GetDungeonFeature(  std::string featureID);
    DungeonObject* GetDungeonObject(std::string objectID);


    void AddDungeonFeature(DungeonFeature* newFeature);
    
    bool LoadDungeonFromDatabase(std::string databasePath, std::string dungeonID);
    void MoveToRoom(std::string roomID);
    void TraverseRoom(std::string doorID);


    std::string GetRoomDescription();
    std::vector<PlayerChoice> GetRoomAvalibleOptions();

    void AttachNewUI(std::shared_ptr<UISceneContainer>container);
    std::shared_ptr<UISceneContainer> GetUIContainerMainViewScene();
    TextArea* GetUIContainerMainViewSceneMainTextBox();
    TextArea_SelectableItems* GetUIContainerMainViewScenePlayerChoiceBox();
    void UpdateRoomUI();
    void UpdateRoomOptions();

    

    void SendDataToUI(const std::string& data);
};


#endif
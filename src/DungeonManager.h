#ifndef DUNGEONMANAGER_H
#define DUNGEONMANAGER_H

#include <vector>
#include <unordered_map>
#include "VirDungeonObject.h"
#include "DungeonRoom.h"
#include "UISceneContainer.h"
#include "DebugLogging.h"
#include <memory>
#include <cassert>
class DungeonManager{
    private:
    std::unordered_map<std::string, DungeonObject*> dungeonElements;
    std::string PlayerCurrentRoomID;
    std::shared_ptr<UISceneContainer> uiContainer;
    std::string id;
    public:
    DungeonManager(std::string id);
    std::string GetID();
    void AddDungeonRoom(DungeonRoom* newdungeonRoom);
    void AddStartingRoom(DungeonRoom* newdungeonRoom);
    void RemoveDungeonRoom(std::string id);

    void AddDungeonFeature(DungeonFeature* newFeature);

    void MoveToRoom(std::string roomID);
    void TraverseRoom(std::string doorID);


    std::string GetRoomDescription();
    std::vector<PlayerChoice> GetRoomAvalibleOptions();

    void AttachNewUI(std::shared_ptr<UISceneContainer>container);
    std::shared_ptr<UISceneContainer> GetUIContainer();
    void UpdateRoomUI();
    void SendDataToUI(const std::string& data);

};


#endif
#ifndef DUNGEONMANAGER_H
#define DUNGEONMANAGER_H

#include <vector>
#include <unordered_map>
#include "VirDungeonObject.h"
#include "DungeonRoom.h"
#include "UISceneContainer.h"
#include "DebugLogging.h"

class DungeonManager{
    private:
    std::unordered_map<std::string, DungeonObject> dungeonElements;
    std::string PlayerCurrentRoomID;
    UISceneContainer* uiContainer;

    public:
    DungeonManager();
    void AddDungeonRoom(DungeonRoom* newdungeonRoom);
    void RemoveDungeonRoom(std::string id);

    void AddDungeonFeature(DungeonFeature* newFeature);

    void MoveToRoom(std::string roomID);
    void TraverseRoom(std::string doorID);


    std::string GetRoomDescription();
    void GetRoomAvalibleOptions();

    void AttachNewUI(UISceneContainer* container);
    void UpdateRoomUI();


};


#endif
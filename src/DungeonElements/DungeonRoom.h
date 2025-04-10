#ifndef DUNGEONROOM_H
#define DUNGEONROOM_H



#include <string>
//#include <vector>
#include <unordered_map>
#include "../Systems/DebugLogging.h"
#include "VirDungeonObject.h"

class DungeonDoor;

class DungeonRoom:public DungeonObject{
 
    protected:
    std::unordered_map <std::string,DungeonDoor*> doorways;
    public:
    DungeonRoom(std::string doorID,std::string name, const std::string& Description,std::string inspectionString);

    bool hasValidConnections()const;
    DungeonDoor* GetDungeonDoor(std::string);
    void AddDungeonDoor(DungeonDoor* newdoorway);

    DungeonFeature* GetDungeonFeature(std::string);
    void AddDungeonFeature(DungeonFeature* newFeature);

    std::vector<PlayerChoice> GenerateObjectOptions(uint32_t options);
    std::vector<PlayerChoice> GetObjectOptions();

    std::string GetRoomEntryDescription();
    virtual std::string interactWithObject(ObjectOptions optionChoice);
};


enum DoorConnectionType{
    NormalDoor,     // a normal door way that can be opened and closed, locked or similar
    Passway,        /// a cave system you would pass through an area, or a roadway where you go to a new part of a town could still be trapped
    Secret,         // a door that is hidden to begin with
    portal          // something that takes you to a new area, could be one way
};
enum DoorState{
    Open,
    Closed,
    Locked,
};

class DungeonDoor:public DungeonFeature{
    private:
    protected:
    DoorConnectionType connectionType;
    DungeonRoom* roomConnectionA;
    DungeonRoom* roomConnectionB;
    std::string DoorUseString;
    DoorState lockState;
    public:
    DungeonDoor(std::string doorID, std::string name, const std::string& Description,std::string inspectionString);
    void SetRoomConnection(DungeonRoom* connectedRoomA,DungeonRoom* connectedRoomB);
    DungeonRoom* GetConnectedRoomA();
    DungeonRoom* GetConnectedRoomB();
    DungeonRoom* GetOtherRoom(DungeonRoom* fromRoom);
    bool isLocked();
    void SetDoorState(DoorState state);
    std::vector<PlayerChoice> GenerateObjectOptions(uint32_t options);
    virtual std::string interactWithObject(ObjectOptions optionChoice);
};


#endif
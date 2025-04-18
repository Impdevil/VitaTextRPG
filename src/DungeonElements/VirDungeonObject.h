#ifndef DUNGEONOBJECT_H
#define DUNGEONOBJECT_H
#include "ObjectOptions.h"
#include <string>
#include <unordered_map>
//#include "../ItemSystems/PlayerItem.h"

#include <vector>

struct PlayerChoice;
class DungeonManager;
class DungeonFeature;
class DungeonObject{
    protected:

    std::string id;
    std::string name;
    DungeonManager* dungeonManager;
    uint32_t objectOptions;// bitmask for all options attached to this object
    std::string description;// base description of the object upon reaching it(rooms description followed by door ways)
    std::string inspectionString;// describes room in more detail, hinting at features and revealing more options 
    std::vector<PlayerChoice> playerChoices;
    std::unordered_map<std::string,DungeonFeature*> dungeonFeature;
    //std::unordered_map<std::string,PlayerItem*> attachedItems;
    public:
    DungeonObject(std::string id, std::string name, std::string Description, std::string inspectionString);
    virtual ~DungeonObject();

    DungeonFeature* GetDungeonFeature(std::string);
    void AddDungeonFeature(DungeonFeature* newFeature);

    //std::unordered_map<std::string,PlayerItem*> GetAttachedItems(){return attachedItems;};
    //void AddAttachedItem(PlayerItem* newItem){attachedItems.emplace(newItem->getId(),newItem);};
    //void RemoveAttachedItem(std::string id){attachedItems.erase(id);};
    
    
    std::string GetID();
    std::string GetName();
    virtual std::string GetObjectDescription();
    virtual std::string GetInspectionDescription();
    void SetObjectOption(int32_t options);
    bool isOptionSet(uint32_t bitmask, ObjectOptions option);
    void SetDungeonManager(DungeonManager* manager);
    DungeonManager* GetDungeonManager();

    virtual std::vector<PlayerChoice> GenerateObjectOptions(uint32_t options);
    virtual std::vector<PlayerChoice> GetObjectOptions();

    virtual std::string interactWithObject(ObjectOptions optionChoice);
    
};



class DungeonFeature: public DungeonObject{
    protected:
    bool discoverible = false;//means that the feature is shown upon entering the room that its held within if the room is inspected
    bool interactive = true;
    public:
    DungeonFeature(std::string id,std::string name, const std::string& Description,std::string inspectionString);
    bool GetDiscoverible();
    void SetDiscoverible(bool discoverible);

    bool CheckFeatureDiscoverible();

};
#endif
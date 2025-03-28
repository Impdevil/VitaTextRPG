#ifndef DUNGEONOBJECT_H
#define DUNGEONOBJECT_H
#include "ObjectOptions.h"
#include <string>
#include <unordered_map>
#include <vector>

class DungeonObject{
    protected:
    std::string id;
    std::string name;
    uint32_t objectOptions;// bitmask for all options attached to this object
    std::string description;// base description of the object upon reaching it(rooms description followed by door ways)
    std::string inspectionString;// describes room in more detail, hinting at features and revealing more options 
    std::vector<PlayerChoice> playerChoices;
    public:
    DungeonObject(std::string id, std::string name, std::string Description, std::string inspectionString);
    virtual ~DungeonObject();

    std::string GetID();
    std::string GetName();
    virtual std::string GetObjectDescription();
    virtual std::string GetInspectionDescription();
    void SetObjectOption(int32_t options);
    bool isOptionSet(uint32_t bitmask, ObjectOptions option);

    virtual std::vector<PlayerChoice> GenerateObjectOptions(uint32_t options);
    virtual std::vector<PlayerChoice> GetObjectOptions();

    virtual std::string interactWithObject(ObjectOptions optionChoice);
    
};



class DungeonFeature: public DungeonObject{
    private:
    bool discoverible;//means that the feature is shown upon entering the room that its held within if the room is inspected
    bool interactive;
    public:
    DungeonFeature(std::string id,std::string name, const std::string& Description,std::string inspectionString);
    bool GetDiscoverible();

};
#endif
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
    ObjectOptions objectOptions;// bitmask for all options attached to this object
    std::string Description;// base description of the object upon reaching it(rooms description followed by door ways)
    std::string inspectionString;// describes room in more detail, hinting at features and revealing more options 
    PlayerChoice playerChoices;
    public:
    DungeonObject(std::string id, std::string name, std::string Description, std::string inspectionString);
    virtual ~DungeonObject();

    std::string GetID();
    std::string GetName();
    virtual std::string GetObjectDescription();
    virtual std::string GetInspectionDescription();
    uint32_t SetObjectOption(int32_t options);


    virtual std::vector<PlayerChoice> GenerateObjectOptions(ObjectOptions options);
    virtual std::vector<PlayerChoice> GetObjectOptions();

    virtual std::string interactWithObject(ObjectOptions optionChoice);
    
};



class DungeonFeature: public DungeonObject{
    private:
    bool discoverible;//means that the feature is shown upon entering the room that its held within if the room is inspected
    bool interactive;
    public:
    DungeonFeature(int doorID,std::string name, const std::string& Description,std::string inspectionString);
    bool GetDiscoverible();

};
#endif
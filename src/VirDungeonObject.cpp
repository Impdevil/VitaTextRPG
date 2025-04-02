#include "VirDungeonObject.h"

#pragma region  DungeonObject
DungeonObject::DungeonObject(std::string id,std::string name,  std::string Description, std::string inspectionString)
    : id(id),name(name), description(Description), inspectionString(inspectionString) {
        

}


std::string DungeonObject::GetName()                            {return name;}
std::string DungeonObject::GetID()                              {return id;}
std::string DungeonObject::GetObjectDescription()               {return description;}
std::string DungeonObject::GetInspectionDescription()           {return inspectionString;}

void DungeonObject::SetObjectOption(int32_t options)            {objectOptions = options;}
void DungeonObject::SetDungeonManager(DungeonManager* manager)  {dungeonManager = manager;};
DungeonManager* DungeonObject::GetDungeonManager()           {return dungeonManager;};
DungeonObject::~DungeonObject() {
    // Destructor implementation
    
}

std::vector<PlayerChoice> DungeonObject::GenerateObjectOptions(uint32_t options){
    std::vector<PlayerChoice> newPlayerChoices;
    objectOptions = options;
    if(isOptionSet(objectOptions, ObjectOptions::Inspect))
    {
        PlayerChoice inspectOption{
                ObjectOptions::Inspect,
                "Inspect " + GetName(),
                true,
                0,
                this,
                GetID(),
                dungeonManager
            };
        newPlayerChoices.push_back(inspectOption);

    }
    playerChoices = newPlayerChoices;
    return newPlayerChoices;
}
std::vector<PlayerChoice> DungeonObject::GetObjectOptions(){
    if (playerChoices.empty()){
        playerChoices = GenerateObjectOptions(objectOptions);
    }
    return playerChoices;
}

bool DungeonObject::isOptionSet(uint32_t bitmask, ObjectOptions option) {
    return (bitmask & static_cast<uint32_t>(option)) != 0;
}

std::string DungeonObject::interactWithObject(ObjectOptions optionChoice){
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region Dungeon Feature
DungeonFeature::DungeonFeature(std::string id, std::string name, const std::string& Description, std::string inspectionString)
:DungeonObject(id,name, Description,inspectionString){}

bool DungeonFeature::GetDiscoverible(){
    return discoverible;
}


#pragma endregion
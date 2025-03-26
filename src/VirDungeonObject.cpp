#include "virDungeonObject.h"

#pragma region  DungeonObject
DungeonObject::DungeonObject(std::string id,std::string name,  std::string Description, std:string inspectionString,)
    : id(id),name(name), Description(Description), inspectionString(inspectionString) {
        

}


std::string DungeonObject::GetName();                       {return name;}
std::string DungeonObject::GetID()                          {return id;}
std::string DungeonObject::GetDescription()                 {return description();}
std::string DungeonObject::GetInspectionDescription()       {return inspectionString();}
uint32_t DungeonObject::SetObjectOption(int32_t options){
    objectOptions = options
}
    

virtual std::Vector<ObjectOptions::PlayerChoice> DungeonObject::GenerateObjectOptions(ObjectOptions:ObjectOptions options){
    std::vector<ObjectOptions::PlayerChoice> newPlayerChoices;
    objectOptions = options 
    if(isOptionSet(objectOptions, ObjectOptions::Inspect))
    {
        ObjectOptions::PlayerChoice inspectOption{
            ObjectOptions::Inspect,
            "Inspect " + GetName(),
            true,
            0,
            this
            newPlayerChoices.push_back(inspectOption);
        };
    }
    return newPlayerChoices
}
DungeonObject::GetObjectOptions(){
    if (playerChoices.empty){
        playerChoices = GenerateObjectOptions(objectOptions);
    }
    return playerChoices;
}

bool isOptionSet(uint32_t bitmask, ObjectOptions option) {
    return (bitmask & static_cast<uint32_t>(option)) != 0;
}

virtual std::string DungeonObject::interactWithObject(ObjectOptions::ObjectOptions optionChoice){
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region Dungeon Feature
virtual DungeonFeature::DungeonFeature(std::string featureID, std::string name, const std::string& Description, std:string inspectionString)
:base(id(featureID),name(name), Description(Description),inspectionString(inspectionString){}

bool DungeonFeature::GetDiscoverible(){
    return discoverible;
}


#pragma endregion
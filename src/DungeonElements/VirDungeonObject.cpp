#include "VirDungeonObject.h"
#include "DungeonManager.h"



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
DungeonFeature* DungeonObject::GetDungeonFeature(std::string id){
    return dungeonFeature.at(id);
}

void DungeonObject::AddDungeonFeature(DungeonFeature* newFeature){
    dungeonFeature.emplace(newFeature->GetID(),newFeature);
}

std::vector<PlayerChoice> DungeonObject::GenerateObjectOptions(uint32_t options){
    std::vector<PlayerChoice> newPlayerChoices;
    if (options != 0){
        objectOptions = options;
        logToFile(("Object options set to: " + std::to_string(objectOptions)).c_str());
    }
    else{
        logToFile(("Object options not set."+ GetID()).c_str());
    }
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
    std::vector<PlayerChoice> newPlayerChoices = playerChoices;
    std::vector<PlayerChoice> dungeonFeaturesOptions;
    for(auto& pair:dungeonFeature){
        if (pair.second->GetDiscoverible()){
            dungeonFeaturesOptions = pair.second->GetObjectOptions();
            newPlayerChoices.insert(newPlayerChoices.end(), dungeonFeaturesOptions.begin(),dungeonFeaturesOptions.end());
        }
    }
    return newPlayerChoices;
}

bool DungeonObject::isOptionSet(uint32_t bitmask, ObjectOptions option) {
    return (bitmask & static_cast<uint32_t>(option)) != 0;
}


//all objects should have the inspect option, and so if a object is attached to this then it should be discoverible.
std::string DungeonObject::interactWithObject(ObjectOptions optionChoice){
    bool refreshUI = false;
    std::string returnString;
    switch (optionChoice) {
        case ObjectOptions::Inspect:
        returnString = GetInspectionDescription();
        //logToFile(("DungeonRoom::interactWithObject GetUIContainer"+dungeonManager->GetUIContainerMainViewScene().get()->GetName()).c_str());

        ///find all discoverable features and doors, add them to the UI
        for(auto& pair:dungeonFeature){
            if (!pair.second->GetDiscoverible()){
                //logToFile(("Feature "+ pair.second->GetID() + " is not discoverible").c_str());
                if (pair.second->CheckFeatureDiscoverible()){
                    refreshUI = true;
                    //logToFile(("Feature "+ pair.second->GetID() + " is discovered").c_str());
                    pair.second->SetDiscoverible(true);
                    returnString += " " + pair.second->GetObjectDescription();
                    GetObjectOptions();
                }
            }
        }
        break;
        default:
            returnString = "This action is not possible for the " + GetName() + "!";
            break;
    }
    if (refreshUI){
        dungeonManager->UpdateRoomOptions();
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
void DungeonFeature::SetDiscoverible(bool discoverible){
    this->discoverible = discoverible;
}
bool DungeonFeature::CheckFeatureDiscoverible(){
    if (true){
        discoverible = true;
        return true;

    }
    return false;
}

#pragma endregion
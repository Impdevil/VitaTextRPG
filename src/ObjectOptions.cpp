#include "ObjectOptions.h"
#include "VirDungeonObject.h"
#include "DungeonManager.h"

void PlayerChoice::UseAttachedObject(){
    if (attachedObject == nullptr) {
        logToFile("Error: attachedObject is null.");
        return;
    }
    std::string returnString;
    logToFile(("Using attached object: " + attachedObject->GetName()  + " with option " + ObjectOptionsToString(option)).c_str());
    returnString = attachedObject->interactWithObject(option);
    attachedObject->GetDungeonManager()->GetUIContainer();
    logToFile(("Attached Objects manager: " + attachedObject->GetDungeonManager()->GetID()).c_str());

    if (returnString.empty()){
        logToFile(("No return string from attached object. " + attachedObject->GetName()).c_str());
    }
    else{
        logToFile(("AttachedObjects Returned String:  " + returnString).c_str());
        dungeonManager->SendDataToUI(returnString);
    }
}

std::string PlayerChoice::ObjectOptionsToString(ObjectOptions option) {
    switch (option) {
        case ObjectOptions::None:
            return "None";
        case ObjectOptions::Inspect:
            return "Inspect";
        case ObjectOptions::TakeItem:
            return "Take Item";
        case ObjectOptions::UnlockDoor:
            return "Unlock Door";
        case ObjectOptions::PassThrough:
            return "Pass Through";
        case ObjectOptions::OpenChest:
            return "Open Chest";
        case ObjectOptions::DisarmTrap:
            return "Disarm Trap";
        default:
            return "Unknown Option";
    }
}
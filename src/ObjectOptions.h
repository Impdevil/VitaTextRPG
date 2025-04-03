#ifndef OBJECTOPTIONS_H
#define OBJECTOPTIONS_H

#include <string>
#include "DebugLogging.h"
//#include "VirDungeonObject.h"

class DungeonObject;
class DungeonManager;

enum class ObjectOptions : uint32_t {///bitmask
    None            = 0b00000000,  // No options available
    Inspect         = 0b00000001,  // Inspect the object and show features with in a room, show more detail of a feature
    TakeItem        = 0b00000010,  // Enter a connected room
    UnlockDoor      = 0b00000100,  // Unlock a door
    PassThrough     = 0b00001000,  // Pass through the door
    Open            = 0b00010000,  // Open a treasure chest
    DisarmTrap      = 0b00100000   // Disarm a trap
};
struct PlayerChoice{
    ObjectOptions option;
    std::string optionString;
    bool usable;
    int priority;
    DungeonObject* attachedObject;
    std::string id;
    DungeonManager* dungeonManager;

    void UseAttachedObject();
    std::string ObjectOptionsToString(ObjectOptions option);

};



#endif
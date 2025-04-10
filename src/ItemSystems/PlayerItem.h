#ifndef PLAYERITEM_H
#define PLAYERITEM_H

#include <string>
template <typename ParentType>
class PlayerItem {
public:
    // Constructor for PlayerItem
    PlayerItem(int id, const std::string& name, const std::string& description, ParentType* parent = nullptr)
        : id(id), name(name), description(description), used(false), parent(static_cast<void*>(parent)) {}

    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    bool isUsed() const { return used; }

    ParentType* getParent() const { return static_cast<ParentType*>(parent); }

    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setDescription(const std::string& newDescription) { description = newDescription; }
    void markAsUsed() { used = true; }
    void setParent(ParentType* newParent) { parent = newParent; }
    // Apply custom data to a DungeonObject
    void interactWithDungeonObject(DungeonObject& dungeonObject) const {
        //dungeonObject.applyCustomData(customData);
    }

private:
    int id; // Unique ID for the item
    std::string name; // Name of the item
    std::string description; // Description of the item
    bool used; // Tracks whether the item has been used
    void* parent; // Pointer to the parent object (DungeonRoom, DungeonFeature, Player, NPC, etc.)
};
class ItemTracking {
public:
    ItemTracking() :  trackerID(0), currentLocation(nullptr) {}



    // Getters and setters for trackerID
    int getTrackerID() const { return trackerID; }
    void setTrackerID(int id) { trackerID = id; }

    // Getters and setters for currentLocation
    ParentType* getCurrentLocation() const { return currentLocation; }
    void setCurrentLocation(ParentType* location) { currentLocation = location; }

private:
    int trackerID; // Unique tracker ID for the item
    ParentType* currentLocation; // Pointer to the current location of the item
};

#endif
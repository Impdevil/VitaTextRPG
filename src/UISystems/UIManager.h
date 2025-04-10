// filepath: /home/rnbl/vitaProjects/project1/src/UIManager.h
#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>
#include "UISceneContainer.h"

class UIManager {
private:
    std::unordered_map<std::string, std::shared_ptr<UISceneContainer>> sceneContainers;

    // Private constructor for singleton pattern
    UIManager() = default;

public:
    // Delete copy constructor and assignment operator to prevent copying
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    // Static method to access the singleton instance
    static UIManager& GetInstance();

    // Methods to manage UISceneContainers
    void AddSceneContainer(std::shared_ptr<UISceneContainer> sceneContainer);
    void RemoveSceneContainer(const std::string& name);
    std::shared_ptr<UISceneContainer> GetSceneContainer(const std::string& name);
};

#endif
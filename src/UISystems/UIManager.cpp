// filepath: /home/rnbl/vitaProjects/project1/src/UIManager.cpp
#include "UIManager.h"
#include "../Systems/DebugLogging.h"

UIManager& UIManager::GetInstance() {
    static UIManager instance;
    return instance;
}

void UIManager::AddSceneContainer(std::shared_ptr<UISceneContainer> sceneContainer) {
    if (sceneContainers.find(sceneContainer->GetName()) != sceneContainers.end()) {
        logToFile(("SceneContainer with name '" + sceneContainer->GetName() + "' already exists.").c_str());
        return;
    }
    sceneContainers[sceneContainer->GetName()] = sceneContainer;
    logToFile(("Added SceneContainer: " + sceneContainer->GetName()).c_str());
}

void UIManager::RemoveSceneContainer(const std::string& name) {
    if (sceneContainers.erase(name) > 0) {
        logToFile(("Removed SceneContainer: " + name).c_str());
    } else {
        logToFile(("SceneContainer with name '" + name + "' does not exist.").c_str());
    }
}

std::shared_ptr<UISceneContainer> UIManager::GetSceneContainer(const std::string& name) {
    if (sceneContainers.find(name) != sceneContainers.end()) {
        return sceneContainers[name];
    }
    logToFile(("SceneContainer with name '" + name + "' not found.").c_str());
    return nullptr;
}
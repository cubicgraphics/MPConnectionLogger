#pragma once

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "modloader/shared/modloader.hpp"

// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#define LOG_INFO(...) getLogger().info(__VA_ARGS__)
#define LOG_ERROR(...) getLogger().error(__VA_ARGS__)

#include "GlobalNamespace/MockPlayer.hpp"
using namespace GlobalNamespace;

//extern GlobalNamespace::MockPlayer _mockPlayer;

//void CreateMockPlayer();

#include "HMUI/ViewController.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"

void GameplaySettings(UnityEngine::GameObject* gameObject, bool firstActivation);

// Define these functions here so that we can easily read configuration and log information from other files
Configuration& getConfig();
Logger& getLogger();
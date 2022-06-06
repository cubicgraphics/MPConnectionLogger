#include "main.hpp"

#include "questui/shared/QuestUI.hpp"


#include "GlobalNamespace/MultiplayerBigAvatarAnimator.hpp"
#include "GlobalNamespace/EaseType.hpp"

#include "GlobalNamespace/MockPlayer.hpp"
#include "GlobalNamespace/MockPlayerSettings.hpp"
#include "GlobalNamespace/IConnectedPlayer.hpp"
#include "GlobalNamespace/ConnectedPlayerManager.hpp"
#include "GlobalNamespace/ConnectedPlayerManager_ConnectedPlayer.hpp"
#include "GlobalNamespace/ConnectedPlayerManager_PlayerConnectedPacket.hpp"
#include "GlobalNamespace/ConnectedPlayerManager_PlayerAvatarPacket.hpp"
#include "MultiplayerCore/shared/GlobalFields.hpp"
#include "GlobalNamespace/IConnection.hpp"
#include "GlobalNamespace/DisconnectedReason.hpp"
#include "GlobalNamespace/ConnectionFailedReason.hpp"
#include "GlobalNamespace/ConnectedPlayerManager_KickPlayerPacket.hpp"
#include "GlobalNamespace/ConnectedPlayerManager_PlayerDisconnectedPacket.hpp"


using namespace GlobalNamespace;

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
/*
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}
*/

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;
	
    getLogger().info("Completed setup!");
}

//GlobalNamespace::ConnectedPlayerManager* _ConnectedPlayerManager;

//GlobalNamespace::MockPlayer _mockPlayer;
//GlobalNamespace::MockPlayerSettings settings = GlobalNamespace::MockPlayerSettings();
/*
void CreateMockPlayer()
{
    settings.userName = "TESTMOCK334";
    settings.userId = "TESTMOCKPLAYER";
    settings.sortIndex = 3;
    _mockPlayer = GlobalNamespace::MockPlayer();
    _mockPlayer.settings = &settings;
    _mockPlayer.isMe = false;
    _mockPlayer.SetState("player", true);
    _mockPlayer.isConnected = true;
    _mockPlayer.SetState("wants_to_play_next_level", true);
    _mockPlayer.multiplayerAvatarData = MultiplayerCore::_multiplayerSessionManager->get_localPlayer()->get_multiplayerAvatarData();

    _ConnectedPlayerManager = MultiplayerCore::_multiplayerSessionManager->connectedPlayerManager;

    ConnectedPlayerManager::PlayerConnectedPacket* mockplayerpacket;
    mockplayerpacket->Init(0, "TESTMOCK334", "TESTMOCKPLAYER", false);


    ConnectedPlayerManager::ConnectedPlayer* connectedmockplayer = ConnectedPlayerManager::ConnectedPlayer::CreateRemoteConnectedPlayer(_ConnectedPlayerManager,_ConnectedPlayerManager->GetNextConnectionId(),mockplayerpacket,NULL);
    _ConnectedPlayerManager->AddPlayer(connectedmockplayer);
}
*/
MAKE_HOOK_MATCH(ConnectedPlayerManager_HandleConnected, &ConnectedPlayerManager::HandleConnected, void, ConnectedPlayerManager* self){
    getLogger().debug("ConnectedPlayerManager_HandleConnected, this is local player (local player joined)");
    ConnectedPlayerManager_HandleConnected(self);
}
MAKE_HOOK_MATCH(ConnectedPlayerManager_HandleInitialized, &ConnectedPlayerManager::HandleInitialized, void, ConnectedPlayerManager* self){
    getLogger().debug("ConnectedPlayerManager_HandleInitialized");
    ConnectedPlayerManager_HandleInitialized(self);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_HandleServerPlayerConnected, &ConnectedPlayerManager::HandleServerPlayerConnected, void, ConnectedPlayerManager* self, ConnectedPlayerManager::PlayerConnectedPacket* packet, ::GlobalNamespace::IConnectedPlayer* iPlayer){
    getLogger().debug("ConnectedPlayerManager_HandleServerPlayerConnected, Server player connected, IPlayerUserName: %s", static_cast<std::string>(iPlayer->get_userName()).c_str());
    ConnectedPlayerManager_HandleServerPlayerConnected(self, packet, iPlayer);
}
MAKE_HOOK_MATCH(ConnectedPlayerManager_HandleConnectionConnected, &ConnectedPlayerManager::HandleConnectionConnected, void, ConnectedPlayerManager* self, IConnection* connection){
    getLogger().debug("ConnectedPlayerManager_HandleConnectionConnected, Connection connected, IConnectionUserName: %s", static_cast<std::string>(connection->get_userName()).c_str());
    ConnectedPlayerManager_HandleConnectionConnected(self, connection);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_HandlePlayerAvatarUpdate, &ConnectedPlayerManager::HandlePlayerAvatarUpdate, void, ConnectedPlayerManager* self, ConnectedPlayerManager::PlayerAvatarPacket* packet, IConnectedPlayer* iPlayer){
    getLogger().debug("ConnectedPlayerManager_HandlePlayerAvatarUpdate, AvatarUpdate, IConnectedPlayerUserName: %s", static_cast<std::string>(iPlayer->get_userName()).c_str());
    ConnectedPlayerManager_HandlePlayerAvatarUpdate(self, packet, iPlayer);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_AddPlayer, &ConnectedPlayerManager::AddPlayer, void, ConnectedPlayerManager* self, ConnectedPlayerManager::ConnectedPlayer* player){
    getLogger().debug("ConnectedPlayerManager_AddPlayer, Adding player, ConnectedPlayerUserName: %s", static_cast<std::string>(player->get_userName()).c_str());
    ConnectedPlayerManager_AddPlayer(self, player);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_ConnectedPlayer_CreateRemoteConnectedPlayer, &ConnectedPlayerManager::ConnectedPlayer::CreateRemoteConnectedPlayer, ConnectedPlayerManager::ConnectedPlayer*, ConnectedPlayerManager* manager, uint8_t connectionId, ConnectedPlayerManager::PlayerConnectedPacket* packet, ConnectedPlayerManager::ConnectedPlayer* parent){
    getLogger().debug("ConnectedPlayerManager_ConnectedPlayer_CreateRemoteConnectedPlayer, creating remote player, ConnectedPlayerUserName: %s", static_cast<std::string>(parent->get_userName()).c_str());
    return ConnectedPlayerManager_ConnectedPlayer_CreateRemoteConnectedPlayer(manager, connectionId, packet, parent);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_ConnectedPlayer_CreateLocalPlayer, &ConnectedPlayerManager::ConnectedPlayer::CreateLocalPlayer, ConnectedPlayerManager::ConnectedPlayer*, ConnectedPlayerManager* manager, StringW userId, StringW userName, bool isConnectionOwner, ArrayW<uint8_t> publicEncryptionKey, ArrayW<uint8_t> random){
    getLogger().debug("ConnectedPlayerManager_ConnectedPlayer_CreateLocalPlayer, creating local player, LocalPlayerUserName: %s", static_cast<std::string>(userName).c_str());
    return ConnectedPlayerManager_ConnectedPlayer_CreateLocalPlayer(manager, userId, userName, isConnectionOwner, publicEncryptionKey, random);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_ConnectedPlayer_CreateDirectlyConnectedPlayer, &ConnectedPlayerManager::ConnectedPlayer::CreateDirectlyConnectedPlayer, ConnectedPlayerManager::ConnectedPlayer*, ConnectedPlayerManager* manager, uint8_t connectionId, IConnection* connection){
    getLogger().debug("ConnectedPlayerManager_ConnectedPlayer_CreateDirectlyConnectedPlayer, creating directly connected player, ConnectedPlayerUserName: %s", static_cast<std::string>(connection->get_userName()).c_str());
    return ConnectedPlayerManager_ConnectedPlayer_CreateDirectlyConnectedPlayer(manager, connectionId, connection);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_HandleConnectionDisconnected, &ConnectedPlayerManager::HandleConnectionDisconnected, void, ConnectedPlayerManager* self, IConnection* connection, DisconnectedReason disconnectedReason){
    getLogger().debug("ConnectedPlayerManager_HandleConnectionDisconnected, Handle ConnectionDisconnected Reason: %d UserName: %s", disconnectedReason.value, static_cast<std::string>(connection->get_userId()).c_str());
    ConnectedPlayerManager_HandleConnectionDisconnected(self, connection, disconnectedReason);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_HandleServerPlayerDisconnected, &ConnectedPlayerManager::HandleServerPlayerDisconnected, void, ConnectedPlayerManager* self, ConnectedPlayerManager::PlayerDisconnectedPacket* packet, IConnectedPlayer* iPlayer){
    getLogger().debug("ConnectedPlayerManager_HandleServerPlayerDisconnected, Handle ConnectionDisconnected Reason: %d ,UserName: %s", packet->disconnectedReason.value, static_cast<std::string>(iPlayer->get_userName()).c_str());
    ConnectedPlayerManager_HandleServerPlayerDisconnected(self, packet, iPlayer);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_HandleKickPlayerPacket, &ConnectedPlayerManager::HandleKickPlayerPacket, void, ConnectedPlayerManager* self, ConnectedPlayerManager::KickPlayerPacket* packet, IConnectedPlayer* iPlayer){
    getLogger().debug("ConnectedPlayerManager_HandleKickPlayerPacket, Handle ConnectionDisconnected Reason: %d ,UserName: %s", packet->disconnectedReason.value, static_cast<std::string>(iPlayer->get_userName()).c_str());
    ConnectedPlayerManager_HandleKickPlayerPacket(self, packet, iPlayer);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_HandleDisconnected, &ConnectedPlayerManager::HandleDisconnected, void, ConnectedPlayerManager* self, DisconnectedReason disconnectedReason){
    getLogger().debug("ConnectedPlayerManager_HandleDisconnected, Handle ConnectionDisconnected Reason: %d", disconnectedReason.value);
    ConnectedPlayerManager_HandleDisconnected(self, disconnectedReason);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_KickPlayer, &ConnectedPlayerManager::KickPlayer, void, ConnectedPlayerManager* self, StringW userId, DisconnectedReason disconnectedReason){
    getLogger().debug("ConnectedPlayerManager_KickPlayer, Handle ConnectionDisconnected Reason: %d , UserId: %s", disconnectedReason.value, static_cast<std::string>(userId).c_str());
    ConnectedPlayerManager_KickPlayer(self, userId, disconnectedReason);
}

MAKE_HOOK_MATCH(ConnectedPlayerManager_HandleConnectionFailed, &ConnectedPlayerManager::HandleConnectionFailed, void, ConnectedPlayerManager* self, ConnectionFailedReason reason){
    getLogger().debug("ConnectedPlayerManager_HandleConnectionFailed, Handle ConnectionFailedReason Reason: %d", reason.value);
    ConnectedPlayerManager_HandleConnectionFailed(self, reason);
}


// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    QuestUI::Register::RegisterGameplaySetupMenu(modInfo, QuestUI::Register::Online, GameplaySettings);

    getLogger().info("Installing hooks...");

    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandleConnected);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandleInitialized);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandleServerPlayerConnected);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandleConnectionConnected);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandlePlayerAvatarUpdate);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_AddPlayer);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_ConnectedPlayer_CreateRemoteConnectedPlayer);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_ConnectedPlayer_CreateLocalPlayer);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_ConnectedPlayer_CreateDirectlyConnectedPlayer);

    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandleConnectionDisconnected);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandleServerPlayerDisconnected);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandleKickPlayerPacket);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandleDisconnected);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_KickPlayer);
    INSTALL_HOOK(getLogger(), ConnectedPlayerManager_HandleConnectionFailed);



    getLogger().info("Installed all hooks!");
}
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "HMUI/Touchable.hpp"

using namespace GlobalNamespace;
using namespace QuestUI;
using namespace HMUI;

#include "MultiplayerCore/shared/GlobalFields.hpp"
#include "questui/shared/CustomTypes/Components/WeakPtrGO.hpp"


WeakPtrGO<UnityEngine::GameObject> settingsGO;

UnityEngine::Transform* GetSubcontainer(UnityEngine::UI::VerticalLayoutGroup* vertical) {
    auto horizontal = BeatSaberUI::CreateHorizontalLayoutGroup(vertical);
    horizontal->GetComponent<UnityEngine::UI::LayoutElement*>()->set_minHeight(8);
    horizontal->set_childForceExpandHeight(true);
    horizontal->set_childAlignment(UnityEngine::TextAnchor::MiddleCenter);
    return horizontal->get_transform();
}


void GameplaySettings(UnityEngine::GameObject* gameObject, bool firstActivation) {
    if(firstActivation) {
        auto vertical = BeatSaberUI::CreateVerticalLayoutGroup(gameObject->get_transform());
        
        // raise up container
        vertical->get_rectTransform()->set_anchoredPosition({0, 31});

        auto AddMockPlayer = BeatSaberUI::CreateUIButton(GetSubcontainer(vertical), "Add Mock player", {-8, 16}, {40, 8}, [](){
            //CreateMockPlayer();
       });
    
        BeatSaberUI::AddHoverHint(AddMockPlayer, "Adds a mock player into the lobby");
        }
    settingsGO = gameObject;
}


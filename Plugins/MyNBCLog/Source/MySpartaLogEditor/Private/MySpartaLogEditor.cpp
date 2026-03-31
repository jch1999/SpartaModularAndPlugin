#include "MySpartaLogEditor.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/SListView.h"
#include "ToDoListAsset.h"

// 탭의 고유 이름 (ID)
static const FName ToDoTabName("ToDoListTab");

void FMySpartaLogEditorModule::StartupModule()
{
    // 탭 스포너 등록 — 이 탭이 어떻게 생겼는지 엔진에 알려줌
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
        ToDoTabName,
        FOnSpawnTab::CreateRaw(this, &FMySpartaLogEditorModule::OnSpawnToDoTab)
    )
    .SetDisplayName(FText::FromString("ToDo List"))
    .SetMenuType(ETabSpawnerMenuType::Hidden); // 메뉴는 직접 등록할 거라 Hidden

    // 메뉴 등록은 에디터가 완전히 로드된 뒤 해야 함
    UToolMenus::RegisterStartupCallback(
        FSimpleMulticastDelegate::FDelegate::CreateRaw(
            this, &FMySpartaLogEditorModule::RegisterMenus
        )
    );
}

void FMySpartaLogEditorModule::ShutdownModule()
{
    // 탭 등록 해제
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ToDoTabName);

    // 메뉴 등록 해제
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);
}

void FMySpartaLogEditorModule::RegisterMenus()
{
    // 메뉴 소유권 설정 (ShutdownModule에서 일괄 해제할 수 있게)
    FToolMenuOwnerScoped OwnerScoped(this);

    // "Tools" 메뉴를 가져옴
    UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
    FToolMenuSection& Section = Menu->FindOrAddSection("MySpartaTools");
    Section.Label = FText::FromString("Sparta Tools");

    // "Open ToDo List" 메뉴 항목 추가
    Section.AddMenuEntry(
        "OpenToDoList",
        FText::FromString("Open ToDo List"),
        FText::FromString("Opens the ToDo List panel"),
        FSlateIcon(),
        FUIAction(FExecuteAction::CreateRaw(
            this, &FMySpartaLogEditorModule::OpenToDoListTab
        ))
    );
}

void FMySpartaLogEditorModule::OpenToDoListTab()
{
    // 이미 열려있으면 포커스, 없으면 새로 생성
    FGlobalTabmanager::Get()->TryInvokeTab(ToDoTabName);
}

TSharedRef<SDockTab> FMySpartaLogEditorModule::OnSpawnToDoTab(const FSpawnTabArgs& Args)
{
    // 탭 안에 표시할 Slate UI
    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SVerticalBox)

            // 타이틀
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(10.f)
            [
                SNew(STextBlock)
                .Text(FText::FromString("My ToDo List"))
            ]

            // 임시 버튼 (Phase 3에서 실제 기능으로 교체할 자리)
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(10.f)
            [
                SNew(SButton)
                .Text(FText::FromString("Add Item (WIP)"))
            ]
        ];
}

// 모듈 등록 매크로
IMPLEMENT_MODULE(FMySpartaLogEditorModule, MySpartaLogEditor);
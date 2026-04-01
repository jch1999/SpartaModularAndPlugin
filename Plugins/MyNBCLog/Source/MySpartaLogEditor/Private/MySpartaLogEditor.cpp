#include "MySpartaLogEditor.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "PropertyCustomizationHelpers.h"   // 에셋 피커
#include "AssetRegistry/AssetRegistryModule.h"
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

// 탭 UI 전체 구성
TSharedRef<SDockTab> FMySpartaLogEditorModule::OnSpawnToDoTab(const FSpawnTabArgs& Args)
{
    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SVerticalBox)

            // ── 1. 에셋 피커 ──────────────────────────────
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(8.f)
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(FMargin(0.f, 0.f, 8.f, 0.f))
                [
                    SNew(STextBlock)
                    .Text(FText::FromString("ToDoList 에셋:"))
                ]

                + SHorizontalBox::Slot()
                .FillWidth(1.f)
                [
                    // SObjectPropertyEntryBox: 콘텐츠 브라우저와 연동되는
                    // 에셋 선택 위젯. PropertyCustomizationHelpers에 있음
                    SNew(SObjectPropertyEntryBox)
                    .AllowedClass(UToDoListAsset::StaticClass())
                    .ObjectPath_Lambda([this]()
                    {
                        // 현재 선택된 에셋의 경로를 반환
                        return CurrentAsset
                            ? CurrentAsset->GetPathName()
                            : FString();
                    })
                    .OnObjectChanged_Lambda([this](const FAssetData& AssetData)
                    {
                        // 에셋이 선택되면 로드하고 리스트 갱신
                        CurrentAsset = Cast<UToDoListAsset>(
                            AssetData.GetAsset()
                        );
                        RefreshList();
                    })
                ]
            ]

            // ── 구분선 ────────────────────────────────────
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SSeparator)
            ]

            // ── 2. ToDo 리스트 ────────────────────────────
            + SVerticalBox::Slot()
            .FillHeight(1.f)
            .Padding(8.f)
            [
                SAssignNew(ListView, SListView<TSharedPtr<int32>>)
                .ListItemsSource(&ListIndices)
                .OnGenerateRow(SListView<TSharedPtr<int32>>::FOnGenerateRow::CreateRaw(this, &FMySpartaLogEditorModule::OnGenerateRow))
                .SelectionMode(ESelectionMode::None)
            ]

            // ── 구분선 ────────────────────────────────────
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SSeparator)
            ]

            // ── 3. 항목 추가 버튼 ─────────────────────────
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(8.f)
            [
                SNew(SButton)
                .Text(FText::FromString("+ 항목 추가"))
                .OnClicked(FOnClicked::CreateRaw(this, &FMySpartaLogEditorModule::OnAddItemClicked))
            ]
        ];
}

// 에셋 피커에서 에셋이 선택됐을 때 호출
void FMySpartaLogEditorModule::RefreshList()
{
    ListIndices.Empty();
    
    if (CurrentAsset)
    {
        // Items 개수만큼 인덱스 생성 (0, 1, 2, ...)
        for (int32 i = 0; i < CurrentAsset->Items.Num(); ++i)
        {
            ListIndices.Add(MakeShared<int32>(i));
        }
    }
    
    // 리스트 뷰가 유효하면 UI 갱신
    if (ListView.IsValid())
    {
        ListView->RequestListRefresh();
    }
}

// + 버튼 클릭 핸들러
FReply FMySpartaLogEditorModule::OnAddItemClicked()
{
    if (!CurrentAsset)
    {
        return FReply::Handled();
    }
    
    // 에셋에 새 항목 추가
    FToDoItem NewItem;
    NewItem.Text = FString::Printf(TEXT("새 항목: %d"), CurrentAsset->Items.Num() +1);
    NewItem.bCompleted = false;
    
    // 에셋을 dirty 상태로 표시 (저장 필요함을 엔진에 알림)
    CurrentAsset->MarkPackageDirty();
    
    // 리스트 갱신
    RefreshList();
    
    return FReply::Handled();
}

// SListView가 각 행을 그릴 때 호출
TSharedRef<ITableRow> FMySpartaLogEditorModule::OnGenerateRow(
    TSharedPtr<int32> ItemIndex,
    const TSharedRef<STableViewBase>& OwnerTable)
{
    // Item이 유효한지 확인
    if (!ItemIndex.IsValid() || !CurrentAsset)
    {
        return SNew(STableRow<TSharedPtr<int32>>, OwnerTable);
    }
    
    const int32 Index = *ItemIndex;

    if (!CurrentAsset->Items.IsValidIndex(Index))
    {
        return SNew(STableRow<TSharedPtr<int32>>, OwnerTable);
    }
    
    // 캡처용 변수
    UToDoListAsset* Asset = CurrentAsset;
    
    return SNew(STableRow<TSharedPtr<int32>>, OwnerTable)
        .Padding(FMargin(4.f,2.f))
        [
            SNew(SHorizontalBox)
            
            // 체크박스
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            [
                SNew(SCheckBox)
                .IsChecked_Lambda([Asset, Index]()
                {
                    if (Asset && Asset->Items.IsValidIndex(Index))
                    {
                        return Asset->Items[Index].bCompleted
                            ? ECheckBoxState::Checked
                            : ECheckBoxState::Unchecked;
                    }
                    return ECheckBoxState::Unchecked;
                })
                .OnCheckStateChanged_Lambda([Asset, Index](ECheckBoxState NewState)
                {
                    // 체크박스가 바뀌면 원본 에셋의 Items도 같이 수정
                    if (Asset && Asset->Items.IsValidIndex(Index))
                    {
                        Asset->Items[Index].bCompleted =
                            (NewState == ECheckBoxState::Checked);
                        Asset->MarkPackageDirty();
                    }
                })
            ]
            // 텍스트
            + SHorizontalBox::Slot()
            .VAlign(VAlign_Center)
            .Padding(FMargin(6.f,0.f))
            [
                SNew(STextBlock)
                .Text_Lambda([Asset, Index]()
                {
                    if (Asset && Asset->Items.IsValidIndex(Index))
                    {
                        return FText::FromString(Asset->Items[Index].Text);
                    }
                    return FText::GetEmpty();
                })
            ]
        ];
}

// 모듈 등록 매크로
IMPLEMENT_MODULE(FMySpartaLogEditorModule, MySpartaLogEditor);
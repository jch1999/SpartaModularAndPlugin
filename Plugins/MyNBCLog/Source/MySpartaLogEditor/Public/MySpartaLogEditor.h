#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "ToDoListAsset.h"

// 타입 별칭으로 미리 정의해두면 반복 작성을 줄일 수 있어요
using FToDoItemPtr = TSharedPtr<FToDoItem, ESPMode::NotThreadSafe>;

class FMySpartaLogEditorModule : public IModuleInterface
{
public:
	// 모듈이 로드될 때 호출 - 여기서 메뉴/패널 등록
	virtual void StartupModule() override;
	
	// 모듈이 언로드될 때 호출 - 여기서 등록 해제
	virtual void ShutdownModule() override;

private:
	// 에디터 메뉴에 ToDoList 항목을 등록하는 함수
	void RegisterMenus();
	
	// 메뉴 클릭 시 ToDoList 패널을 여는 함수
	void OpenToDoListTab();
	
	// 탭이 생성될 때 내용을 채우는 함수
	TSharedRef<SDockTab> OnSpawnToDoTab(const FSpawnTabArgs& Args);
	
	// 리스트를 다시 그리는 함수
	void RefreshList();
	
	// + 버튼 클릭 시 새 항목 추가
	FReply OnAddItemClicked();
	
	// 각 리스트 행을 생성하는 함수 (SListView에서 호출)
	TSharedRef<ITableRow> OnGenerateRow(
		TSharedPtr<int32> ItemIndex,
		const TSharedRef<STableViewBase>& OwnerTable
	);
	
private:
	// 현재 선택된 에셋
	TObjectPtr<UToDoListAsset> CurrentAsset = nullptr;
	
	// 데이터의 인덱스 뱌열
	TArray<TSharedPtr<int32>> ListIndices;
	TSharedPtr<SListView<TSharedPtr<int32>>> ListView;
};
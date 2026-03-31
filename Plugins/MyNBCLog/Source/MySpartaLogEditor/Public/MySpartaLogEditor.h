#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

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
};
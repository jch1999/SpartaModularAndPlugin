// MySpartaLog.h

#pragma once

#include "CoreMinimal.h"
#include "../../../../../ProgramFiles/EpicGames/UE_5.6/Engine/Source/Runtime/Core/Public/Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

MYSPARTALOG_API DECLARE_LOG_CATEGORY_EXTERN(LogMySpartaModule, Log, All);

class FMySpartaLog : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
// MySpartaLog.cpp

#include "MySpartaLog.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogMySpartaModule);

void FMySpartaLog::StartupModule()
{
	UE_LOG(LogMySpartaModule, Warning, TEXT("MySpartaLog is start"));
}

void FMySpartaLog::ShutdownModule() {}

IMPLEMENT_MODULE(FMySpartaLog, MySpartaLog);
// TestActor.cpp

#include "TestActor.h"
#include "MySpartaLog.h"

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogMySpartaModule, Display, TEXT("Actor call MySpartaLog Module!"));
}
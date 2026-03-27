// TestActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class MYSPARTALOG_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:
	ATestActor();
	
protected:
	virtual void BeginPlay() override;
};



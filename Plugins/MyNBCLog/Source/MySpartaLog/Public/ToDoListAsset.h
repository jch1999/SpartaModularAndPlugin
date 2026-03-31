#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ToDoListAsset.generated.h"

USTRUCT(BlueprintType)
struct FToDoItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ToDo")
	FString Text;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ToDo")
	bool bCompleted = false;
};

UCLASS(BlueprintType)
class MYSPARTALOG_API UToDoListAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ToDo")
	TArray<FToDoItem> Items;
};
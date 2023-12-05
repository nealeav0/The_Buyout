// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "Dialogue.generated.h"

USTRUCT(BlueprintType)
struct FDialogueStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RowID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ParentRowID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> ChildrenIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Dialogue;


	FDialogueStruct()
	{
		RowID = 0;
		ParentRowID = -1;
		ChildrenIDs.Add(1);
		ChildrenIDs.Add(3);
		Dialogue = FString(TEXT("This is dialogue one."));
	}
};

/**
 * 
 */
UCLASS()
class CS354P_F_API UDialogue : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};

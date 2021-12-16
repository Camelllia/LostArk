// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostArk.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "LAGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FLACharacterData : public FTableRowBase
{
	GENERATED_BODY()



public:
	FLACharacterData() : Level(1), MaxHp(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};
/**
 * 
 */
UCLASS()
class LOSTARK_API ULAGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ULAGameInstance();

	virtual void Init() override;
	FLACharacterData* GetLACharacterData(int32 Level);

private:
	UPROPERTY()
	class UDataTable* LACharacterTable;
	
};

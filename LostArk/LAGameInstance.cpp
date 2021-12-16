// Fill out your copyright notice in the Description page of Project Settings.


#include "LAGameInstance.h"

ULAGameInstance::ULAGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/Asp/GameData/LACharacterData.LACharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LACHARACTER(*CharacterDataPath);
	if (DT_LACHARACTER.Succeeded())
	{
		LACharacterTable = DT_LACHARACTER.Object;
	}
}

void ULAGameInstance::Init()
{
	Super::Init();
}

FLACharacterData * ULAGameInstance::GetLACharacterData(int32 Level)
{
	return LACharacterTable->FindRow<FLACharacterData>(*FString::FromInt(Level), TEXT(""));
}
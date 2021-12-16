// Fill out your copyright notice in the Description page of Project Settings.


#include "LACharacterStatComponent.h"
#include "LAGameInstance.h"

// Sets default values for this component's properties
ULACharacterStatComponent::ULACharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;

	// ...
}


// Called when the game starts
void ULACharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void ULACharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void ULACharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	ULAGameInstance* LAGameInstance = Cast<ULAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (nullptr == LAGameInstance) return;

	CurrentStatData = LAGameInstance->GetLACharacterData(NewLevel);
	if (CurrentStatData)
	{
		Level = NewLevel;
		//CurrentHp = CurrentStatData->MaxHp;
	}
	else
	{
		// 데이터 테이블에 없는 레벨일 때
		UE_LOG(LogClass, Warning, TEXT("Level %d data doesn't exist."), NewLevel);
	}
	
}




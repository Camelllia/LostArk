// Fill out your copyright notice in the Description page of Project Settings.


#include "LAAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LAAnimInstance.h"

const FName ALAAIController::HomePosKey(TEXT("HomePos"));
const FName ALAAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ALAAIController::TargetKey(TEXT("Target"));

ALAAIController::ALAAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_LACharacter.BB_LACharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_LACharacter.BT_LACharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ALAAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("Dont have BT"));
		}
	}
}





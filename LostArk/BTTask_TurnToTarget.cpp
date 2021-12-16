// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "LAAIController.h"
#include "LACharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto LACharacter = Cast<ALACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == LACharacter)
		return EBTNodeResult::Failed;

	auto Target = Cast<ALACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ALAAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - LACharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromZ(LookVector).Rotator();
	LACharacter->SetActorRotation(FMath::RInterpTo(LACharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 20.0f));

	return EBTNodeResult::Succeeded;
}


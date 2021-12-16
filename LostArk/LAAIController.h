// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostArk.h"
#include "AIController.h"
#include "LAAIController.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API ALAAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALAAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	class ULAAnimInstance* LAAnim;


	
};

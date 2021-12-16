// Fill out your copyright notice in the Description page of Project Settings.


#include "LAPawn.h"

// Sets default values
ALAPawn::ALAPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALAPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALAPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALAPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


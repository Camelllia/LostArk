// Fill out your copyright notice in the Description page of Project Settings.


#include "LAGameModeBase.h"
#include "LACharacter.h"
#include "LAPawn.h"

ALAGameModeBase::ALAGameModeBase()
{
	DefaultPawnClass = ALACharacter::StaticClass();
}

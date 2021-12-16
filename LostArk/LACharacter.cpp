// Fill out your copyright notice in the Description page of Project Settings.


#include "LACharacter.h"
#include "LAAnimInstance.h"
#include "LACharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "LAAIController.h"


// Sets default values
ALACharacter::ALACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CharacterStat = CreateDefaultSubobject<ULACharacterStatComponent>(TEXT("CHARACTERSTAT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KWANG(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangSunrise.KwangSunrise"));
	if (KWANG.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(KWANG.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> KWANG_ANIM(TEXT("/Game/Animations/KwangAnimBlueprint.KwangAnimBlueprint_C"));
	if (KWANG_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(KWANG_ANIM.Class);
	}

	ViewPoint(0);

	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("LACharacter"));

	CurrentHp = 100;
	MaxHp = 100;

	CurrentMana = 100;
	MaxMana = 100;

	AIControllerClass = ALAAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
// Called when the game starts or when spawned
void ALACharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ALACharacter::ViewPoint(int32 ViewPoint)
{
	if (ViewPoint == 0)
	{
		SpringArm->TargetArmLength = 900.0f;
		SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	}
}

// Called every frame
void ALACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DirectionToMove.SizeSquared() > 0.0f && !IsAttacking)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		AddMovementInput(DirectionToMove);
	}
}

// Called to bind functionality to input
void ALACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ALACharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ALACharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ALACharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ALACharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ALACharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Damaged"), EInputEvent::IE_Pressed, this, &ALACharacter::Damaged);
	PlayerInputComponent->BindAction(TEXT("Ability"), EInputEvent::IE_Pressed, this, &ALACharacter::Ability);
	PlayerInputComponent->BindAction(TEXT("NAbility"), EInputEvent::IE_Pressed, this, &ALACharacter::NAbility);
	PlayerInputComponent->BindAction(TEXT("PlusMana"), EInputEvent::IE_Pressed, this, &ALACharacter::PlusMana);
	PlayerInputComponent->BindAction(TEXT("Cast"), EInputEvent::IE_Pressed, this, &ALACharacter::CanCast);
	PlayerInputComponent->BindAction(TEXT("NonCast"), EInputEvent::IE_Pressed, this, &ALACharacter::NonCast);
}

void ALACharacter::UpDown(float NewAxisValue)
{
	DirectionToMove.X = NewAxisValue;
}

void ALACharacter::LeftRight(float NewAxisValue)
{
	DirectionToMove.Y = NewAxisValue;
}

void ALACharacter::LookUp(float NewAxisValue)
{
	//AddControllerPitchInput(NewAxisValue);
}

void ALACharacter::Turn(float NewAxisValue)
{
	//AddControllerYawInput(NewAxisValue);
}

void ALACharacter::Attack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		LAAnim->PlayAttackMontage();
		LAAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}

}

void ALACharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LAAnim = Cast<ULAAnimInstance>(GetMesh()->GetAnimInstance());

	LAAnim->OnMontageEnded.AddDynamic(this, &ALACharacter::OnAttackMontageEnded);

	LAAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			LAAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	LAAnim->OnAttackHitCheck.AddUObject(this, &ALACharacter::AttackCheck);
}

void ALACharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void ALACharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ALACharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ALACharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Red : FColor::White;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit"));

			FDamageEvent DamegeEvent;
			HitResult.Actor->TakeDamage(50.0f, DamegeEvent, GetController(), this);

		}
	}
}

float ALACharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Taken Damage"));

	if (FinalDamage > 0.0f)
	{
		LAAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		SetCanBeDamaged(false);
		UE_LOG(LogTemp, Warning, TEXT("AI Dead"));
	}

	return FinalDamage;
}

void ALACharacter::Damaged()
{
	CurrentHp = CurrentHp - 20;

	if (CurrentHp <= 0)
	{
		LAAnim->SetDeadAnim();
		SetActorEnableCollision(false);
		SetCanBeDamaged(false);
		UE_LOG(LogTemp, Warning, TEXT("Character Dead"));

	}
}

void ALACharacter::Ability()
{
	LAAnim->SetAbilityAnim();
	CurrentMana -= 50;
}

void ALACharacter::NAbility()
{
	LAAnim->NSetAbilityAnim();
}


void ALACharacter::PlusMana()
{
	if(CurrentMana < MaxMana)
		CurrentMana += 50;
}

void ALACharacter::CanCast()
{
	LAAnim->SetCastAnim();
	CurrentMana -= 50;

}

void ALACharacter::NonCast()
{
	LAAnim->SetNonCastAnim();
}

void ALACharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}
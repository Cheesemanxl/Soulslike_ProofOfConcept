// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	BaseRollDistance = 120.0f;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);

	bIsAttacking = false;
	bIsRolling = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRolling)
	{
		FVector NewLocation = GetActorLocation() + (RollDirection * BaseRollDistance);
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), NewLocation, DeltaTime, 5.0f));
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &APlayerCharacter::Roll);

	PlayerInputComponent->BindAction("LeftLightAttack", IE_Pressed, this, &APlayerCharacter::LeftLightAttack);
	PlayerInputComponent->BindAction("LeftHeavyAttack", IE_Pressed, this, &APlayerCharacter::LeftHeavyAttack);
	PlayerInputComponent->BindAction("RightLightAttack", IE_Pressed, this, &APlayerCharacter::RightLightAttack);
	PlayerInputComponent->BindAction("RightHeavyAttack", IE_Pressed, this, &APlayerCharacter::RightHeavyAttack);
}

bool APlayerCharacter::CanMove(float value)
{
	return	!bIsAttacking &&
			!bIsRolling &&
			value != 0.0f;
}

bool APlayerCharacter::CanAction()
{
	return	!bIsAttacking &&
			!bIsRolling;
}

void APlayerCharacter::MoveForward(float value)
{
	if (CanMove(value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, value);
	}
}

void APlayerCharacter::MoveRight(float value)
{
	if (CanMove(value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, value);
	}
}

void APlayerCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void APlayerCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void APlayerCharacter::LeftLightAttack()
{
	if (CanAction())
	{
		bIsAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("LeftLight"));
		}
	}
}

void APlayerCharacter::LeftHeavyAttack()
{
	if (CanAction())
	{
		bIsAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("LeftHeavy"));
		}
	}
}

void APlayerCharacter::RightLightAttack()
{
	if (CanAction())
	{
		bIsAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("RightLight"));
		}
	}
}

void APlayerCharacter::RightHeavyAttack()
{
	if (CanAction())
	{
		bIsAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("RightHeavy"));
		}
	}
}

void APlayerCharacter::AttackEnd()
{
	bIsAttacking = false;
}

void APlayerCharacter::Roll()
{
	if (CanAction())
	{
		bIsRolling = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			RollDirection = GetActorForwardVector();
			RollCurrentLocation = GetActorLocation();

			AnimInstance->Montage_Play(CombatMontage, 2.0f);
			AnimInstance->Montage_JumpToSection(FName("Roll"));
		}
	}
}

void APlayerCharacter::RollEnd()
{
	bIsRolling = false;
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SOULSLIKE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* CombatMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FVector RollDirection;
	FVector RollCurrentLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rolling")
	float BaseRollDistance;

	bool bIsAttacking;
	bool bIsRolling;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void RollEnd();

	bool CanMove(float value);
	bool CanAction();

	void MoveForward(float value);
	void MoveRight(float value);

	void Turn(float value);
	void LookUp(float value);

	void LeftLightAttack();
	void LeftHeavyAttack();
	void RightLightAttack();
	void RightHeavyAttack();

	void Roll();
};

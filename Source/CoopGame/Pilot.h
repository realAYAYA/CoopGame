// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Pilot.generated.h"

UCLASS()
class COOPGAME_API APilot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APilot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Properties
	float Heath;
	float MaxHeath;
	float Stamina;
	float MaxStamina;
	float StaminaCost;
	float StaminaRestore;

	//Run when created
	
	// Run in BeginPlay
	void LoadWeapon();

	// Tick
	void UpdateHUD();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* Camera;

	class AWeapon* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon>StartWeaponClass;
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName WeaponAttachSocketName;

	class UPilotHUD* HUD;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UPilotHUD> WidgetClass;

	// InputSettings
	void StartFire();
	void StopFire();

	void MoveForward(float val);
	void MoveRight(float val);

	void CrouchBegin();
	void CrouchEnd();

	UCurveFloat* FloatCurve;
	FTimeline ZoomTimeline;
	void ZoomBegin();
	void ZoomEnd();

	UFUNCTION()
	void DoZoom(float FieldOfView);

	void SprintBegin();
	void SprintEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

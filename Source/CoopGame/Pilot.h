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
	
	// Properties
	float Heath;
	float MaxHeath;
	float Stamina;
	float MaxStamina;
	float StaminaCost;
	float StaminaRecharge;

	// Status
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	bool bDied;

	//Run when created
	
	// Run in BeginPlay
	void LoadWeapon();

	// Tick
	void UpdateHUD(); //Run in Tick

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPilotHeathComponent* HealthComponet;

	UPROPERTY(Replicated)
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

	bool IsSprinting;
	FTimerHandle SprintTimerHandle;
	void SprintBegin();
	void SprintEnd();
	void SprintRestore();// Run in Tick
	UFUNCTION()
	void SprintDrain();
	UFUNCTION()
	void RechargeStamina();// Delay CallBack
	
	// Events
	UFUNCTION()
	void OnHealthChanged(UPilotHeathComponent* OwnerHealthComp, float Health, float HeathDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetDeath();
};

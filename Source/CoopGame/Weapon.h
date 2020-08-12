// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class COOPGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Property
	int Ammo;
	int MaxAmmo;
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	float FiringRate;
	int FireModle;
	float TimeBetweenShoots;
	FTimerHandle TimeHandle_TimeBetweenShoots;
	float LastFireTime;
	


	// 
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	TSubclassOf<class ABullet> Bullet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* MuzzleEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* ImpactEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* TracerEffect;// 枪火的延展效果

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	class USoundBase* NoAmmoSound;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	class USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> FireCameraShake;

public:	
	// Called every frame
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnFire();
	void StartFire();
	void StopFire();
};

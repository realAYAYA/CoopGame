// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

USTRUCT()
struct FHitScanTrace// Using to synchronism fire effects
{
	GENERATED_BODY()
public:
	UPROPERTY()
	uint8 BurstCounter;
};

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

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)// 这个宏的作用在于实时监控变量的改变,并绑定回调函数
	FHitScanTrace HitScanTrace;
	UFUNCTION()
	void OnRep_HitScanTrace();

public:	
	int Ammo;
	int MaxAmmo;
	// Called every frame
	UFUNCTION(Server, Reliable, WithValidation)
	void SeverFire();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnFire();
	void StartFire();
	void StopFire();
	void PlayFireEffects();
};

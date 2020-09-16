// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"// 粒子特效头文件
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Bullet.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MuzzleSocketName = "bullet_socket";
	TracerTargetName = "Target";

	Ammo = 31;
	MaxAmmo = 31;
	FiringRate = 600;// Shoots per min

	SetReplicates(true);// Open network function:Make client replicate server's actions
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShoots = 60 / FiringRate;// Second per shoot
}

void AWeapon::SeverFire_Implementation()
{
	OnFire();
}
bool AWeapon::SeverFire_Validate()
{
	return true;
}

void AWeapon::OnRep_HitScanTrace()
{
	if (Bullet != NULL)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		FTransform  SpawnLocation = MeshComponent->GetSocketTransform(MuzzleSocketName);
		GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, ActorSpawnParams);
		PlayFireEffects();
	}
}

void AWeapon::OnFire()
{
	if (GetLocalRole() < ROLE_Authority) {// if you are not server
		SeverFire();// Network:Make sever's client character fire
		//return;
	}

	if (GetLocalRole() == ROLE_Authority) {// Network:Synchronism client's actions while sever fire
		HitScanTrace.BurstCounter++;
	}

	if (Ammo <= 0) {
		PlayFireEffects();
		return;
	}

	// try and fire a projectile
	if (Bullet != NULL)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		FTransform  SpawnLocation = MeshComponent->GetSocketTransform(MuzzleSocketName);
		GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, ActorSpawnParams);

		PlayFireEffects();
		Ammo--;
	}

	LastFireTime = GetWorld()->TimeSeconds;// Note last time shoot
}

void AWeapon::StartFire()
{
	float FirstDelay = FMath::Max(0.0f, LastFireTime + TimeBetweenShoots - GetWorld()->TimeSeconds);
	GetWorldTimerManager().SetTimer(TimeHandle_TimeBetweenShoots, this, &AWeapon::OnFire, TimeBetweenShoots, true, FirstDelay);
}

void AWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimeHandle_TimeBetweenShoots);
}

void AWeapon::PlayFireEffects()
{
	if (Ammo<=0) {
		if (NoAmmoSound) {
			UGameplayStatics::PlaySoundAtLocation(this, NoAmmoSound, GetActorLocation(), 0.2f);
		}
	}
	else {
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.2f);
			//Shake Camera
			APawn* MyOwner = Cast<APawn>(GetOwner());
			if (MyOwner) {
				APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
				if (PC) {
					PC->ClientPlayCameraShake(FireCameraShake);
				}
			}
		}
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const //Rewrite GetLifetimeReplicated() to 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AWeapon, HitScanTrace, COND_SkipOwner);
}


/*射线检测实现
#include "Kismet/GameplayStatics.h"// 可以设置伤害事件
#include "Particles\ParticleSystemComponent.h"// 粒子特效头文件

void NoUse() {
	if (MyOwner) {
		FVector EyeLocation;
		FRotator EyeRotator;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotator);
		FVector TraceEnd = EyeLocation + (EyeRotator.Vector() * 1000.0f);
		FHitResult Hit;
		FCollisionQueryParams QueryParam;
		QueryParam.AddIgnoredActor(MyOwner);// Ignore Player
		QueryParam.AddIgnoredActor(this);// Ignore Weapon
		QueryParam.bTraceComplex = true;// Open ComplexTrace
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParam)) //第四个变量可以是自定义的碰撞通道
		{
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, 20, EyeRotator.Vector(), Hit, MyOwner->GetInstigatorController(), this, DamageType);

			if (ImpactEffect) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
		}
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1);// Draw Line
		if (MuzzleEffect) {
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
		}
		if (TracerEffect) {
			FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
			if (TracerComp) {
				TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
			}
		}
	}
}
*/
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

	Ammo = 20;
	MaxAmmo = 31;
	FiringRate = 600;// Shoots per min
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShoots = 60 / FiringRate;// Second per shoot
}

void AWeapon::OnFire()
{
	if (Ammo <= 0) {
		return;
		if (NoAmmoSound) {
			UGameplayStatics::PlaySoundAtLocation(this, NoAmmoSound, GetActorLocation(), 0.2f);
		}
	}

	// try and fire a projectile
	if (Bullet != NULL)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		FTransform  SpawnLocation = MeshComponent->GetSocketTransform(MuzzleSocketName);
		GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, ActorSpawnParams);
		// try to play the sound if specified
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.2f);
		}
	}
	//Shake Camera
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner) {
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC) {
			PC->ClientPlayCameraShake(FireCameraShake);
		}
	}
	Ammo--;
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
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParam)) {
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

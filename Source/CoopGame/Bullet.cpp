// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Pilot.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetGenerateOverlapEvents(true);//打开碰撞检测
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnHit);//设置事件触发委托
	this->OnDestroyed.AddDynamic(this, &ABullet::DestroyHandler);

	// Set as root component
	RootComponent = Mesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->InitialSpeed = 3000.f;
	MovementComp->MaxSpeed = 3000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = true;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(8.0f);//设置自我销毁时间,在BeginPlay里可以，在构造函数里调用会报错

}

void ABullet::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)//碰撞事件
{
	APilot* Target = Cast<APilot>(OtherActor);
	if (Target!=nullptr) {
		FCollisionQueryParams QuryParams;
		QuryParams.bReturnPhysicalMaterial = true;
		Target->SetDeath();
		this->Destroy();
		if (HitSound) {
			UGameplayStatics::PlaySound2D(this, HitSound);
		}
	}
}

void ABullet::DestroyHandler(AActor* DestroyActor)
{
}

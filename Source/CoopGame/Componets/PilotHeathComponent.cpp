// Fill out your copyright notice in the Description page of Project Settings.


#include "PilotHeathComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPilotHeathComponent::UPilotHeathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	DefaultHeath = 100;
	Health = DefaultHeath;
}


// Called when the game starts
void UPilotHeathComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner) {
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UPilotHeathComponent::TakeAnyDamageHandler);
	}
	
}

void UPilotHeathComponent::TakeAnyDamageHandler(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
		return;
	Health = Health >= Damage ? Health - Damage : 0;
	UE_LOG(LogTemp, Log, TEXT("Health Changed %s"), *FString::SanitizeFloat(Health));

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

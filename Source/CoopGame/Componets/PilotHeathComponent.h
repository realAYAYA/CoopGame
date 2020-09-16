// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PilotHeathComponent.generated.h"

//添加自定义事件,用的时候放到顶上
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, class UPilotHeathComponent*, HealthComp, float, Health, float, HeathDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Coop), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API UPilotHeathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPilotHeathComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeathComponents")
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeathComponents")
	float DefaultHeath;
	
	UFUNCTION()
	void TakeAnyDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;// 自定义事件
};

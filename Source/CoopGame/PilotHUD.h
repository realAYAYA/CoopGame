// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PilotHUD.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API UPilotHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UProgressBar* heathBar;
	UPROPERTY()
	class UProgressBar* staminaBar;
	
	UPROPERTY()
	class UTextBlock* AmmoText;
	UPROPERTY()
	class UTextBlock* MaxAmmoText;//需要头文件
};

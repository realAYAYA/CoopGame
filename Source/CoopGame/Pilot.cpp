// Fill out your copyright notice in the Description page of Project Settings.


#include "Pilot.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"//这个组件用来设置MovementComponent的
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"// Set HUD
#include "Components/ProgressBar.h"// Set ProgressBar
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Weapon.h"
#include "PilotHUD.h"
#include "Engine/Engine.h"// 用来输出到屏幕的工具

// Sets default values
APilot::APilot()
{
	//Initial Properties
	MaxHeath = 1.0f;
	Heath = 0.39f;
	MaxStamina = 1.0f;
	Stamina = 0.2f;

	WeaponAttachSocketName = "WeaponSocket";

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//InitCamera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);

	//Set MovementComponent
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;//设置这个才能启用组件自带的蹲起方法。
}

// Called when the game starts or when spawned
void APilot::BeginPlay()
{
	Super::BeginPlay();
	LoadWeapon();

	FloatCurve = NewObject<UCurveFloat>();
	FloatCurve->FloatCurve.AddKey(0, 90);
	FloatCurve->FloatCurve.AddKey(0.3, 45);

	FOnTimelineFloatStatic TimelineCallback;
	TimelineCallback.BindUFunction(this, TEXT("DoZoom"));
	ZoomTimeline.AddInterpFloat(FloatCurve, TimelineCallback);

	//Inital HUD
	if (WidgetClass) {
		HUD = CreateWidget<UPilotHUD>(GetWorld(), WidgetClass);
		if (HUD) {
			HUD->AddToViewport();
			if (HUD->heathBar)
				HUD->heathBar->SetPercent(Heath / MaxHeath);
			if (HUD->staminaBar)
				HUD->staminaBar->SetPercent(Stamina / MaxStamina);
			if (HUD->AmmoText)
				HUD->AmmoText->SetText(FText::FromString(FString::FromInt(20)));
			if (HUD->MaxAmmoText)
				HUD->MaxAmmoText->SetText(FText::FromString(FString::FromInt(61)));
		}
	}
}

// Called every frame
void APilot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ZoomTimeline.TickTimeline(DeltaTime);// Zoom
}

void APilot::LoadWeapon()
{
	FActorSpawnParameters SpawnParams;// 创建生成参数
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;// 设置参数：有碰撞时仍要生成
	CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(StartWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon) {
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}
}

void APilot::UpdateHUD()
{
	if (HUD) {
		if (HUD->heathBar)
			HUD->heathBar->SetPercent(Heath / MaxHeath);
		if (HUD->staminaBar)
			HUD->staminaBar->SetPercent(Stamina / MaxStamina);
		if (HUD->AmmoText)
			HUD->AmmoText->SetText(FText::FromString(FString::FromInt(15)));
	}
}

void APilot::StartFire()
{
	if (CurrentWeapon) {
		CurrentWeapon->StartFire();
	}
	if (!CurrentWeapon) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Weapon!!!!!!"), false);
}

void APilot::StopFire()
{
	if (CurrentWeapon) {
		CurrentWeapon->StopFire();
	}
	if (!CurrentWeapon) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Weapon~~~~~~"), false);
}


void APilot::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector(), val);
}

void APilot::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector(), val);
}

void APilot::CrouchBegin()
{
	Crouch();
}

void APilot::CrouchEnd()
{
	UnCrouch();
}

void APilot::ZoomBegin()
{
	ZoomTimeline.Play();
}

void APilot::ZoomEnd()
{
	ZoomTimeline.Reverse();
}

void APilot::DoZoom(float FieldOfView)
{
	if (Camera) {
		Camera->SetFieldOfView(FieldOfView);
	}
	if (!Camera) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Camera!!!!!!"), false);
}

void APilot::SprintBegin()
{
	GetCharacterMovement()->MaxWalkSpeed = 2200;
}

void APilot::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

// Called to bind functionality to input
void APilot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APilot::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APilot::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APilot::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APilot::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Roll", this, &APilot::AddControllerRollInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APilot::CrouchBegin);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APilot::CrouchEnd);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APilot::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &APilot::ZoomBegin);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &APilot::ZoomEnd);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APilot::SprintBegin);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APilot::SprintEnd);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APilot::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APilot::StopFire);
}

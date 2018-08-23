// Fill out your copyright notice in the Description page of Project Settings.

#include "MPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MPShooter.h"
#include "MPWeapon.h"
#include "ActorHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMPCharacter::AMPCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	HealthComp = CreateDefaultSubobject<UActorHealthComponent>(TEXT("HealthComponent"));

	ZoomedFOV = 60.f;
	ZoomInterpSpeed = 20.f;

	WeaponAttachSocketName = "WeaponSocket";

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

}

// Called when the game starts or when spawned
void AMPCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;

	if (Role == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<AMPWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon) {
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
		}
	}

	HealthComp->OnHealthChanged.AddDynamic(this, &AMPCharacter::OnHealthChanged);
}

void AMPCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector()*Value);
}

void AMPCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector()*Value);

}

void AMPCharacter::BeginCrouch()
{
	Crouch();
}

void AMPCharacter::EndCrouch()
{
	UnCrouch();
}


void AMPCharacter::BeginZoom()
{
	bWantToZoom = true;
}

void AMPCharacter::EndZoom()
{
	bWantToZoom = false;

}

void AMPCharacter::StartFire()
{
	if (CurrentWeapon) {
		CurrentWeapon->StartFire();
	}
}

void AMPCharacter::EndFire()
{
	if (CurrentWeapon) {
		CurrentWeapon->StopFire();
	}
}

void AMPCharacter::OnHealthChanged(UActorHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.f && !bDied) {
		bDied = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.f);
	}
}

// Called every frame
void AMPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantToZoom ? ZoomedFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void AMPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &AMPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMPCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMPCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMPCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMPCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMPCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMPCharacter::EndZoom);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMPCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMPCharacter::EndFire);


}

FVector AMPCharacter::GetPawnViewLocation() const
{
	if (CameraComp) {
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void AMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//
	DOREPLIFETIME(AMPCharacter, CurrentWeapon);
	DOREPLIFETIME(AMPCharacter, bDied);

}

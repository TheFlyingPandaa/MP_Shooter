// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupSpawnerActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "PowerupActor.h"


// Sets default values
APickupSpawnerActor::APickupSpawnerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComp;
	SphereComp->SetSphereRadius(75.f);


	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);

	SetReplicates(true);

}

// Called when the game starts or when spawned
void APickupSpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority) {
		Respawn();

	}
}



void APickupSpawnerActor::Respawn()
{
	if (!PowerUpClass) {
		UE_LOG(LogTemp, Warning, TEXT("Powerup class is nullptr, set it in %s"), *GetName());
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerUpInstance = GetWorld()->SpawnActor<APowerupActor>(PowerUpClass, GetTransform(), SpawnParams);
}

void APickupSpawnerActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role == ROLE_Authority && PowerUpInstance) {
		PowerUpInstance->ActivatePowerUp(OtherActor);
		PowerUpInstance = nullptr;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APickupSpawnerActor::Respawn, CooldownDuration);
	}
}


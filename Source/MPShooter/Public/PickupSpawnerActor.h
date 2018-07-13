// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnerActor.generated.h"

class USphereComponent;
class UDecalComponent;
class APowerupActor;


UCLASS()
class MPSHOOTER_API APickupSpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupSpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UDecalComponent* DecalComp;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
		TSubclassOf<APowerupActor>PowerUpClass;

	APowerupActor* PowerUpInstance;
	void Respawn();

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
		float CooldownDuration;

	FTimerHandle TimerHandle_RespawnTimer;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


};

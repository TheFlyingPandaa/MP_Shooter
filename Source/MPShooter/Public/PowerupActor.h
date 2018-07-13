// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerupActor.generated.h"

UCLASS()
class MPSHOOTER_API APowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerupActor();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		float PowerupInterval;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		int32 TotalNrOfTicks;

	int32 TicksProcessed = 0;

	FTimerHandle TimerHandle_PowerupTick;

	UFUNCTION()
		void OnTickPowerup();

	UPROPERTY(Replicated = OnRep_PowerupActive)
		bool bIsPowerupActive;

	UFUNCTION()
		void OnRep_PowerupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnPowerupStateChanged(bool bNewIsActive);




public:
	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnActivated(AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnPowerUpTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUp")
		void OnExpired();

	void ActivatePowerUp(AActor* OtherActor);
};

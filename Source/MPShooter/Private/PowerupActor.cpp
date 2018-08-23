// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerupActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APowerupActor::APowerupActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PowerupInterval = 0.f;
	TotalNrOfTicks = 0;

	bIsPowerupActive = false;

	SetReplicates(true);
}


void APowerupActor::OnTickPowerup()
{
	TicksProcessed++;

	OnPowerUpTicked();

	if (TicksProcessed >= TotalNrOfTicks) {
		OnExpired();

		bIsPowerupActive = false;
		OnRep_PowerupActive();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void APowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);
}

void APowerupActor::ActivatePowerUp(AActor* OtherActor)
{
	OnActivated(OtherActor);

	bIsPowerupActive = true;
	OnRep_PowerupActive();

	if (PowerupInterval > 0.f) {
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &APowerupActor::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void APowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//
	DOREPLIFETIME(APowerupActor, bIsPowerupActive);

}


// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UActorHealthComponent::UActorHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	DefaultHealth = 100;

	SetIsReplicated(true);

}


// Called when the game starts
void UActorHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			Owner->OnTakeAnyDamage.AddDynamic(this, &UActorHealthComponent::HandleTakeAnyDamage);
		}

	}
	Health = DefaultHealth;
}

void UActorHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{


	if (Damage <= 0.f) {
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);

	UE_LOG(LogTemp, Warning, TEXT("Health is %f"), Health);

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UActorHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.f || Health <= 0.f) {
		return;
	}

	Health = FMath::Clamp(Health + HealAmount, 0.f, DefaultHealth);

	OnHealthChanged.Broadcast(this, Health, -HealAmount, nullptr, nullptr, nullptr);
	UE_LOG(LogTemp, Warning, TEXT("Health is %f +"), Health);

}

void UActorHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//
	DOREPLIFETIME(UActorHealthComponent, Health);

}



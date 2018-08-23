// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveActor.generated.h"

class UActorHealthComponent;
class UStaticMeshComponent;
class UParticleSystem;
class URadialForceComponent;

UCLASS()
class MPSHOOTER_API AExplosiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UActorHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		URadialForceComponent* RadialForceComp;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
		UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
		UMaterialInterface* ExplodedMaterial;


	UFUNCTION()
		void OnHealthChanged(UActorHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(ReplicatedUsing = OnRep_Exploded, BlueprintReadOnly, Category = "Setup")
		bool bExploded = false;

	UFUNCTION()
		void OnRep_Exploded();

	UPROPERTY(EditDefaultsOnly, Category = "FX")
		float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		float ExplosionRadius = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		float ExplosionDamage = 100;

};

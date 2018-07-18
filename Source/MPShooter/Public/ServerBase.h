// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ServerBase.generated.h"

UCLASS()
class MPSHOOTER_API AServerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AServerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable, Category = "Server")
		void ServerTravel(const FString & InURL, bool bAbsolute, bool bShouldSkipGameNotify);


	
};

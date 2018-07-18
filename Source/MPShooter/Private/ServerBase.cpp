// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerBase.h"
#include "Engine/World.h"

// Sets default values
AServerBase::AServerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

}

// Called when the game starts or when spawned
void AServerBase::BeginPlay()
{
	Super::BeginPlay();
	
}



void AServerBase::ServerTravel(const FString & InURL, bool bAbsolute, bool bShouldSkipGameNotify)
{
	auto World = GetWorld();
	if (World)
	{
		World->ServerTravel(InURL, bAbsolute, bShouldSkipGameNotify);
	}

}

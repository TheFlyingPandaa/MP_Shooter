// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SteamFriendsFunctionLibrary.h"
#include "OnlineSubsystemUtils.h"
#include "OnlinePresenceInterface.h"

#include "CoreMinimal.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "GetSteamFriendsCallProxyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetSteamFriendsDelegate, const TArray<FSteamFriend>&, Result);


UCLASS()
class MPSHOOTER_API UGetSteamFriendsCallProxyBase : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

private:
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;
	UObject* WorldContextObject;
	FOnReadFriendsListComplete OnReadFriendsCompleteDelegate;
public:
	UPROPERTY(BlueprintAssignable)
		FGetSteamFriendsDelegate OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetSteamFriendsDelegate OnFailure;


public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"), Category = "SteamFriends")
	static UGetSteamFriendsCallProxyBase* GetSteamFriends(UObject* InWorldContextObject, class APlayerController* InPlayerController);

	void OnGetSteamFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	virtual void Activate() override;
	
	
};

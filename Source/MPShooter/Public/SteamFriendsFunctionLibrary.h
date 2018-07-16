// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "OnlinePresenceInterface.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SteamFriendsFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FSteamFriend
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Steam|Friends")
		FString DisplayName;

	UPROPERTY(BlueprintReadOnly, Category = "Steam|Friends")
		FString RealName;

	UPROPERTY(BlueprintReadOnly, Category = "Steam|Friends")
		FString StatusString;

	UPROPERTY(BlueprintReadOnly, Category = "Steam|Friends")
		bool bIsOnline;

	UPROPERTY(BlueprintReadOnly, Category = "Steam|Friends")
		bool bIsPlaying;

	UPROPERTY(BlueprintReadOnly, Category = "Steam|Friends")
		bool bIsPlayingSameGame;

	UPROPERTY(BlueprintReadOnly, Category = "Steam|Friends")
		bool bIsJoinable;

	UPROPERTY(BlueprintReadOnly, Category = "Steam|Friends")
		bool bHasVoiceSupport;

	EOnlinePresenceState::Type PresenceState;
	TSharedPtr<const FUniqueNetId> UniqueNetID;
};

UENUM()
enum class ESFAsyncResultSwitch:uint8
{
	OnSuccess,
	AsyncLoading,
	OnFailure
};

UENUM(Blueprintable)
enum class ESFAvatarSize :uint8
{
	SF_SMALL=1 UMETA(DisplayName="Small Avatar"),
	SF_MEDIUM = 2 UMETA(DisplayName = "Medium Avatar"),
	SF_LARGE = 3 UMETA(DisplayName = "Large Avatar"),
};


class UTexture2D;

UCLASS()
class MPSHOOTER_API USteamFriendsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:

	UFUNCTION(BlueprintCallable, Category = "Steam|Friends", meta = (ExpandEnumAsExecs = "Result"))
		static class UTexture2D* GetSteamFriendAvatar(const FSteamFriend SteamFriend, ESFAsyncResultSwitch& Result, ESFAvatarSize AvatarSize = ESFAvatarSize::SF_MEDIUM);

	UFUNCTION(BlueprintCallable, Category = "Steam|Friends", meta = (ExpandEnumAsExecs = "Result"))
		static void GetSteamFriendGame(const FSteamFriend SteamFriend, ESFAsyncResultSwitch& Result, FString& GameName);

	
};

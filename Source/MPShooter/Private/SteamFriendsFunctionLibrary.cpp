// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamFriendsFunctionLibrary.h"
#include "MPShooter.h"
#include "Engine/Texture2D.h"

#include "steam/steam_api.h"

UTexture2D* USteamFriendsFunctionLibrary::GetSteamFriendAvatar(const FSteamFriend SteamFriend, ESFAsyncResultSwitch& Result, ESFAvatarSize AvatarSize /*= ESFAvatarSize::SF_MEDIUM*/)
{
	if (!SteamFriend.UniqueNetID.IsValid())
	{
		Result = ESFAsyncResultSwitch::OnFailure;
		return nullptr;
	}

	uint32 Height = 0;
	uint32 Width = 0;

	if (SteamAPI_Init())
	{
		uint64 ID = *((uint64*)SteamFriend.UniqueNetID->GetBytes());

		int AvatarSizeNumber = 0;

		switch (AvatarSize)
		{
		case ESFAvatarSize::SF_SMALL:
			AvatarSizeNumber = SteamFriends()->GetSmallFriendAvatar(ID);
			break;
		case ESFAvatarSize::SF_MEDIUM:
			AvatarSizeNumber = SteamFriends()->GetMediumFriendAvatar(ID);
			break;
		case ESFAvatarSize::SF_LARGE:
			AvatarSizeNumber = SteamFriends()->GetLargeFriendAvatar(ID);
			break;
		default:
			break;
		}

		if (AvatarSizeNumber==-1)
		{
			Result = ESFAsyncResultSwitch::AsyncLoading;
			return nullptr;
		}

		SteamUtils()->GetImageSize(AvatarSizeNumber, &Width, &Height);

		if (Height > 0 && Height > 0) {
			uint8* AvatarRGBA = new uint8[Height*Width * 4];
			SteamUtils()->GetImageRGBA(AvatarSizeNumber, AvatarRGBA, Height*Width * 4 * sizeof(char));

			UTexture2D* AvatarTexture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);

			uint8* MipData = (uint8*)AvatarTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(MipData, (void*)AvatarRGBA, Height*Width * 4);
			AvatarTexture->PlatformData->Mips[0].BulkData.Unlock();

			delete[] AvatarRGBA;

			AvatarTexture->PlatformData->NumSlices = 1;
			AvatarTexture->NeverStream = true;
			AvatarTexture->UpdateResource();

			Result = ESFAsyncResultSwitch::OnSuccess;

			return AvatarTexture;
		}

	}
	Result = ESFAsyncResultSwitch::OnFailure;
	return nullptr;
}

void USteamFriendsFunctionLibrary::GetSteamFriendGame(const FSteamFriend SteamFriend, ESFAsyncResultSwitch& Result, FString& GameName)
{
	if (!SteamFriend.UniqueNetID.IsValid())
	{
		Result = ESFAsyncResultSwitch::OnFailure;
		return ;
	}

	if (SteamAPI_Init()) {
		uint64 ID = *((uint64*)SteamFriend.UniqueNetID->GetBytes());

		FriendGameInfo_t GameInfo;
		bool bIsInGame = SteamFriends()->GetFriendGamePlayed(ID, &GameInfo);

		if (bIsInGame && GameInfo.m_gameID.IsValid())
		{
			char NameBuffer[512];
			int NameLength = SteamAppList()->GetAppName(GameInfo.m_gameID.AppID(), NameBuffer, 512);

			if (NameLength!=-1)
			{
				GameName = FString(UTF8_TO_TCHAR(NameBuffer));
			}

			Result = ESFAsyncResultSwitch::OnSuccess;
			return;
		}
	}

	Result = ESFAsyncResultSwitch::OnFailure;
}

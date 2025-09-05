// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HWGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HWPlayerController.h"

void AHWGameStateBase::MulticastRPCChatLoginMessage_Implementation(const FString& Name)
{
	if (HasAuthority() == false)
	{
		if (HasAuthority() == false)
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (IsValid(PC) == true)
			{
				AHWPlayerController* HWPC = Cast<AHWPlayerController>(PC);
				if (IsValid(HWPC) == true)
				{
					FString NotificationString = Name + TEXT(" has joined the game.");
					HWPC->PrintChatMessage(NotificationString);
				}
			}
		}
	}
}
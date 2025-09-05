// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HWPlayerController.h"
#include "UI/HWChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/HWGameModeBase.h"
#include "Player/HWPlayerState.h"
#include "Net/UnrealNetwork.h"

AHWPlayerController::AHWPlayerController()
{
	bReplicates = true;
}

void AHWPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	if(IsValid(ChatInputClass) == true)
	{
		ChatInputWidget = CreateWidget<UHWChatInput>(this, ChatInputClass);
		if(IsValid(ChatInputWidget) == true)
		{
			ChatInputWidget->AddToViewport();
		}
	}
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}

}
void AHWPlayerController::SetChatMessage(const FString& InMessage)
{
	ChatMessage = InMessage;

	if (IsLocalController() == true)
	{
		AHWPlayerState* HWPS = GetPlayerState<AHWPlayerState>();
		if (IsValid(HWPS) == true)
		{
			FString CombinedMessageString = HWPS->PlayerName + TEXT(": ") + InMessage;

			ServerRPCPrintChatMessage(CombinedMessageString);
		}
	}
}

void AHWPlayerController::SetChatMessageString(const FString& InMessage)
{
	ChatMessage = InMessage;

	//PrintChatMessageString(InChatMessageString);
	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessage(InMessage);
	}
}

void AHWPlayerController::PrintChatMessage(const FString& InMessage)
{
	UKismetSystemLibrary::PrintString(this, InMessage, true, true, FLinearColor::Green, 5.f);
}

void AHWPlayerController::ClientRPCPrintChatMessage_Implementation(const FString& InMessage)
{
	PrintChatMessage(InMessage);
}

void AHWPlayerController::ServerRPCPrintChatMessage_Implementation(const FString& InMessage)
{
	/*for(TActorIterator<AHWPlayerController> It(GetWorld()); It; ++It)
	{
		AHWPlayerController* HWPC = *It;
		if(IsValid(HWPC) == true)
		{
			HWPC->ClientRPCPrintChatMessage(InMessage);
		}
	}*/

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AHWGameModeBase* HWGM = Cast<AHWGameModeBase>(GM);
		if (IsValid(HWGM) == true)
		{
			HWGM->PrintChatMessage(this, InMessage);
		}
	}
}

void AHWPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NotificationText);
}
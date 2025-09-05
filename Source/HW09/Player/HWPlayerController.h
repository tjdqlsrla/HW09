// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HWPlayerController.generated.h"

class UHWChatInput;
class UUserWidget;

/**
 * 
 */
UCLASS()
class HW09_API AHWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AHWPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	void SetChatMessage(const FString& InMessage);

	void PrintChatMessage(const FString& InMessage);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessage(const FString& InMessage);
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessage(const FString& InMessage);

	void SetChatMessageString(const FString& InMessage);
protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHWChatInput> ChatInputClass;
	UPROPERTY()
	TObjectPtr<UHWChatInput> ChatInputWidget;

	FString ChatMessage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;


};

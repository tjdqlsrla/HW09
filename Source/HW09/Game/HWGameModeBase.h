// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HWGameModeBase.generated.h"

class AHWPlayerController;

/**
 * 
 */
UCLASS()
class HW09_API AHWGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenSecretNumber();

	bool IsGuessNumber(const FString& InNumber);

	FString JudgeNumber(const FString& InSecretNumber, FString& InGuessNumber);

	void PrintChatMessage(AHWPlayerController* InChatPlayerController ,const FString& InMessage);
	
	void InCreaseTryCount(AHWPlayerController* InChatPlayerController);

	void ResetGame();

	void JudgeGame(AHWPlayerController* InChatPlayerController, int StrikeCount);

protected:
	FString SecretNumber;

	TArray<TObjectPtr<AHWPlayerController>> PlayerControllers;
	
};

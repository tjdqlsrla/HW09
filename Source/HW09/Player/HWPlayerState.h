// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HWPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API AHWPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:

	AHWPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetPlayerInfo();

public:
	UPROPERTY(Replicated)
	FString PlayerName;

	UPROPERTY(Replicated)
	int32 CurrentTryCount;

	UPROPERTY(Replicated)
	int32 MaxTryCount;
};

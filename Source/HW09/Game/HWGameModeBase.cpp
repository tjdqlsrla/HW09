// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HWGameModeBase.h"
#include "Game/HWGameStateBase.h"
#include "Player/HWPlayerController.h"
#include "EngineUtils.h"
#include "Player/HWPlayerState.h"

void AHWGameModeBase::OnPostLogin(AController* NewPlayer)
{
	/*Super::OnPostLogin(NewPlayer);

	AHWGameStateBase* GS = GetGameState<AHWGameStateBase>();
	if (IsValid(GS) == true)
	{
		GS->MulticastChatLoginMessage(TEXT("ABC"));
	}

	AHWPlayerController* HWPC = Cast<AHWPlayerController>(NewPlayer);
	if (IsValid(HWPC) == true)
	{
		PlayerControllers.Add(HWPC);
	}*/

	AHWPlayerController* HWPC = Cast<AHWPlayerController>(NewPlayer);
	if (IsValid(HWPC) == true)
	{
		PlayerControllers.Add(HWPC);

		AHWPlayerState* HWPS = HWPC->GetPlayerState<AHWPlayerState>();
		if (IsValid(HWPS) == true)
		{
			HWPS->PlayerName = TEXT("Player") + FString::FromInt(PlayerControllers.Num());
		}

		AHWGameStateBase* HWGS = GetGameState<AHWGameStateBase>();
		if (IsValid(HWGS) == true)
		{
			HWGS->MulticastRPCChatLoginMessage(HWPS->PlayerName);
		}
	}
}

void AHWGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumber = GenSecretNumber();
}

void AHWGameModeBase::PrintChatMessage(AHWPlayerController* InChatPlayerController, const FString& InMessage)
{
	int Index = InMessage.Len() - 3;
	FString GuessNumber = InMessage.RightChop(Index);
	if (IsGuessNumber(GuessNumber) == true)
	{
		FString JudgeResult = JudgeNumber(SecretNumber, GuessNumber);

		InCreaseTryCount(InChatPlayerController);
		for (TActorIterator<AHWPlayerController> It(GetWorld()); It; ++It)
		{
			AHWPlayerController* HWPC = *It;
			if (IsValid(HWPC) == true)
			{
				FString CombinedMessage = InMessage + TEXT(" -> ") + JudgeResult;
				HWPC->ClientRPCPrintChatMessage(CombinedMessage);

				int32 StrikeCount = FCString::Atoi(*JudgeResult.Left(1));
				JudgeGame(HWPC, StrikeCount);

			}
		}
	}
	else
	{
		for (TActorIterator<AHWPlayerController> It(GetWorld()); It; ++It)
		{
			AHWPlayerController* HWPC = *It;
			if (IsValid(HWPC) == true)
			{
				HWPC->ClientRPCPrintChatMessage(InMessage);
			}
		}
	}
}

FString AHWGameModeBase::GenSecretNumber()
{
	TArray<int32> Num;
	for (int32 i = 1; i <= 9; ++i)
	{
		Num.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Num = Num.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Num.Num() - 1);
		Result.Append(FString::FromInt(Num[Index]));
		Num.RemoveAt(Index);
	}

	return Result;
}

bool AHWGameModeBase::IsGuessNumber(const FString& InNumber)
{
	bool bCanPlay = false;

	do {

		if (InNumber.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumber)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString AHWGameModeBase::JudgeNumber(const FString& InSecretNumber, FString& InGuessNumber)
{
	int32 Strike = 0, Ball = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumber[i] == InGuessNumber[i])
		{
			Strike++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumber[i]);
			if (InSecretNumber.Contains(PlayerGuessChar))
			{
				Ball++;
			}
		}
	}

	if (Strike == 0 && Ball == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), Strike, Ball);
}

void AHWGameModeBase::InCreaseTryCount(AHWPlayerController* InChatPlayerController)
{
	AHWPlayerState* HWPS = InChatPlayerController->GetPlayerState<AHWPlayerState>();
	if (IsValid(HWPS) == true)
	{
		HWPS->CurrentTryCount++;
	}
}

void AHWGameModeBase::ResetGame()
{
	SecretNumber = GenSecretNumber();

	for (const auto& HWPC : PlayerControllers)
	{
		AHWPlayerState* HWPS = HWPC->GetPlayerState<AHWPlayerState>();
		if (IsValid(HWPS) == true)
		{
			HWPS->CurrentTryCount = 0;
		}
	}
}

void AHWGameModeBase::JudgeGame(AHWPlayerController* InChatPlayerController, int StrikeCount)
{
	if (3 == StrikeCount)
	{
		AHWPlayerState* HWPS = InChatPlayerController->GetPlayerState<AHWPlayerState>();
		for (const auto& CXPlayerController : PlayerControllers)
		{
			if (IsValid(HWPS) == true)
			{
				FString CombinedMessageString = HWPS->PlayerName + TEXT(" has won the game.");
				CXPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CXPlayerController : PlayerControllers)
		{
			AHWPlayerState* HWPS = CXPlayerController->GetPlayerState<AHWPlayerState>();
			if (IsValid(HWPS) == true)
			{
				if (HWPS->CurrentTryCount < HWPS->MaxTryCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& HWPC : PlayerControllers)
			{
				HWPC->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}

}
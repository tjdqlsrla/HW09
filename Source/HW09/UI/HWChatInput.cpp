// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HWChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/HWPlayerController.h"

void UHWChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ChatInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnTextCommitted) == false)
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnTextCommitted);
	}
}

void UHWChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (ChatInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnTextCommitted) == true)
	{
		ChatInputBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnTextCommitted);
	}
}

void UHWChatInput::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	if(InCommitType == ETextCommit::OnEnter)
	{
		APlayerController* PC = GetOwningPlayer();
		if(IsValid(PC) == true)
		{
			AHWPlayerController* HWPC = Cast<AHWPlayerController>(PC);
			if(IsValid(HWPC) == true)
			{
				HWPC->SetChatMessage(InText.ToString());
			}
		}
	}
}
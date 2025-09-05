// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HWChatInput.generated.h"


class UEditableTextBox;
/**
 * 
 */
UCLASS()
class HW09_API UHWChatInput : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitType);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatInputBox;

};

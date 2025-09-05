// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HWPawn.h"
#include "HW09.h"

void AHWPawn::BeginPlay()
{
	Super::BeginPlay();
	FString NetRoleString = HW09FuctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *HW09FuctionLibrary::GetNetModeString(this), *NetRoleString);
	HW09FuctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void AHWPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);	
}
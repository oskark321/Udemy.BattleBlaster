// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenCountTowers.h"

#include "Components/TextBlock.h"

void UScreenCountTowers::SetMessageText(FString(Message), FString(CountTowers))
{
	MessageTextBlock->SetText(FText::FromString(Message));
	CountTowersTextBlock->SetText(FText::FromString(CountTowers));
}

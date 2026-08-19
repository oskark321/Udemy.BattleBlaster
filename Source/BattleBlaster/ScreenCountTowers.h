// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenCountTowers.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API UScreenCountTowers : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CountTowersTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MessageTextBlock;

	void SetMessageText(FString(Message), FString(CountTowers));
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenMessage.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API UScreenMessage : public UUserWidget
{
	GENERATED_BODY()
	

public:

	//S4 L137
	UPROPERTY(EditAnywhere, meta = (BindWidget)) //meta = (BindWidget) jest bardzo ważne ponieważ chcemy powiązać tą zmienną z widgetem który mamy w projekcie WBP
	class UTextBlock* MessageTextBlock;
	//zmienna typu TextBlock - zosta on też utworzony w UE klasie WBP_ScreenMessage w panelu, musi być ta sama nazwa!

	void SetMessageText(FString Message);
};

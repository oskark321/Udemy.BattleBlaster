// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

#include "Components/TextBlock.h"

void UScreenMessage::SetMessageText(FString Message)
{
	//S4 L137
	MessageTextBlock->SetText(FText::FromString(Message));
	//ustawiamy teks z w naszym MessageTextBlock (musimy przekonwertować na FText z FString)
}

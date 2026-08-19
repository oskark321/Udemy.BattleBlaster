// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"

#include "Kismet/GameplayStatics.h"


void UBattleBlasterGameInstance::ChangeLevel(int32 Index)
{
	//4 L134
	if (Index > 0 && Index <= LastLevelIndex)
	{
		CurrentLevelIndex = Index;

		FString LevelNameString = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);
		//konstru³ujemy FString przy pomocy zmiennych innego typu aby np wyœwietliæ jaki lvl  

		UGameplayStatics::OpenLevel(GetWorld(), *LevelNameString);
	}

}

void UBattleBlasterGameInstance::LoadNextLevel()
//otwórz kolejny poziom
{
	//S4 L134
	if (CurrentLevelIndex < LastLevelIndex)
	{
		ChangeLevel(CurrentLevelIndex + 1);
	}
	else
	{
		RestartGame();
	}
}

void UBattleBlasterGameInstance::RestartCurrentLevel()
//restart obecny lvl
{
	//S4 L134
	ChangeLevel(CurrentLevelIndex);
}

void UBattleBlasterGameInstance::RestartGame()
//restart gry
{
	//S4 L134
	ChangeLevel(1);
}

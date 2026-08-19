// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	class ATank* TankInGameModeClass;

	int32 TowerCount;
	//zmienna gdzie przechowywujemy ile jest wrogich wierz - gdy spadnie do 0 wygrywamy gre

	void ActorDied(AActor* DeadActor);

	UPROPERTY(EditAnywhere)
	float GameOverDelay = 3.0f;

	void OnGameOverTimerTimeout();

	bool IsVictory = false;

	//S4 L135
	UPROPERTY(EditAnywhere)
	int32 CountdownDelay = 3;

	int32 CountdownSeconds;

	FTimerHandle CountdownTimerHandle;

	void OnCountdownTimerTimeout();
	//funkcja zwrotna która będzie działać po upływie czsu Timera

	//S4 L138
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UScreenMessage> ScreenMessageClass;
	//zmienna TSubclassOf którą połączymy w edytorze z naszym BP UserWidget czyli WBP_ScreenMessage

	class UScreenMessage* ScreenMessageWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UScreenCountTowers> ScreenCountTowersClass;

	class UScreenCountTowers* ScreenCountTowersWidget;

};

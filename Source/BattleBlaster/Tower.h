// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ATower : public ABasePawn
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class ATank* TankInTowerClass;

	UPROPERTY(EditAnywhere)
	float FireRange = 700.0f;

	//S4 L119
	void CheckFireCondition();

	bool IsInFireRange();

	UPROPERTY(EditAnywhere)
	float FireRate = 2.0f;

	//S4 L128
	void HandleDestruction();
};

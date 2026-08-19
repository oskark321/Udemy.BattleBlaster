// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"


UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//S4 L101
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	//S4 L104
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext; //a tutaj zmienna aby dodaæ nasz IMC_Default w Edytorze

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveInputAction;

	//S4 L110
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* TurnInputAction;

	//S4 L118
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* FireInputAction;

	//S4 L108
	void MoveInput(const struct FInputActionValue& Value);
	//tutaj s³owo struct dzia³a jak w klasach s³owo class, ale ¿e to jest struktura to struct

	//S4 L110
	void TurnInput(const struct FInputActionValue& Value);
	//tutaj s³owo struct dzia³a jak w klasach s³owo class, ale ¿e to jest struktura to struct

	//S4 L109
	UPROPERTY(EditAnywhere)
	float TankSpeed = 600.0f;

	//S4 L110
	UPROPERTY(EditAnywhere)
	float TurnSpeed = 100.0f;

	//S4 L128
	void HandleDestruction();
		
	//S4 L129
	void SetPlayerEnabled(bool Enabled);
	APlayerController* PlayerController;
	bool IsAlive = true;
	bool GetIsAlive();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	//S4 L100
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TurretMesh;

	//S4 L118
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	//S4 L112
	void RotateTurret(FVector LookAtTarget);

	//S4 L118
	void FirePawn();

	//S4 L120
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;
	//zmienna UClass podklasa l¹cz¹ca c++ z edytorem gdzie mamy BP_Projectile <tutaj nazwa klasy z którj pochodzi BP> i nazwa w³asna 

	//S4 L128
	void HandleDestruction();

	//S4 L139
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* DeathParticles;
	//zmienna gdzie w edytorze przypiszemy odpowieni efekt niagara

	//S4 L140
	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;
	//zmienna gdzie przypiszujemy dzwiêk w edytorze

	UPROPERTY(EditAnywhere)
	float VolumeSound = 5.0f;

	//S4 L141
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;
	//subklasa dla bazy wstrz¹sów kamery i zmienna gdzie w edytorze przypiszemy do niej BP - bez do³¹czania ¿adnych nag³ówków
};

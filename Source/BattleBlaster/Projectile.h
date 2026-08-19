// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BATTLEBLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	//S4 L121
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComp;
	//specjalny komponent do poruszania pociskiem, zazwyczaj w³aœciciela lecz mo¿ê poruszaæ te¿ inny komponent
	//jeœli komponent jest komponentem aktywnym to nazwa klasy bêdzie mia³a prefix U

	//S4 L123
	UFUNCTION() //musi to byæ UFunkcja poniewa¿ bêdziemy u¿ywaæ jej w systemie delegatów - dlatego musimy zarejestrowaæ j¹ w systemie UE
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	//funkcja gdzie bêd¹ wydarzenia po uderzeniu plus zmienne których wymaga delegat OnComponentHit

	//S4 L124
	UPROPERTY(EditAnywhere)
	float Damage = 25.0f;

	//S4 L139
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* TrailParticles;
	//komponent niagara który mo¿emy do³¹czyæ do BP_Projectile jako element jego budowy

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HitParticles;
	//zmienna gdzie w edytorze przypiszemy odpowieni efekt niagara

	//S4 L140
	UPROPERTY(EditAnywhere)
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;
	//zmienna gdzei przypiszujemy dzwiêk w edytorze

	UPROPERTY(EditAnywhere)
	float VolumeSound = 5.0f;

	//S4 L141
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;
	//subklasa dla bazy wstrz¹sów kamery i zmienna gdzie w edytorze przypiszemy do niej BP - bez do³¹czania ¿adnych nag³ówków
};

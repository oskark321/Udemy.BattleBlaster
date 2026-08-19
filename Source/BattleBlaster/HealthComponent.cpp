// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	//heatlt to aktualne zdrowie pi¹ka a maxhealth to zdrowie maksymlne - mo¿emy modyfikowaæ
	
	//S4 L126
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);
	//delegat przyjmij dowolne obra¿enia i dodajemy funkcjê z odpowiednimi parametrami zmiennymi poznajemy je pooprzez CTRL+OnTakeAnyDamage potem CTRL+FTakeAnyDamageSignature i w nawiasach
	//po OnTakeAnyDamage mamy typ i nazwê parametrów zestaw

	//S4 L127
	BattleBlasterGameMode = Cast<ABattleBlasterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	//pobieramy GameMode aby wywolaæ funkcjê ActorDied z klasy ABattleBlasterGameMode

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
//(aktor który zosta³ uszkodzony, rozmiar wyrz¹dzonych szkód - pobiera to z funkcji ApplyDamage w AProjectile, rodzaj wyrz¹dzonych szkód, kontroler, aktora który spowodowa³ szkody)
{
	//S4 L126
	if (Damage > 0.0f)
	{
		Health -= Damage;
		if (Health <= 0.0f)
		{
			//S4 L127
			if (BattleBlasterGameMode)
			{
				BattleBlasterGameMode->ActorDied(DamagedActor);
			}
		}
	}
}

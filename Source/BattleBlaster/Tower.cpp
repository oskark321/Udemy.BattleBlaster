// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "Tank.h"


void ATower::BeginPlay()
{
	Super::BeginPlay();

	//S4 L119
	FTimerHandle FireTimerHandle;
	//Unikalny uchwyt, który mo¿na wykorzystaæ do rozró¿niania timerów posiadaj¹cych identycznych delegatów.
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
	//zwróci obiekt menadzera TIMERÓW który jest odpowiedzialny za tworzenie i ustawianie TIMERÓW.posiada funkcjê ustaw TIMER-regulator czasowy(uchwyt TIMERA - tworzymy sami, s³owo kluczowe
	//wskazuj¹ce na t¹ klasê - funkcja chce wiedzieæ która instancja jakiej klasy wykonuje wywo³anie, adres funkcji któr¹ chcem wywo³aæ, co jaki czas ma siê wywo³ywaæ funkcja, 
	//true wskazuje na pêtle

}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//S4 L117
	/*
	if (TankInTowerClass)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), TankInTowerClass->GetActorLocation());
		//oblicza nam dystans miêdzy dwoma punktami/wektorami i zwraca float
		if (DistanceToTank <= FireRange)
		//sprawdzamy dystans do czo³gu z zasiêgiem strza³u aby turety obraca³y siê jak podjedziemy blizej
		{
			RotateTurret(TankInTowerClass->GetActorLocation());
			//Przekazujemy funkcji obrót wierzyczki (lokalizacjê czo³gu) - bo obraca siê wierzyczka wierzy
			
		}
	}
	*/
	
	//S4 L119
	if (IsInFireRange())
	{
		RotateTurret(TankInTowerClass->GetActorLocation());
	}
}

void ATower::CheckFireCondition()
//funkcja sprawdza czy jesteœmy w stanie odpaliæ czy nie
{
	//S4 L119
	if (TankInTowerClass && TankInTowerClass->GetIsAlive() && IsInFireRange())
	{
		
		FirePawn();
	}
}

bool ATower::IsInFireRange()
{
	//S4 L119
	bool Result = false;
	if (TankInTowerClass)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), TankInTowerClass->GetActorLocation());
		//oblicza nam dystans miêdzy dwoma punktami/wektorami i zwraca float
		//if (DistanceToTank <= FireRange)
			//sprawdzamy dystans do czo³gu z zasiêgiem strza³u aby turety obraca³y siê jak podjedziemy blizej
		//{
		//	Result = true;
		//}
		Result = (DistanceToTank <= FireRange);
		//to samo co if na górze
	}
	return Result;
}

void ATower::HandleDestruction()
{
	//S4 L128
	Super::HandleDestruction();
	//wykonujemy Super::nazwa funkcji poniewa¿ g³owna funkcja jest w klasie ABasePawn, w ATank i ATower mamy taka sam¹ funkcjê dla tego musimy wywo³aæ super na samym pocz¹tku funkcji!
	//mo¿emy tworzyæ te same funkcje tylko w jednostce nadrzêdnej i podrzêdnej czyli w klasie rodzic i dziecko. Oryginalna funkcja jest w klasie ABasePawn poniewarz to jest rodziæ i
	//czêœæ rzeczy jak dzwiêk czy ob³ok po zniszczeni bêdzie dzia³a siê to samo w ATamk i ATower czyli w klasach dzieci gdzie mamy super i bêd¹ sie ju¿ wykonywac rzeczy typowo dla danej
	//klasy np ATank - po zniszczenu czo³g bêdzie niewidoczny aby dalej kamera dzia³a i wyœwietli siê komunikat ¿e przegrywasz grê, ATower - po zniszczemi niszczymy na prawdê wierze.
	//Najpierw jest wywo³ywana funkcja nadrzêdna bez Super a potem funkcja podrzêdna która zawiera s³owo Super

	//UE_LOG(LogTemp, Warning, TEXT("Tower HandleDestruction"));
	Destroy();
}

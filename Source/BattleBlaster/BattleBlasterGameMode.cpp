// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"

#include "Tank.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "BattleBlasterGameInstance.h"
#include "ScreenMessage.h"
#include "ScreenCountTowers.h"

void ABattleBlasterGameMode::BeginPlay()
{

	Super::BeginPlay();

	//S4 L114
	TArray<AActor*> Towers;
	//tablica która wype³ni siê wskaŸnikami aktorów

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
//funkcja ta znajduje wszystkich aktorów w œwiecie okreœlonej klasy(ten œwiat, podklasa której obiektem jest UClass - reprezentuje klasê której chcemy obiekty liczyc StaticClass() - jest
//to funkcja która zwróci obiekt UClass którego szukamy,OutActors wyjœciowy aktor/parametr jest to tablica wskaŸników aktorów nazywanych OutActors wyjœciowymi aktorami - przekazujemy to do
//funkcji która wype³ni j¹ wszystkimi aktorami których szukamy - jest to Out argument wiêc zosatnie on wype³niony wszystkimi aktorami po wywo³aniu tej funkcji)
//funkcja spojrzy na œwwiat gry, i znajdzie wszystkich aktorów z ATower Class, umieœci ich wszystkich w tej tablicy jako wskaŸnik do AActor

	TowerCount = Towers.Num();
	//Nun to funkcja tablicy która zwraca liczbê elementów tej tablicy czyli praktycznie ile wierz mamy

	UE_LOG(LogTemp, Warning, TEXT("We have %d enemys"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//zwraca pionek kontrolera gracza o podanym indeksie gracza 0 bo single i gracz ma indeks 0 czyli zwraca nasz Czo³g

	if (PlayerPawn)
	{
		TankInGameModeClass = Cast<ATank>(PlayerPawn);
		//rzuttukemy czo³g z pionka gracza
		if (!TankInGameModeClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode: Failed to find the tank actors!"));
		}
	}
	
	//S4 L116
	//AActor* Tower = Towers[0];
	//odnoszenie siê do elemetów tablicy u¿ywamy nawiasów prostok¹tnych, tutaj pierwszy element poniewa¿ numerowane sa od 0 1 2 3 4 itd tutaj mamy zapisanie pierwszego elementu tablicy do
	//zmiennej - oczywiœcie musi byæ tego samego typu co tablica

	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount) //mamy pêtle
	{
		AActor* TowerActor = Towers[LoopIndex]; //która bêdzie zapêtlaæ ka¿dy element wewn¹trz tablicy wierzy
		//zapisujemy jeden z elementów tablicy do zmiennej

		if (TowerActor)
		{
			ATower* Tower = Cast<ATower>(TowerActor); // spróbuje rzutowaæ ten element do zmiennej typu wierzy
			//obsadzamy TowerActor jako Tower wierzê w nowej zmiennej
			if (Tower && TankInGameModeClass)
			{
				Tower->TankInTowerClass = TankInGameModeClass; //a jeœli to zadzia³a ustawi Tank z klasy Tower na Tank ale który mamy tutaj w GameMode
				//Tower->Tank to czo³g który mamy w klasie Tower czyli tamten czo³g przypisuje do czo³gu który mamy tutaj w GameMode

				//UE_LOG(LogTemp, Warning, TEXT("%s setting the Tank veriable"), *Tower->GetActorNameOrLabel());
			}

		} 

		LoopIndex++;
		//zwiêkszamy licznik/index pêtli o jeden
	}

	//S4 L115
	/*
	UE_LOG(LogTemp, Warning, TEXT("Starts of the while loop"));
	int32 LoopIndex = 0;
	while (LoopIndex < 10)
	//pêtla while bêdzie wykonywana dopuki warunek bêdzie prawdziwy
	{
		UE_LOG(LogTemp, Warning, TEXT("Boody of the while loop, loop index: %d"), LoopIndex);
		LoopIndex++;
	}
	UE_LOG(LogTemp, Warning, TEXT("End of the while loop, loop index: %d"), LoopIndex);
	*/

	//S4 L138
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		//utwórz widget i zwróci jako wskaŸnik - funkca szablonowa
		//<typ klasy widgetu który chcemy utworzyæ, tutaj nasza klasa>(kontroler gracza, obiekt klasy TSubclassOf który bêdzie BP widgetu)
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen(0);
			//dodajemy widget do ekranu gracza 0 aby by³ widoczny wyœwietlany tekst
			ScreenMessageWidget->SetMessageText("Get Ready! Kill All Towers");
			//przekazujemy tekst do klasy UScreenMessage do funkcji SetMessageText
		}

		ScreenCountTowersWidget = CreateWidget<UScreenCountTowers>(PlayerController, ScreenCountTowersClass);
		//utwórz widget i zwróci jako wskaŸnik - funkca szablonowa
		//<typ klasy widgetu który chcemy utworzyæ, tutaj nasza klasa>(kontroler gracza, obiekt klasy TSubclassOf który bêdzie BP widgetu)
		if (ScreenCountTowersWidget)
		{
			ScreenCountTowersWidget->AddToPlayerScreen(0);
			//dodajemy widget do ekranu gracza 0 aby by³ widoczny wyœwietlany tekst
			ScreenCountTowersWidget->SetMessageText("Enemies to kill: ", FString::FromInt(TowerCount));
			//przekazujemy tekst do klasy UScreenCountTowers do funkcji SetMessageText
		}
	}

	//S4 L135
	CountdownSeconds = CountdownDelay;

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1, true);
	//timer który wystartuje funkcje gdzie jst odliczanie do poczatku gry
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	//S4 L135
	CountdownSeconds--;

	if (CountdownSeconds > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Countdown: %d"), CountdownSeconds);
		//S4 L138
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
		//przekazujey teks skonwertowany z int
	}
	else if (CountdownSeconds == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Go Go Go"));
		TankInGameModeClass->SetPlayerEnabled(true);
		//S4 L138
		ScreenMessageWidget->SetMessageText("Go Go Go!");
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		//czyszczenie Timera aby skoñczyæ odliczanie
		//UE_LOG(LogTemp, Warning, TEXT("Clear Timer"));

		//S4 L138
		//ScreenMessageWidget->RemoveFromViewport();
		//usuwa UserWidget z ekranu
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
		//ustawiamy widocznoœæ widgetu na ukryty
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	//S4 L131
	bool IsGameOver = false;

	//S4 L127
	if (DeadActor == TankInGameModeClass)
	//porównujemy pi¹ki które zgin¹ a mamy tylko 2
	{
		// tutaj jeœli zginie czo³g
		TankInGameModeClass->HandleDestruction();
		IsGameOver = true;
	}
	else
	{
		//jeœli zginie wierza
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower)
		{
			DeadTower->HandleDestruction();
			TowerCount--;

			ScreenCountTowersWidget->SetMessageText("Enemies to kill: ", FString::FromInt(TowerCount));

			if (!TowerCount)
			{
				//UE_LOG(LogTemp, Warning, TEXT("All Towers are dead, Y Win"));
				IsVictory = true;
				IsGameOver = true;
			}
		}
	}

	//S4 L131
	if (IsGameOver)
	{
		
		FString GameOverString;
		/*
		if (IsVictory)
		{
			GameOverString = "Victory";
		}
		else
		{
			GameOverString = "Defeat";
		} */

		//operator trójwarunkowy/instrukcja trójargumentowa zamiast tego if else na górze. if else jest potê¿niejsze mo¿emy dodaæ ca³y ci¹g kodu po warunku, tutaj nie
		GameOverString = IsVictory ? "Victory :)" : "Defeat :(";
		//zmienna do której chcemy przypisaæ wyniuk = warunek, znak zapytania, po znaku wartoœæ która bêdzie dla wyniku true warunku, dwukropek i wartoœæ warunku dla wyniku false

		//S4 L138
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);
		//ustawiamy widocznoœæ widgetu na widoczny
		ScreenMessageWidget->SetMessageText(GameOverString);

		//S4 L132
		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
		//timer bêdzie czeka³ 3s a¿ wywo³a funkcjê OnGameOverTimerTimeout bez êtli - tylko raz
	}

}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	//S4 L132
	//FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	//pobieramy nazwê aktualnego lvl
	//UGameplayStatics::OpenLevel(GetWorld(), *CurrentLevelName);
	//otwiera lvl(pobieramy œwiat, nazwa lvl jaki ma siê otworzyæ - aktualny lvl * - aby FString czyta³ jako FName)

	//S4 L134
	UGameInstance* GameInstance = GetGameInstance();
	//funkcja zwraca instancjê gry jako nowy wskaŸnik instancji gry, funkcja nale¿y do aktora wiêc w dowolnym aktorze w dowolnym miejscu kodu zawsze mo¿esz uzyskaæ instancjê gry 

	if (GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (IsVictory)
			{
				//za³adowanie nastêpnego poziomu po zwyciêstwie
				//S4 L134
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				//przeladowanie poziomu gdy przegramy
				//S4 L134
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}

}

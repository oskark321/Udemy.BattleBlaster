// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"



ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//S4 L101
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")); //tworzymy subobjekt, potem widoczny w edytorze jak stworzymy BP_<nadajemu mu typ>(i nazwê)
	SpringArmComp->SetupAttachment(RootComponent); //przypinamy do root

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent")); //tworzymy subobjekt, potem widoczny w edytorze jak stworzymy BP_<nadajemu mu typ>(i nazwê)
	CameraComp->SetupAttachment(SpringArmComp); //przypinamy do ramienia sprêz¿ynuj¹cego

}
// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	//S4 L104
	//dodawanie konteksty mapowania - kod jest standardowy za ka¿dym razem bêdzie wygl¹da³ tak samo
	/*
	if (DefaultMappingContext)
	{
		APlayerController* PlayerControler = Cast<APlayerController>(Controller); //rzutowanie<typ w który chcemy przekszta³ciæ kontroler>(i co chcemy przekszta³ciæ)
		//potrzebujemy tego aby pobraæ lokalnego gracza
		  if (PlayerControler)
		{
			ULocalPlayer* LocalPlayer = PlayerControler->GetLocalPlayer(); //za pomoc¹ playerControler pobieramy lokalnego gracza

			if (LocalPlayer)
			{
				UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
				//uzyskujemy dostêp do rozszerzonego podsystemu lokalnego odtwarzacza wejœciowego abyœmy mogli wykonaæ wywo³anie funkcji dodania kontekstu mapowania (AddMappingContext)
				//aby to uzyskaæ bêdziemy musieli uzyskaæ dostêp do funkcji klasy z lokalnej klasy gracza po przez funkcje GetSubsystem<>() - jest to funkcja klasy dla tego musimy u¿yæ
				//nazwy klasy (ULocalPlayer) i operatora rozdzielnoœci zakresu (::) i dopiero nazwê funkcji (GetSubsystem) - pobierz podsystem okreœlonego typu
				//<oczywiœcie typ jaki chcemy aby zwróci³a funkcja>(i co chcemy przekszta³ciæ)

				if (Subsystem)
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
					//dodajemy kontekst mapowania (zmienna UInputMappingContext do której w edytorze dodajemy nasz IMC_Default, tutaj priorytet mapowania - mamy tylko jeden wiêc nie ma znaczenia)
					//UE_LOG(LogTemp, Warning, TEXT("Yes, we have a Default Mapping Context"));
				}
			}
		}
	}
	*/
	//S4 L104 i L105 wyjaœnienie i ops na górze w L104 robimy wszystkie if a w L105 uczymy siê deklarowania zmiennych wewn¹trz if
	PlayerController = Cast<APlayerController>(Controller);
	if (DefaultMappingContext)
	{
		if (PlayerController)
		{
			if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
					//UE_LOG(LogTemp, Warning, TEXT("Yes, we have a Default Mapping Context"));
				}
			}
		}

	}

	//S4 L135
	SetPlayerEnabled(false);
	//uruchamiamy funkcjê wiêc wy³¹czamy wejœcia i kursor myszki
	 
	//S4 L105
	//tworzenie zmiennej w if
	if (bool MyBool = true) //jeœli tu utworzê zmienn¹ zostanie ona usuniêta na koniec tego bloku
	{ //blok kodu to para nawiasów klamrowych takich jak te w if lub funkcjach od tego do tego }
		//zmienna lokalna utworzona wewn¹trz bloku kodu jest dostêpna tylko wewn¹trz tego bloku np
		int MyInt = 10; //jeœli tu utworzê zmienn¹ zostanie ona usuniêta na koniec tego bloku kodu czyli jak zamkniemy nawias klamrowy } od tego if
		//UE_LOG(LogTemp, Warning, TEXT("It Works"));
	} //do tego

}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//S4 L111
	if (PlayerController) //Cast<APlayerController>(Controller) mo¿e byc tez tak
		//z kontrolera uzyskujemy/rzutujemy na APlayerController
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		//pobiera trafienie z kursora(jaki kanal kolizji tu kana³ widocznoœci, false na kompleks œladów - pyta czy chemy u¿ywaæ z³o¿onych kszta³tów kolizji do œledzenia - tu nie potrzebne
		//wiec false, zmienna w której zapisywane s¹ dane z trafienia)
		FVector HitPoint = HitResult.ImpactPoint;
		//pobieramy punk trafienia w œwiecie - to bêdzie nasze obracanie wierzyczki te¿ 

		//DrawDebugSphere(GetWorld(), HitPoint, 30.0f, 15, FColor::Green);

		//S4 L112
		RotateTurret(HitPoint);

	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//S4 L107
	if (MoveInputAction && TurnInputAction)
	{
		if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
			//zdobywamy ulepszony komponent wejœciowy aby zdobyæ ulepszon¹ funkcjê BindAction
			//rzutuj¹c<tutaj oczywiœcie jaki typ chcemy uzyskaæ>(komponent wejœciowy który daje nam ta funkcja SetupPlayerInputComponent - s³u¿y ona do powi¹zañ funkcjonaloœci z wyjœciami)
		{
			PlayerEnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
			//korzystaj¹c z ulepszonego komponentu wejœcia wywo³ujemy funkcje AkcjaWi¹zania(zmienna UInputAction któr¹ ³aczymy z InputAction w edytorze UE,
			// zdarzenie wyzwalaj¹ce - co ma siê staæ aby to po³¹czenie zadzia³a³o w tym przypadku nacisn¹æ klawisz, uObject jaki objekt czyli ten, adres funkcji która ma zostaæ po³¹czona)
			
			//S4 L110
			PlayerEnhancedInputComponent->BindAction(TurnInputAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);

			//S4 L118
			PlayerEnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Started, this, &ATank::FirePawn);
			//funkcjê FirePawn mamy w klasie BasePawn lecz klasa Tank dzedziczy po klasie BasePawn wiêc mo¿emy napisaæ ATank::FirePawn
		}
	}

}

void ATank::MoveInput(const FInputActionValue& Value)
//const - nie mo¿emy zmieniæ wartoœci zmiennej wewn¹trz funkcji mo¿emy jedynie odczytaæ wartoœæ zmiennej. zmienna referencyjna do wartoœci InputAction aby
//klawisz W przyj¹³ wartoœæ do przody: 1.0 a klawisz S przyj¹³ wartoœæ do ty³u: -1.0
{
	//S4 L108
	float InputValue = Value.Get<float>();
	//funkcja Get wyodrêbmnia nam typ float z zmiennej Value - chcemy z tego tylko liczbê

	//S4 L109
	FVector DeltaLocation = FVector(0.0f, 0.0f, 0.0f);
	DeltaLocation.X = TankSpeed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	//z wektora oœ X ustawiamy na PrêdkoœcCzo³gu sami j¹ definujemy * InputValue czyli wartoœæ z przycisków albo +1 albo -1 * DeltaTime - dziêki temu otrzymamy DeltaTime a ¿e jest to
	//funkcja klasy dla tego (::)
	AddActorLocalOffset(DeltaLocation, true);
	//dodaje lokalny przesuniêcie aktora i przesuwaj¹cy nas w osi X lokalnej aktora(przesuniêcie jako FVector, true dla bSweep jeœli na true to silnik wykona zamiatanie/przeszukieawnie
	//kolizji dziêki czemu zatrzymamy siê na scianie czy na wrogu 

	//UE_LOG(LogTemp, Warning, TEXT("Input Value is %f"), InputValue);
}

//S4 L110
void ATank::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	//Get pobiera <dany typ tu float> z zmiennej ró¿nego typu

	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);

	DeltaRotation.Yaw = TurnSpeed * InputValue * GetWorld()->GetDeltaSeconds();
	//obracamy siê wzglêdem osi Z czyli chcemy skrêciæ - prêdkoœæ obrotu nasza mienna * WartoœæWejscia czyli tak samo wartoœæ z przycisków +1 albo -1 * DeltaTime - tak te¿ mo¿emy uzyskaæ
	//DeltaTime

	AddActorLocalRotation(DeltaRotation, true);
	//dodajemy lokaln¹ rotacjê aktora i obracamy siê wed³ug osi Z czyli Yaw(Rotacja jako FRotator, i true dla bSweep aby przeszukiwa³o kolizjê i nie przenika³o przezz œciany/wrogów
	//UE_LOG(LogTemp, Warning, TEXT("Rotation Input Value : %f"), InputValue);

}

void ATank::HandleDestruction()
{
	//S4 L128
	Super::HandleDestruction();
	//wykonujemy Super::nazwa funkcji poniewa¿ g³owna funkcja jest w klasie ABasePawn, w ATank i ATower mamy taka sam¹ funkcjê dla tego musimy wywo³aæ super na samym pocz¹tku funkcji!
	//mo¿emy tworzyæ te same funkcje tylko w jednostce nadrzêdnej i podrzêdnej czyli w klasie rodzic i dziecko. Oryginalna funkcja jest w klasie ABasePawn poniewarz to jest rodziæ i
	//czêœæ rzeczy jak dzwiêk czy ob³ok po zniszczeni bêdzie dzia³a siê to samo w ATamk i ATower czyli w klasach dzieci gdzie mamy super i bêd¹ sie ju¿ wykonywac rzeczy typowo dla danej
	//klasy np ATank - po zniszczenu czo³g bêdzie niewidoczny aby dalej kamera dzia³a i wyœwietli siê komunikat ¿e przegrywasz grê, ATower - po zniszczemi niszczymy na prawdê wierze.
	//Najpierw jest wywo³ywana funkcja nadrzêdna bez Super a potem funkcja podrzêdna która zawiera s³owo Super

	//UE_LOG(LogTemp, Warning, TEXT("Tank HandleDestruction"));
	
	//S4 L129
	SetActorHiddenInGame(true);
	//powoduje ¿e aktor staje siê niewidoczny, tylko niewidoczny mo¿e chodziæ strzelaæ i inni mog¹ strzelaæ do niego

	SetActorTickEnabled(false);
	//wy³¹czamy funkcje Tick

	SetPlayerEnabled(false);
	IsAlive = false;

}

void ATank::SetPlayerEnabled(bool Enabled)
{
	//S4 L129
	if (PlayerController)
	{
		if (Enabled)
		{
			EnableInput(PlayerController);
			//wl¹czamy Input czyli wejœcia klawiszy
		}
		else
		{
			DisableInput(PlayerController);
			//wy³¹czamy Input czyli wejœcia klawiszy
		}

		//S4 L130	
		PlayerController->bShowMouseCursor = Enabled;
		//ustawia kursor myszy na true - widoczny lub false - niewidoczny
		//literka b oznacza ¿e jest to typ bool - true/false

	}
}

bool ATank::GetIsAlive()
{
	return IsAlive;
}

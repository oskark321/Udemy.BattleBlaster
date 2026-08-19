// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//S4 L120
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);
	//S4 L121
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	//jeœli komponent jest komponentem aktora UActorComponent to po utworzeniu w konstruktorze nie misimy go nigdzie do³aczaæ
	ProjectileMovementComp->InitialSpeed = 1000.0f;//parametry w komponencie mo¿emy zmieniaæ równie¿ z poziomu c++, nie tylko w edytorze
	ProjectileMovementComp->MaxSpeed = 1000.0f;//parametry w komponencie mo¿emy zmieniaæ równie¿ z poziomu c++, nie tylko w edytorze

	//S4 L139
	TrailParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailParticles"));
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	//S4 L123
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	//po trafieniu komponentu - tworzymy delegat zdarzenie wywo³ane gdy komponent uderzy (lub zostanie uderzony) w coœ twardego - gdy trafi w coœ to zmienne w funkcji OnHit zostan¹ 
	//uzupe³nione informacjami o zdarzeniu trafienia np HitComponent - komponent który wykona³ uderzenie czyl ProjectileMesh, OtherActor - aktor którego trafimy, OtherComp - to komponent
	//w tym aktorze który trafiliœmy i inne informacje w argumentach które mamy w funkcji. Jest to specjalny delegat z którym powi¹¿emy funkcjê co wyda¿y siê po trafieniu,
	//delegat oczekuje specjalnych argumentów dla tego deklarujemy je w funkcji (OnHit) która jest po³¹czona z delegatem, aby znaleœæ te agumenty - Ctrl+OnComponentHit
	//aby otworzyæ deklaracjê tego delegata potem Ctrl+FComponentHitSignature aby otworzyæ deklaracje typu tego delegata tam mamy FiveParams - mówi nam to ¿e delegat oczekuje piêciu ró¿nych
	//argumentów (FComponentHitSignature - nazwa typu delegata, UPrimitiveComponent - klasa która jest w³aœcicielem tego delegata, OnComponentHit - nazwa zmiennej w klasie, nastêpnie
	//zestaw piêciu paramtrów których oczekuje delegat parametry te s¹ podzielone na dwie sekcje typ parametrów i nazwê parametru
	//AddDynamic - ³¹czy nam delegata z funkj¹ (ten objekt, i adres funkcji)

	//S4 L140
	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaunchSound, GetActorLocation(), VolumeSound, VolumeSound);
		//odtwórz dzwiêk w lokalizacji(w tym œiwecie, Jaki to dzwiêk - zmienna do której przypisujemy dzwiêk w edytorze, lokalizacjê tego dzwiêku - czyli w sumie lokalizacjê aktora,
		//g³oœnoœæ dzwiêku zmienna float, tutaj doda³em jakiœ efekt dzwiêkowy aby ró¿ni³ siê trochu od dzwiêku ude¿enia)
	}

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//S4 L125
	AActor* MyProjectileOwner = GetOwner();
	//ustawiamy w³aœciciela klasy AProjectile w³aœciciela pocisku

	if (MyProjectileOwner)
	{
		if (OtherActor && (OtherActor != MyProjectileOwner) && (OtherActor != this))
		//sprawdzamy czy istnieje wogóle trafiony aktor, Trafiony aktor musi byæ inny ni¿ w³aœciciel pocisku i czy nie jest równy temu aby nie trafiæ siebie jako pocisk)
		{
			
			UGameplayStatics::ApplyDamage(OtherActor, Damage, MyProjectileOwner->GetInstigatorController(), this, UDamageType::StaticClass());
			//zastosuj obra¿enia (aktor który zosta³ uszkodzony, iloœæ obra¿eñ które zostan¹ zadane uszkodzonemu aktorowi, inicjator wydarzenia - kontroler odpowiedzialny za spowodowanie
			//uszkodzenia czyli kontroler czo³gu/wierzy kontroler podrzegacza w³aœciciela pocisku, faktyczny aktor który spowodowa³ uszkodzenia czyli pocisk, klasa typu uszkodzenia
			//która jest obiektem UClass - okreœlamy jaki rodzaj obra¿eñ jest stosowany w tej grze nie korzystamy z tego wiêc u¿yjemy tylko ogólnego typu obra¿eñ
			//DamageType - jest u¿ywany do tworzenia wielu typów obra¿eñ np obra¿enia od eksplozji, od uszkodzenia pocisku, od po¿aru itp)

			//S4 L139
			if (HitParticles)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticles, GetActorLocation(), GetActorRotation());
				//u¿ywaj¹c funkcji z biblioteki NIGARA tworzymy system w lokalizacji(oczywiœcie pobieramy œwiat w któym dziaamy, zmienna gdzie w edytorze mamy przyisany ten efekt cz¹steczkowy,
				//pobieramy lokalizacjê aktora gdzie ma siê pojawiæ ten efekt tutaj w miejscu gdzie ude¿y pocisk, no i jeszcze rotacjê pocisku dla systemu cz¹steczek)
			}

			//S4 L140
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation(), VolumeSound);
				//odtwórz dzwiêk w lokalizacji(w tym œiwecie, Jaki to dzwiêk - zmienna do której przypisujemy dzwiêk w edytorze, lokalizacjê tego dzwiêku - czyli w sumie lokalizacjê aktora,
				//g³oœnoœæ dzwiêku zmienna float)
			}

			//S4 L141
			if (HitCameraShakeClass)
			{
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				//pobieramy kontroler gracza z UGS (ten œwiat, no i ¿e single to indeks gracza 0)
				if (PlayerController)
				{
					PlayerController->ClientStartCameraShake(HitCameraShakeClass);
					//rozpocznij dr¿enie kamery (zmienna typu subclass gdzie w edytorze przypiszemy do niej BP - bez do³¹czania ¿adnych nag³ówków)
				}
			}

		}

	}
	Destroy();

}



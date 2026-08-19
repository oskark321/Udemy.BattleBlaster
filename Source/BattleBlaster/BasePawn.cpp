// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//S4 L100
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent")); //tworzymy subobjekt, potem widoczny w edytorze jak stworzymy BP_<nadajemu mu typ>(i nazwê)
	SetRootComponent(CapsuleComp); //ustawiamy jako komponent g³ówny

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseComponent")); //tworzymy subobjekt, potem widoczny w edytorze jak stworzymy BP_<nadajemu mu typ>(i nazwê)
	BaseMesh->SetupAttachment(CapsuleComp); //przypinamy do root

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretComponent")); //tworzymy subobjekt, potem widoczny w edytorze jak stworzymy BP_<nadajemu mu typ>(i nazwê)
	TurretMesh->SetupAttachment(BaseMesh); //przypinamy do BaseComp
	
	//S4 L118
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint")); //tworzymy subobjekt, potem widoczny w edytorze jak stworzymy BP_<nadajemu mu typ>(i nazwê)
	ProjectileSpawnPoint->SetupAttachment(TurretMesh); //przypinamy go do TurretMesh
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	//S4 L112
	FVector VectorToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	//wektor do celu = wektor uderzenia czo³gu a wierzyczki jeszcze nie wiadomo - lokalizacja siatki Mesh wierzyczki

	FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f);
	//spojrzenie na rotacje = pobieramy i ustawiamy tylko obrót w oko³o osi Z aby nam siê wierzyczka tylko obraca³a lewo prawo

	FRotator InterpolatedRotation = FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), 3.0f);
	//R oznacza rotation czyli funkcja pobiera rotacjê i interpoluje go w kierunku docelowym(aktualna rotacja, docelowa rotacja, DeltaTime, prêdkoœæ z jak¹ siê obraca)

	TurretMesh->SetWorldRotation(InterpolatedRotation);
	//ustawia obrót w przestrzeni œwiata to wa¿ne ¿e w przestrzeni œwiata(i kierunek obrotu)

}

void ABasePawn::FirePawn()
{
	//S4 L118
	FVector ProjectileLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ProjectileRotation = ProjectileSpawnPoint->GetComponentRotation();
	//DrawDebugSphere(GetWorld(), ProjectileLocation, 15.0f, 20, FColor::Green, false, 3.0f);

	//S4 L120
	//GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileLocation, ProjectileRotation);
	//funkcja odrodz aktora, funkcja szablonowa<nazwa klasy której aktor ma siê odrodziæ czyli nasz pocisk>(tutaj aktor z edytora który ma sie odrodziæ czyli nasz BP_Projectile ale ¿e nie
	//mamy w c++ do niego dostepu musimy utworzyæ now¹ zmienn¹ podklasê i ustawiæ j¹ w edytorze, lokalizacja odrodzenia sie pocisku, rotacja odradzaj¹cego siê pocisku)

	//S4 L122
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileLocation, ProjectileRotation);
	//mo¿emy zapisaæ tworz¹cego siê aktora do zmiennej
	
	if (Projectile)
	{
		Projectile->SetOwner(this);
		//ustawiamy w³aœciciela pocisku (potrzebna do zadawania obra¿eñ) - przekazujemy aktorowi czo³g lub wierza ¿e pocisk bedzie mia³ go jako w³aœciciela, funkcja SetOwner pochodzi
		//z klasy AActor wiêc ka¿dy aktor z którym pracujesz mo¿esz ustawiæ jego w³aœciciela za pomoc¹ funkcji SetOwner
		//this - wskaŸnik do instancji tej klasy, ¿e pracujemy w BasePawn to w tym prypadku bedzie to wiêc wskaŸnik do klasy ABasePawn, jeœli strzelac bêdzie czo³g gracza to wskaŸnik 
		//bêdzie wskazywa³ bazowy pionek czo³gu gracza czyli czo³g, jeœli wierza bedzie strzelaæ to bêdzie wskazywaæ na wierze
		//za ka¿dym razem gdy wystrzelimy pocisk ustawimy jego w³aœciciela na aktora bazowego czyli czo³g lub wierza
		
		/*
		AActor* ProjectileOwner = Projectile->GetOwner();
		if (ProjectileOwner)
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner the Projectile is: %s"), *ProjectileOwner->GetActorNameOrLabel());
		}
		*/


	}
}

void ABasePawn::HandleDestruction()
{
	//S4 L128
	//UE_LOG(LogTemp, Warning, TEXT("BasePawn HandleDestruction"));

	//S4 L139
	if (DeathParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathParticles, GetActorLocation(), GetActorRotation());
		//u¿ywaj¹c funkcji z biblioteki NIGARA tworzymy system w lokalizacji(oczywiœcie pobieramy œwiat w któym dziaamy, zmienna gdzie w edytorze mamy przyisany ten efekt cz¹steczkowy,
		//pobieramy lokalizacjê aktora gdzie ma siê pojawiæ ten efekt tutaj w miejscu gdzie ude¿y pocisk, no i jeszcze rotacjê pocisku dla systemu cz¹steczek
	}

	//S4 L140
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), VolumeSound);
		//odtwórz dzwiêk w lokalizacji(w tym œiwecie, Jaki to dzwiêk - zmienna do której przypisujemy dzwiêk w edytorze, lokalizacjê tego dzwiêku - czyli w sumie lokalizacjê aktora,
			//g³oœnoœæ dzwiêku zmienna float)
	}

	//S4 L141
	if (DeathCameraShakeClass)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(DeathCameraShakeClass);
			//rozpocznij dr¿enie kamery (zmienna typu subclass gdzie w edytorze przypiszemy do niej BP - bez do³¹czania ¿adnych nag³ówków)
		}
	}

}


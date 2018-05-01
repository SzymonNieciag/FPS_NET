// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSobject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"


// Sets default values
AFPSobject::AFPSobject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;
	SphereCOmponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCOmponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCOmponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCOmponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	SphereCOmponent->SetupAttachment(MeshComponent);

	//SphereCOmponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSobject::OnOverlapBegin);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSobject::BeginPlay()
{
	Super::BeginPlay();
	PlayEffect();
}

void AFPSobject::PlayEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}


void AFPSobject::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffect();
	if (Role = ROLE_Authority) {
		UE_LOG(LogTemp, Warning, TEXT("Your message"));
		auto *FPScharacter = Cast<AFPSCharacter>(OtherActor);

		if (FPScharacter)
		{
			FPScharacter->isCarryingObjective = true;
			Destroy();
		}
	}
}
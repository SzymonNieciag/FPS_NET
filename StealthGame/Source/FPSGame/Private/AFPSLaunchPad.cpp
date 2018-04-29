// Fill out your copyright notice in the Description page of Project Settings.

#include "AFPSLaunchPad.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>


// Sets default values
AAFPSLaunchPad::AAFPSLaunchPad()
{
	OverlapComp=CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap  Component"));
	OverlapComp->SetBoxExtent(FVector(75, 75, 50));
	RootComponent = OverlapComp;

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	MeshComp->SetupAttachment(RootComponent);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AAFPSLaunchPad::OverlapBegin);

	LaunchStrength = 1500;
	LaunchPitchAngle = 35.00f;

}

void AAFPSLaunchPad::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResultp)
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector()*LaunchStrength;

	ACharacter*otherCharacter = Cast<ACharacter>(OtherActor);
	if (otherCharacter)
	{
		otherCharacter->LaunchCharacter(LaunchVelocity, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadeffect, GetActorLocation());
	}
	else if (OtherComp)
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadeffect, GetActorLocation());
	}
}

// Called when the game starts or when spawned
void AAFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}


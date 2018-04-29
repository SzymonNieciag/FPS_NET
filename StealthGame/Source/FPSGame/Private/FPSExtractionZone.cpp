// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	BoxCom = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxCom->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCom->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCom->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxCom->SetBoxExtent(FVector(200.0f));
	RootComponent = BoxCom;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f);

	DecalComp->SetupAttachment(BoxCom);

	BoxCom->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::OnOverlapBegin);
}
void AFPSExtractionZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Your message"));
	AFPSCharacter *MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn&&MyPawn->isCarryingObjective)
	{
		AFPSGameMode *GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(MyPawn,true);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissionSound);
	}
}

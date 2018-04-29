// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS_AI_Guardian.h"
#include <Perception/PawnSensingComponent.h>
#include <DrawDebugHelpers.h>
#include "FPSGameMode.h"
#include <AI/Navigation/NavigationSystem.h>


// Sets default values
AFPS_AI_Guardian::AFPS_AI_Guardian()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));

	SensingComponent->OnSeePawn.AddDynamic(this, &AFPS_AI_Guardian::OnpawnSee);
	SensingComponent->OnHearNoise.AddDynamic(this, &AFPS_AI_Guardian::OnNoiseHeard);

	GuardianState = EAIstate::Idle;
}

// Called when the game starts or when spawned
void AFPS_AI_Guardian::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();

	if (bPatrol)
	{
		MoveToNextTopatrolPoint();
	}
	
}

void AFPS_AI_Guardian::OnpawnSee(APawn *otherPawn)
{
	if (otherPawn)
	{
		DrawDebugSphere(GetWorld(), otherPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);
		SetGuardianState(EAIstate::Alerted);
	}
	AFPSGameMode *GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(otherPawn, false);
	}
	// Stop patroling
	AController *Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
		CurrentpatrolPoint = NULL;
	}

}

void AFPS_AI_Guardian::OnNoiseHeard(APawn * NoiseInstigator, const FVector & Location, float Volume)
{
	if (GuardianState == EAIstate::Alerted)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Yellow, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimeHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimeHandle_ResetOrientation, this, &AFPS_AI_Guardian::ResetOrientation,3.0f);

	SetGuardianState(EAIstate::Suspicious);
	// Stop patroling
	AController *Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}

}

void AFPS_AI_Guardian::SetGuardianState(EAIstate NewState)
{
	if (GuardianState == NewState)
		return;
	else
		GuardianState = NewState;

	OnStateChange(NewState);
}

void AFPS_AI_Guardian::ResetOrientation()
{
	if (GuardianState == EAIstate::Alerted)
		return;

	SetActorRotation(OriginalRotation);

	SetGuardianState(EAIstate::Idle);
	// Stop patroling
	if (bPatrol)
	{
		MoveToNextTopatrolPoint();
	}
}

// Called every frame
void AFPS_AI_Guardian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentpatrolPoint)
	{
		FVector Delta = GetActorLocation() - CurrentpatrolPoint->GetActorLocation();
		float DistanceToGoal = Delta.Size();
		if (DistanceToGoal < 100)
		{
			MoveToNextTopatrolPoint();
		}
	}

}

void AFPS_AI_Guardian::MoveToNextTopatrolPoint()
{
	if (CurrentpatrolPoint != FirstPatrolPoint)
		CurrentpatrolPoint = FirstPatrolPoint;
	else
		CurrentpatrolPoint = SecondPatrolPoint;
	UNavigationSystem::SimpleMoveToActor(GetController(), CurrentpatrolPoint);
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPS_AI_Guardian.generated.h"

UENUM(BlueprintType)
enum class EAIstate:uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPS_AI_Guardian : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPS_AI_Guardian();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Components")
	class UPawnSensingComponent *SensingComponent;

	UFUNCTION()
		void OnpawnSee(APawn *otherPawn);
	UFUNCTION()
		void OnNoiseHeard(APawn *NoiseInstigator,const FVector& Location,float Volume);

	FRotator OriginalRotation;

	FTimerHandle TimeHandle_ResetOrientation;

	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
	EAIstate GuardianState;

	UFUNCTION()
		void OnRep_GuardState();
	UFUNCTION()
	void SetGuardianState(EAIstate NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void OnStateChange(EAIstate NewState);

	UFUNCTION()
	void ResetOrientation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(editinstanceonly, Category = "AI")
		bool bPatrol;

	UPROPERTY(editinstanceonly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor *FirstPatrolPoint;
	UPROPERTY(editinstanceonly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor *SecondPatrolPoint;

	AActor *CurrentpatrolPoint;

	void MoveToNextTopatrolPoint();

	
};

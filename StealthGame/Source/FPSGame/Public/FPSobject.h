// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSobject.generated.h"

UCLASS()
class FPSGAME_API AFPSobject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSobject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnyWhere, Category = "Components")
		class UStaticMeshComponent *MeshComponent;

	UPROPERTY(VisibleAnyWhere, Category = "Components")
		class USphereComponent *SphereCOmponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class UParticleSystem *PickupFX;

	void PlayEffect();

public:
	// Called every frame
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//UFUNCTION()
    //void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
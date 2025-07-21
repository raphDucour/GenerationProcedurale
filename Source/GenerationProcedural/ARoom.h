// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "ARoom.generated.h"

UCLASS()
class GENERATIONPROCEDURAL_API AARoom : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARoom();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	// Composant pour l'entrée (flèche verte)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* EntryPoint;

	// Racine pour les sorties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* ExitsRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* FirstExit;

	// Box de collision pour vérifier la place disponible
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

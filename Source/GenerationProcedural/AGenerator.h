// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "AGenerator.generated.h"

USTRUCT(BlueprintType)
struct FRoomConfig
{
	GENERATED_BODY()

	// Type de salle
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AARoom> RoomClass;

	// Probabilité d'apparition 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Probability = 1.0f;

	// Peut spawn une seule fois ?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = " Ne peut spawn que une fois max", ToolTip = "Si coché, cette salle ne pourra apparaître qu'une seule fois dans la génération."))
	bool bUnique = false;

	// Peut spawn une seule fois ?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = " Doit apparaitre au moins une fois", ToolTip = "Si coché, cette salle devra apparaître au moins une fois dans la génération, si vous mettez une probabilité de 0 elle apparaitra à la toute fin."))
	bool bMustSpawn = false;
};

UCLASS()
class GENERATIONPROCEDURAL_API AAGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* Root;

	// Probabilité d'apparition 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	int numGeneration = 100;

	// Fonction pour générer la map
	UFUNCTION(BlueprintCallable)
	void GenerateMap(int NumRoomGenerated);

	// Liste des types de salles possibles avec config
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoomConfig> RoomConfigs;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	TArray<FTransform> OpenExits;

	void AddNewExits(AARoom* Room);
	bool IsRoomOverlapping(AARoom* Room);

	int PickRoomIndexByProbability();
	AARoom* SpawnRoomAtExit(int RoomIdx, const FTransform& ExitTransform);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WorldCube.generated.h"

UCLASS()
class DEX_API AWorldCube : public AActor
{
	GENERATED_BODY()

	UStaticMeshComponent* Cube;

	UPROPERTY() 
		UMaterial* Grass;
	UPROPERTY() 
		UMaterial* Gold;
	UPROPERTY()
		UMaterial* Glass;
	
public:	
	// Sets default values for this actor's properties
	AWorldCube();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void UpdateMesh(int);

	const static int32 CubeScale = 200;
	const static int32 GRASS = 0;
	const static int32 GOLD = 1;
	const static int32 GLASS = 2;
	
};

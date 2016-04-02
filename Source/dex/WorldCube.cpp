// Fill out your copyright notice in the Description page of Project Settings.

#include "dex.h"
#include "WorldCube.h"

// Sets default values
AWorldCube::AWorldCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube'"));

	static ConstructorHelpers::FObjectFinder<UMaterial> Grass(TEXT("MaterialInstanceConstant'/Game/StarterContent/Materials/M_Ground_Moss'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Gold(TEXT("MaterialInstanceConstant'/Game/StarterContent/Materials/M_Metal_Gold'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Glass(TEXT("MaterialInstanceConstant'/Game/StarterContent/Materials/M_Glass'"));
	this->Grass = Grass.Object;
	this->Gold = Gold.Object;
	this->Glass = Glass.Object;

	this->Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("A Cube"));
	this->Cube->SetStaticMesh(StaticMesh.Object);
	this->Cube->SetMaterial(0, this->Grass);

	float TempCubeScale = AWorldCube::CubeScale / 100.00;
	this->Cube->SetRelativeScale3D(FVector(TempCubeScale, TempCubeScale, TempCubeScale));

}

// Called when the game starts or when spawned
void AWorldCube::BeginPlay()
{
	Super::BeginPlay();

}

void AWorldCube::UpdateMesh(int Type) {
	if (Type == AWorldCube::GOLD) {
		//this->MyBlock->SetMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("/Game/StarterContent/Materials/M_Water_Ocean")));
		this->Cube->SetMaterial(0, this->Gold);
	} else if(Type == AWorldCube::GLASS) {
		this->Cube->SetMaterial(0, this->Glass);
	} else {
		this->Cube->SetMaterial(0, this->Grass);
	}
}


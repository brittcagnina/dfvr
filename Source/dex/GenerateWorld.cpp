// Fill out your copyright notice in the Description page of Project Settings.

#include "dex.h"
#include "GenerateWorld.h"
#include "WorldCube.h"
#include "WorldSingleton.h"
#include "FileHandler.h"
#include "PerlinNoise.h"

GenerateWorld::GenerateWorld()
{
}

GenerateWorld::~GenerateWorld()
{
}

void GenerateWorld::CreateMinimalLandscape(UWorld** World) {
	int32 x, y, z;
	//PerlinNoise Noise(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);
	int32 MaxHeight = 3;
	PerlinNoise Noise(1, .06, MaxHeight, 1, 42);
	for (x = -GenerateWorld::WorldXSize; x < GenerateWorld::WorldXSize; x++) {
		for (y = -GenerateWorld::WorldYSize; y < GenerateWorld::WorldYSize; y++) {

			double Height = Noise.GetHeight(x, y);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("BEFORE X=%d Y=%d Z="), x, y) +  FString::SanitizeFloat(Height));

			if (Height < 0) {
				Height = FMath::Ceil(Height);
			} else {
				Height = FMath::Floor(Height);
			}

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("AFTER X=%d Y=%d Z="), x, y) + FString::FromInt(Height));

			for (z = -(MaxHeight + 1); z <= Height; z++) {
				if (*World) {

					const FVector CubeLocation = FVector(
						x * AWorldCube::CubeScale,
						y * AWorldCube::CubeScale,
						z * AWorldCube::CubeScale
						);

					const FRotator CubeRotation = FRotator::ZeroRotator;
					if (z == 0) {
						this->AddCube(&(*World), CubeLocation, CubeRotation, AWorldCube::GRASS);
					}
					else {
						this->AddCube(&(*World), CubeLocation, CubeRotation, AWorldCube::GRASS);
					}

					/*
					FBufferArchive ToBinary;
					FileHandler::PushCubeDataToBuffer(&ToBinary, AWorldCube::GRASS, CubeLocation);
					FString Path = "C:\\ProgramData\\MyCubes\\CUBE " + CubeLocation.ToString() + ".bin";
					FileHandler::SaveDataToFile(Path, &ToBinary);
					*/
				}
			}

			/*
			for (z = -(GenerateWorld::WorldZSize - 1 ); z <= 0; z++) {
				if (*World) {

					const FVector CubeLocation = FVector(
						x * AWorldCube::CubeScale, 
						y * AWorldCube::CubeScale, 
						z * AWorldCube::CubeScale
					);

					const FRotator CubeRotation = FRotator::ZeroRotator;
					if (z == 0) {
						this->AddCube(&(*World), CubeLocation, CubeRotation, AWorldCube::GRASS);
					} else {
						this->AddCube(&(*World), CubeLocation, CubeRotation, AWorldCube::GRASS);
					}

					FBufferArchive ToBinary;
					FileHandler::PushCubeDataToBuffer(&ToBinary, AWorldCube::GRASS, CubeLocation);
					FString Path = "C:\\ProgramData\\MyCubes\\CUBE " + CubeLocation.ToString() + ".bin";
					FileHandler::SaveDataToFile(Path, &ToBinary);
				}
			}
			*/

		}
	}
}

void GenerateWorld::AddCube(UWorld** world, const FVector location, const FRotator rotation, int Type) {
	if (world) {
		AActor* CubeActor = (*world)->SpawnActor<AWorldCube>(AWorldCube::StaticClass(), location, rotation);
		//CubeActor->DisableComponentsSimulatePhysics();
		AWorldCube* Cube = Cast<AWorldCube>(CubeActor);

		if (Type != AWorldCube::GRASS) {
			Cube->UpdateMesh(Type);
		}

		WorldSingleton* Singleton = WorldSingleton::Instance();
		Singleton->WorldCubes.Add(location.ToString(), CubeActor);
		Singleton->DiscCubes.Add(location.ToString());
	}
}

void GenerateWorld::RemoveCube(UWorld** World, AActor** Actor) {
	if (World && Actor) {
		(*World)->DestroyActor(*Actor);
	}
}

FVector GenerateWorld::FindNearbyBelowCube(FVector Position) {
	return GenerateWorld::FindRelativeCube(Position, 0, 0, 0);
}

FVector GenerateWorld::FindNearbyAheadCube(FVector Position) {
	return GenerateWorld::FindRelativeCube(Position, 0, 0, AWorldCube::CubeScale);
}

FVector GenerateWorld::FindNearbyAheadFloorCube(FVector Position) {
	return GenerateWorld::FindRelativeCube(Position, 0, 0, 0);
}

FVector GenerateWorld::FindRelativeCube(FVector Position, int32 XBump, int32 YBump, int32 ZBump) {

	int32 CubeAtomicNum = AWorldCube::CubeScale;
	int32 XRemander = FMath::Round(Position.X) % CubeAtomicNum;
	int32 YRemander = FMath::Round(Position.Y) % CubeAtomicNum;

	int32 XDivided = XDivided = FMath::Floor(Position.X / CubeAtomicNum) * CubeAtomicNum;
	int32 YDivided = YDivided = FMath::Floor(Position.Y / CubeAtomicNum) * CubeAtomicNum;

	int32 XAdjust = CubeAtomicNum;
	int32 YAdjust = CubeAtomicNum;

	if (Position.X < 0) {
		XAdjust *= -1;
		XDivided = FMath::Ceil(Position.X / CubeAtomicNum) * CubeAtomicNum;
	}
	if (Position.Y < 0) {
		YAdjust *= -1;
		YDivided = FMath::Ceil(Position.Y / CubeAtomicNum) * CubeAtomicNum;
	}

	if (FMath::Abs(XRemander) >= (CubeAtomicNum / 2))
		XDivided = XDivided + XAdjust;
	if (FMath::Abs(YRemander) >= (CubeAtomicNum / 2))
		YDivided = YDivided + YAdjust;

	//Z axis logic (Pawn specific)
	int32 PawnHeight = 100;
	float ZPos = ( Position.Z / CubeAtomicNum ) - ( PawnHeight / 100 );
	ZPos = FMath::Floor(ZPos) * CubeAtomicNum;

	FVector NearbyCube = FVector(XDivided + XBump, YDivided + YBump, ZPos + ZBump);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Position.ToString() + FString(" NEARBY BELOW CUBE: ") + NearbyCube.ToString());
	return NearbyCube;
}
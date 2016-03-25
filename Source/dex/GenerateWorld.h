// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class DEX_API GenerateWorld
{
public:
	GenerateWorld();
	~GenerateWorld();
	void AddCube(UWorld**, const FVector, const FRotator, int type);
	void RemoveCube(UWorld**, AActor**);
	void CreateMinimalLandscape(UWorld**);
	static FVector FindRelativeCube(FVector, int32, int32, int32);
	static FVector FindNearbyBelowCube(FVector);
	static FVector FindNearbyAheadCube(FVector);
	static FVector FindNearbyAheadFloorCube(FVector);

	const static int32 WorldXSize = 20;
	const static int32 WorldYSize = 20;
	const static int32 WorldZSize = 5;
};

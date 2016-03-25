// Fill out your copyright notice in the Description page of Project Settings.

#include "dex.h"
#include "WorldSingleton.h"
#include "WorldCube.h"
#include "GenerateWorld.h"

WorldSingleton* WorldSingleton::InstancePtr = NULL;

WorldSingleton::WorldSingleton()
{
	TSet<FVector> Sphere;
	int x, y, z;
	for (x = -3; x < 3; x++) {
		for (y = -3; y < 3; y++) {
			for (z = -3; z < 3; z++) {
				Sphere.Add(FVector(x * AWorldCube::CubeScale, y * AWorldCube::CubeScale, z * AWorldCube::CubeScale));
			}
		}
	}
	this->LoadSphere = Sphere;
}

WorldSingleton* WorldSingleton::Instance() {
	if (!InstancePtr)
		InstancePtr = new WorldSingleton;
	return InstancePtr;
}

TSet<FVector> WorldSingleton::GetLoadSphere() {
	return this->LoadSphere;
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class DEX_API WorldSingleton
{
	WorldSingleton();
	static WorldSingleton* InstancePtr;
	TSet<FVector> LoadSphere;
public:
	static WorldSingleton* Instance();

	UPROPERTY()
		TMap<FString, AActor*> WorldCubes;
	UPROPERTY()
		TSet<FString> DiscCubes;
	
	TSet<FVector> GetLoadSphere();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class DEX_API FileHandler
{
public:
	FileHandler();
	~FileHandler();
	static void FileHandler::PushCubeDataToBuffer(FBufferArchive* Ar, int32, FVector);
	static bool FileHandler::SaveDataToFile(const FString, FBufferArchive*);
	static bool FileHandler::LoadGameDataFromFile(const FString, TArray<uint8>*);
	static void FileHandler::GetCubeDataFromBuffer(TArray<uint8>*, int32*, FVector*);

	static void FileHandler::SaveLoadData(FArchive&, int32&, FVector&);
	static bool FileHandler::LoadGameDataFromFile(const FString&, int32&, FVector&);
};

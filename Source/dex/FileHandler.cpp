// Fill out your copyright notice in the Description page of Project Settings.

#include "dex.h"
#include "FileHandler.h"

FileHandler::FileHandler()
{
}

FileHandler::~FileHandler()
{
}

//SaveLoadData
void FileHandler::PushCubeDataToBuffer(FBufferArchive* Ar, int32 SaveDataInt32, FVector SaveDataVector)
{
	*Ar << SaveDataInt32;
	*Ar << SaveDataVector;
}

bool FileHandler::SaveDataToFile(const FString FullFilePath, FBufferArchive* ToBinary)
{
	//note that the supplied FString must be the entire Filepath
	// 	if writing it out yourself in C++ make sure to use the \\

// 	FString SavePath = "C:\\MyProject\\MySaveDir\\mysavefile.save";

//Step 1: Variable Data -> Binary

	//No Data
	if ( (*ToBinary).Num() <= 0) return false;
	//~

	//Step 2: Binary to Hard Disk
	if (FFileHelper::SaveArrayToFile(*ToBinary, *FullFilePath))
	{
		// Free Binary Array 	
		(*ToBinary).FlushCache();
		(*ToBinary).Empty();

		//ClientMessage("Save Success!");
		return true;
	}

	// Free Binary Array 	
	(*ToBinary).FlushCache();
	(*ToBinary).Empty();

	//ClientMessage("File Could Not Be Saved!");

	return false;
}

void FileHandler::GetCubeDataFromBuffer(TArray<uint8>* TheBinaryArray, int32* GetDataInt32, FVector* GetDataVector)
{
	//~
	//		  Read the Data Retrieved by GFileManager
	//~
	TArray<uint8> BinaryArray = *TheBinaryArray;
	FMemoryReader FromBinary = FMemoryReader( BinaryArray, true ); //true, free data after done
	FromBinary.Seek(0);
	//SaveLoadData(FromBinary, NumGemsCollected, PlayerLocation, ArrayOfRotationsOfTheStars);
	FromBinary << *GetDataInt32;
	FromBinary << *GetDataVector;

	//~
	//								Clean up 
	//~
	FromBinary.FlushCache();

	// Empty & Close Buffer 
	(*TheBinaryArray).Empty();
	FromBinary.Close();
}


//I am using the sample save data from above examples as the data being loaded
bool FileHandler::LoadGameDataFromFile(const FString FullFilePath, TArray<uint8>* TheBinaryArray) {
	//Load the data array,
	// 	you do not need to pre-initialize this array,
	//		UE4 C++ is awesome and fills it 
	//		with whatever contents of file are, 
	//		and however many bytes that is
	if (!FFileHelper::LoadFileToArray(*TheBinaryArray, *FullFilePath))
	{
		//ClientMessage("FFILEHELPER:>> Invalid File");
		return false;
		//~~
	}

	//Testing
	//ClientMessage("Loaded File Size");
	//ClientMessage(FString::FromInt( (*TheBinaryArray).Num()) );

	//File Load Error
	if ( (*TheBinaryArray).Num() <= 0) return false;


	return true;
}

bool FileHandler::LoadGameDataFromFile(const FString& FullFilePath, int32& SaveDataInt32, FVector& SaveDataVector) {
	//Load the data array,
	// 	you do not need to pre-initialize this array,
	//		UE4 C++ is awesome and fills it 
	//		with whatever contents of file are, 
	//		and however many bytes that is
	TArray<uint8> TheBinaryArray;
	if (!FFileHelper::LoadFileToArray(TheBinaryArray, *FullFilePath))
	{
		//ClientMessage("FFILEHELPER:>> Invalid File");
		return false;
		//~~
	}

	//Testing
	//ClientMessage("Loaded File Size");
	//ClientMessage(FString::FromInt(TheBinaryArray.Num()));

	//File Load Error
	if (TheBinaryArray.Num() <= 0) return false;

	//~
	//		  Read the Data Retrieved by GFileManager
	//~

	FMemoryReader FromBinary = FMemoryReader(TheBinaryArray, true); //true, free data after done
	FromBinary.Seek(0);
	SaveLoadData(FromBinary, SaveDataInt32, SaveDataVector);

	//~
	//								Clean up 
	//~
	FromBinary.FlushCache();

	// Empty & Close Buffer 
	TheBinaryArray.Empty();
	FromBinary.Close();

	return true;
}

//SaveLoadData
void FileHandler::SaveLoadData(FArchive& Ar,
	int32& SaveDataInt32,
	FVector& SaveDataVector
	)
{
	Ar << SaveDataInt32;
	Ar << SaveDataVector;
}
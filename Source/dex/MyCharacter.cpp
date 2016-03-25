// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "dex.h"
#include "MyCharacter.h"
#include "WorldCube.h"
#include "GenerateWorld.h"
#include "WorldSingleton.h"
#include "FileHandler.h"

AMyCharacter::AMyCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

										  // Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Why do I have to build here?
	if (!this->BuiltTerrain)
	{
		this->BuiltTerrain = true;
		UWorld* World = GetWorld();
		GenerateWorld GenWorld;
		GenWorld.CreateMinimalLandscape(&World);

		FVector CharPosition = this->GetActorLocation();
		this->PrevLoadPos = CharPosition;
		CharPosition = GenWorld.FindNearbyBelowCube(CharPosition);

		WorldSingleton* Singleton = WorldSingleton::Instance();
		int32 x, y, z;
		for (x = -GenerateWorld::WorldXSize; x < GenerateWorld::WorldXSize; x++) {
			for (y = -GenerateWorld::WorldYSize; y < GenerateWorld::WorldYSize; y++) {
				for (z = -(GenerateWorld::WorldZSize - 1); z <= 0; z++) {
					if (World) {

						const FVector CubeLocation = FVector(
							(x * AWorldCube::CubeScale), 
							(y * AWorldCube::CubeScale) , 
							(z * AWorldCube::CubeScale)
						);

						//FVector CharPosition = FVector(CharPosition.X, CharPosition.Y, CharPosition.Z);

						if (!Singleton->GetLoadSphere().Contains(CubeLocation - CharPosition)) {
							const FRotator CubeRotation = FRotator::ZeroRotator;

							AActor** Cube = Singleton->WorldCubes.Find(CubeLocation.ToString());
							AActor* NullCube = NULL;
							GenWorld.RemoveCube(&World, &(*Cube)); 
							Singleton->WorldCubes.Remove(CubeLocation.ToString());
						}
					}
				}
			}
		}
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->FrameCount += DeltaTime;
	if (this->FrameCount > .1) {
		this->FrameCount = 0;

		GenerateWorld GenWorld;

		FVector CharPosition = this->GetActorLocation();
		FVector CharDirection = this->GetActorRotation().Vector();
		
		if ( (CharPosition - this->PrevLoadPos).Size() < AWorldCube::CubeScale) {
			return;
		}
		CharPosition = GenWorld.FindNearbyAheadFloorCube(CharPosition + CharDirection * AWorldCube::CubeScale);

		FString FullFilePath = "C:\\ProgramData\\MyCubes\\CUBE " + CharPosition.ToString() + ".bin";
		int32 GetDataInt32 = 0;
		FVector GetDataVector = FVector::ZeroVector;

		bool IsLoaded = FileHandler::LoadGameDataFromFile(FullFilePath, GetDataInt32, GetDataVector);
		if (IsLoaded) {
			UWorld* World = GetWorld();

			WorldSingleton* Singleton = WorldSingleton::Instance();
			if (!Singleton->WorldCubes.Contains(CharPosition.ToString())) {
				TSet<FVector> Vectors = Singleton->GetLoadSphere();
				for (FVector Vector : Vectors) {
					FVector CubeVector = Vector + CharPosition;
					if (Singleton->DiscCubes.Contains(CubeVector.ToString())) {
						if (!Singleton->WorldCubes.Contains(CubeVector.ToString())) {
							GenWorld.AddCube(&World, CubeVector, FRotator::ZeroRotator, GetDataInt32);
							this->PrevLoadPos = CharPosition;
						}
					}
				}
			}
		}
	}
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	InputComponent->BindAction("RemoveBelowCube", IE_Pressed, this, &AMyCharacter::RemoveBelowCube);
	InputComponent->BindAction("RemoveAheadCube", IE_Pressed, this, &AMyCharacter::RemoveAheadCube);
}


// Pawn pressed '1'
void AMyCharacter::RemoveBelowCube() {
	this->RemoveCube(AMyCharacter::DIGBELOW);
}

// Pawn pressed '2'
void AMyCharacter::RemoveAheadCube() {
	this->RemoveCube(AMyCharacter::DIGAHEAD);
}

void AMyCharacter::RemoveCube(int32 Direction)
{
	if ((Controller != NULL)) {

		UWorld* World = GetWorld();
		FVector CharPosition = this->GetActorLocation();
		FVector CharDirection = this->GetActorRotation().Vector();

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, CharDirection.ToString());
		//FRotator CubeRotation = FRotator::ZeroRotator;

		GenerateWorld GenWorld;
		WorldSingleton* Singleton = WorldSingleton::Instance();

		FVector CubeVec;
		if (Direction == AMyCharacter::DIGBELOW) {
			CubeVec = GenerateWorld::FindNearbyBelowCube(CharPosition);
		} else {
			CubeVec = GenerateWorld::FindNearbyAheadCube(CharPosition + CharDirection * AWorldCube::CubeScale);
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, CubeVec.ToString());
		if (Singleton->WorldCubes.Contains(CubeVec.ToString())) {
			AActor* CubePtr = *Singleton->WorldCubes.Find(CubeVec.ToString());
			AWorldCube* Cube = Cast<AWorldCube>(CubePtr);

			//Cube->UpdateMesh(1);
			GenWorld.RemoveCube(&World, &CubePtr);
			Singleton->WorldCubes.Remove(CubeVec.ToString());
			Singleton->DiscCubes.Remove(CubeVec.ToString());
			//GenWorld.AddCube(&World, CubeVec, CubeRotation, AWorldCube::GOLD);
		}
	}
}

void AMyCharacter::MoveForward(float Val)
{
	if ((Controller != NULL) && (Val != 0.0f)) {

		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}

void AMyCharacter::MoveRight(float Val)
{
	if ((Controller != NULL) && (Val != 0.0f)) {

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Val);
	}
}
// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS(Blueprintable)
class AMyCharacter : public ACharacter
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	TArray<AActor*> cubes;
	bool BuiltTerrain = false;
	float FrameCount = 0;
	FVector PrevLoadPos = FVector(0, 0, 0);

protected:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	UFUNCTION()
	void MoveForward(float Val);
	void MoveRight(float Val);
	void RemoveBelowCube();
	void RemoveAheadCube();
	void RemoveCube(int32 Direction);
	

public:
	AMyCharacter();
	virtual void Tick(float DeltaSeconds) override;

	const static int32 DIGBELOW = 0;
	const static int32 DIGAHEAD = 1;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};


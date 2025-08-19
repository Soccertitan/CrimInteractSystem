// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CrimInteractorInterface.generated.h"

class UCrimInteractorComponent;
// This class does not need to be modified.
UINTERFACE()
class UCrimInteractorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRIMINTERACTSYSTEM_API ICrimInteractorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Returns the InteractorComponent.
	UFUNCTION(BlueprintNativeEvent, Category = "CrimInteractSystem")
	UCrimInteractorComponent* GetCrimInteractorComponent() const;
};

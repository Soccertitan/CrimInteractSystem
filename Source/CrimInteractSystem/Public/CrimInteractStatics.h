// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CrimInteractStatics.generated.h"

class UCrimInteractorComponent;
/**
 * 
 */
UCLASS(ClassGroup = "Crim Interact System")
class CRIMINTERACTSYSTEM_API UCrimInteractStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Get the InteractorComponent if the actor implements the CrimInteractorInterface. If it does not, fallbacks to
	 * checking the actor's components.
	 */
	UFUNCTION(BlueprintPure, Category = "CrimInteractSystem", meta = (DefaultToSelf = "Actor"))
	static UCrimInteractorComponent* GetCrimInteractorComponent(const AActor* Actor);
};

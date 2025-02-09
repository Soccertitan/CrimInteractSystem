// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CrimInteractSystemUISubsystem.generated.h"

class UCrimInteractorComponent;
class UCrimVM_Interactor;

/**
 * A subsystem for working with the Interactable Viewmodel
 */
UCLASS()
class CRIMINTERACTSYSTEM_API UCrimInteractSystemUISubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Interact System|UI")
	UCrimVM_Interactor* GetOrCreateInteractorViewModel(APlayerController* PlayerController);

protected:
	UPROPERTY(Transient)
	UCrimVM_Interactor* InteractorViewModel;

	UCrimVM_Interactor* CreateInteractorViewModel(APlayerController* PlayerController);

	UCrimInteractorComponent* GetInteractorComponent(APlayerController* PlayerController) const;
};

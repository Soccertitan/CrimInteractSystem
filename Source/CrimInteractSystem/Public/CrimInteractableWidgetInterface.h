// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CrimInteractableWidgetInterface.generated.h"

class UCrimInteractableViewModel;
// This class does not need to be modified.
UINTERFACE()
class UCrimInteractableWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRIMINTERACTSYSTEM_API ICrimInteractableWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Sets the view model for a widget.
	 * @note The Widget should set it's ViewModel to Manual. As the InteractableComponent will automatically create the
	 * ViewModel and assign it to the InteractableWidgetComponent's widget that implement this interface.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetInteractableViewModel(UCrimInteractableViewModel* ViewModel);
};

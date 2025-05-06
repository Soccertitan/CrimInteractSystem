// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CrimInteractableWidgetComponent.generated.h"

/**
 * Contains a widget that is managed by the CrimInteractableComponent. The widget in this class should implement the
 * CrimInteractableWidgetInterface when an interactables value is updated.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRIMINTERACTSYSTEM_API UCrimInteractableWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UCrimInteractableWidgetComponent();
	
};

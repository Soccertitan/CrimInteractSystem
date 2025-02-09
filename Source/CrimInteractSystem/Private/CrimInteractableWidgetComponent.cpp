// Copyright Soccertitan


#include "CrimInteractableWidgetComponent.h"


UCrimInteractableWidgetComponent::UCrimInteractableWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetCollisionProfileName(FName("Trigger"));
	SetCanEverAffectNavigation(false);

	Space = EWidgetSpace::Screen;
	bDrawAtDesiredSize = true;
	SetActive(true);
	SetHiddenInGame(true);
}


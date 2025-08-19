// Copyright Soccertitan


#include "CrimInteractStatics.h"

#include "CrimInteractorComponent.h"
#include "CrimInteractorInterface.h"

UCrimInteractorComponent* UCrimInteractStatics::GetCrimInteractorComponent(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	if (Actor->Implements<UCrimInteractorInterface>())
	{
		return ICrimInteractorInterface::Execute_GetCrimInteractorComponent(Actor);
	}

	return Actor->FindComponentByClass<UCrimInteractorComponent>();
}

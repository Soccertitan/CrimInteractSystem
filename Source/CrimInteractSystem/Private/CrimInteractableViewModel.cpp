// Copyright Soccertitan


#include "CrimInteractableViewModel.h"

#include "CrimInteractableComponent.h"

void UCrimInteractableViewModel::SetInteractable(UCrimInteractableComponent* InInteractable)
{
	if (IsValid(InInteractable) && Interactable == nullptr)
	{
		Interactable = InInteractable;

		Interactable->OnInteractableActionTextUpdated.AddUObject(this, &UCrimInteractableViewModel::SetInteractableActionText);
		Interactable->OnInteractableNameTextUpdated.AddUObject(this, &UCrimInteractableViewModel::SetInteractableNameText);
		SetInteractableNameText(Interactable->GetInteractableNameText());
		SetInteractableActionText(Interactable->GetInteractableActionText());
		SetbIsInstantInteract(FMath::IsNearlyZero(Interactable->GetInteractionTime()));
		Interactable->OnBeginInteract.AddUniqueDynamic(this, &UCrimInteractableViewModel::OnBeginInteract);
		Interactable->OnEndInteract.AddUniqueDynamic(this, &UCrimInteractableViewModel::OnEndInteract);
	}
}

void UCrimInteractableViewModel::SetInteractableNameText(const FText& Text)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractableNameText, Text);
}

void UCrimInteractableViewModel::SetInteractableActionText(const FText& Text)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractableActionText, Text);
}

void UCrimInteractableViewModel::SetbIsInstantInteract(bool bInIsInstantInteract)
{
	UE_MVVM_SET_PROPERTY_VALUE(bIsInstantInteract, bInIsInstantInteract);
}

void UCrimInteractableViewModel::SetbIsInteracting(bool Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(bIsInteracting, Value);
}

float UCrimInteractableViewModel::GetInteractPercentage() const
{
	if (IsValid(Interactable))
	{
		return Interactable->GetInteractPercentage();
	}
	return 0.f;
}

float UCrimInteractableViewModel::GetRemainingInteractTime() const
{
	if (IsValid(Interactable))
	{
		float InteractionTime = Interactable->GetInteractionTime();
		return InteractionTime - InteractionTime * Interactable->GetInteractPercentage();
	}
	return 0.f;
}

void UCrimInteractableViewModel::OnBeginInteract(UCrimInteractorComponent* Interactor)
{
	SetbIsInteracting(true);
}

void UCrimInteractableViewModel::OnEndInteract(UCrimInteractorComponent* Interactor)
{
	SetbIsInteracting(false);
}

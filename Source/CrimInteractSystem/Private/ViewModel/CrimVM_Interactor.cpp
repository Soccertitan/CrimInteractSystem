// Copyright Soccertitan


#include "ViewModel/CrimVM_Interactor.h"

#include "CrimInteractableComponent.h"
#include "CrimInteractorComponent.h"


void UCrimVM_Interactor::SetInteractor(UCrimInteractorComponent* Interactor)
{
	if (Interactor == InteractorComponent)
	{
		return;
	}
	
	if (IsValid(InteractorComponent))
	{
		InteractorComponent->OnBeginInteract.RemoveAll(this);
		InteractorComponent->OnEndInteract.RemoveAll(this);
		InteractorComponent->OnInteractableUpdated.RemoveAll(this);
	}
	InteractorComponent = Interactor;

	if (IsValid(InteractorComponent))
	{
		InteractorComponent->OnBeginInteract.AddUniqueDynamic(this, &UCrimVM_Interactor::OnBeginInteract);
		InteractorComponent->OnEndInteract.AddUniqueDynamic(this, &UCrimVM_Interactor::OnEndInteract);
		InteractorComponent->OnInteractableUpdated.AddUniqueDynamic(this, &UCrimVM_Interactor::OnInteractableUpdated);
		OnInteractableUpdated(InteractorComponent->GetInteractable());
	}
}

void UCrimVM_Interactor::SetInteractableNameText(FText Text)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractableNameText, Text);
}

void UCrimVM_Interactor::SetInteractableActionText(FText Text)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractableActionText, Text);
}

void UCrimVM_Interactor::SetbIsInstantInteract(bool bInIsInstantInteract)
{
	UE_MVVM_SET_PROPERTY_VALUE(bIsInstantInteract, bInIsInstantInteract);
}

void UCrimVM_Interactor::SetbIsInteracting(bool Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(bIsInteracting, Value);
}

float UCrimVM_Interactor::GetInteractPercentage() const
{
	if (IsValid(CurrentInteractable))
	{
		return CurrentInteractable->GetInteractPercentage();
	}
	return 0.f;
}

float UCrimVM_Interactor::GetRemainingInteractTime() const
{
	if (IsValid(InteractorComponent))
	{
		return InteractorComponent->GetRemainingInteractTime();
	}
	return 0.f;
}

void UCrimVM_Interactor::OnBeginInteract(UCrimInteractableComponent* Interactable)
{
	SetbIsInteracting(true);
}

void UCrimVM_Interactor::OnEndInteract(UCrimInteractableComponent* Interactable)
{
	SetbIsInteracting(false);
}

void UCrimVM_Interactor::OnInteractableUpdated(UCrimInteractableComponent* Interactable)
{
	if (IsValid(CurrentInteractable))
	{
		CurrentInteractable->OnInteractableActionTextUpdated.RemoveAll(this);
		CurrentInteractable->OnInteractableNameTextUpdated.RemoveAll(this);
	}

	CurrentInteractable = Interactable;
	if (IsValid(CurrentInteractable))
	{
		SetInteractableActionText(CurrentInteractable->GetInteractableActionText());
		SetInteractableNameText(CurrentInteractable->GetInteractableNameText());
		
		CurrentInteractable->OnInteractableActionTextUpdated.AddWeakLambda(this, [this](const FText& Text)
		{
			SetInteractableActionText(Text);
		});
		CurrentInteractable->OnInteractableNameTextUpdated.AddWeakLambda(this, [this](const FText& Text)
		{
			SetInteractableNameText(Text);
		});
	}
}

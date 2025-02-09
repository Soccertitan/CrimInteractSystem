// Copyright Soccertitan


#include "CrimInteractableComponent.h"

#include "CrimInteractableWidgetComponent.h"
#include "CrimInteractorComponent.h"

UCrimInteractableComponent::UCrimInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCollisionProfileName(FName("Trigger"));
	SetCanEverAffectNavigation(false);

	SetActive(true);
	SetHiddenInGame(true);
}

void UCrimInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetComponents<UCrimInteractableWidgetComponent>(InteractableWidgetComponents);
}

void UCrimInteractableComponent::Deactivate()
{
	Super::Deactivate();

	for (int32 i = Interactors.Num() - 1; i >= 0; --i)
	{
		if (UCrimInteractorComponent* Interactor = Interactors[i])
		{
			EndFocus(Interactor);
			EndInteract(Interactor);
		}
	}

	Interactors.Empty();
}

void UCrimInteractableComponent::BeginFocus(UCrimInteractorComponent* Interactor)
{
	if (!IsActive() || !GetOwner() || !Interactor)
	{
		return;
	}

	OnBeginFocus.Broadcast(Interactor);

	if (Cast<APawn>(Interactor->GetOwner())->IsLocallyControlled())
	{
		for (TObjectPtr<UCrimInteractableWidgetComponent>& WidgetComp : InteractableWidgetComponents)
		{
			WidgetComp->SetHiddenInGame(false);
		}
	
		TArray<UActorComponent*> Components;
		GetOwner()->GetComponents(UPrimitiveComponent::StaticClass(), Components, false);
		for (auto& VisualComp : Components)
		{
			if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(VisualComp))
			{
				Prim->SetRenderCustomDepth(true);
			}
		}
	}
}

void UCrimInteractableComponent::EndFocus(UCrimInteractorComponent* Interactor)
{
	OnEndFocus.Broadcast(Interactor);

	if (Cast<APawn>(Interactor->GetOwner())->IsLocallyControlled())
	{
		for (auto& WidgetComp : InteractableWidgetComponents)
		{
			WidgetComp->SetHiddenInGame(true);
		}

		TArray<UActorComponent*> Components;
		GetOwner()->GetComponents(UPrimitiveComponent::StaticClass(), Components, false);
		for (auto& VisualComp : Components)
		{
			if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(VisualComp))
			{
				Prim->SetRenderCustomDepth(false);
			}
		}
	}
}

void UCrimInteractableComponent::BeginInteract(UCrimInteractorComponent* Interactor)
{
	if (CanInteract(Interactor))
	{
		Interactors.AddUnique(Interactor);
		OnBeginInteract.Broadcast(Interactor);
	}
}

void UCrimInteractableComponent::EndInteract(UCrimInteractorComponent* Interactor)
{
	Interactors.RemoveSingle(Interactor);
	OnEndInteract.Broadcast(Interactor);
}

void UCrimInteractableComponent::Interact(UCrimInteractorComponent* Interactor)
{
	if (CanInteract(Interactor))
	{
		OnInteract.Broadcast(Interactor);
	}
}

void UCrimInteractableComponent::SetInteractableNameText(const FText& NewNameText)
{
	InteractableNameText = NewNameText;
	OnInteractableNameTextUpdated.Broadcast(InteractableNameText);
}

void UCrimInteractableComponent::SetInteractableActionText(const FText& NewActionText)
{
	InteractableActionText = NewActionText;
	OnInteractableActionTextUpdated.Broadcast(InteractableActionText);
}

float UCrimInteractableComponent::GetInteractPercentage()
{
	if (Interactors.IsValidIndex(0))
	{
		if (UCrimInteractorComponent* Interactor = Interactors[0])
		{
			if (Interactor && Interactor->IsInteracting())
			{
				return 1.f - FMath::Abs(Interactor->GetRemainingInteractTime() / InteractionTime);
			}
		}
	}
	return 0.f;
}

bool UCrimInteractableComponent::CanInteract(UCrimInteractorComponent* Interactor) const
{
	bool bCanInteract = IsActive() && IsValid(GetOwner()) && IsValid(Interactor);
	if (!bAllowMultipleInteractors && bCanInteract)
	{
		if (Interactors.Num() >= 1)
		{
			bCanInteract = false;
			for (const UCrimInteractorComponent* Item : Interactors)
			{
				if (Interactor == Item)
				{
					bCanInteract = true;
					break;
				}
			}
		}
	}
	return bCanInteract;
}

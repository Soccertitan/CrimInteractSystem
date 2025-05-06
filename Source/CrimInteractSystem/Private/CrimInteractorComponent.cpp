// Copyright Soccertitan


#include "CrimInteractorComponent.h"

#include "CrimInteractableComponent.h"


UCrimInteractorComponent::UCrimInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCollisionProfileName(FName("Trigger"));
	SetCanEverAffectNavigation(false);
}

void UCrimInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UCrimInteractorComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UCrimInteractorComponent::OnEndOverlap);
}

void UCrimInteractorComponent::BeginInteract()
{
	Internal_BeginInteract(GetInteractable());
}

void UCrimInteractorComponent::EndInteract()
{
	if(!GetOwner()->HasAuthority())
	{
		Server_EndInteract();
	}

	InteractionData.bInteractHeld = false;

	GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_Interact);

	if(UCrimInteractableComponent* Interactable = GetInteractable())
	{
		Interactable->EndInteract(this);
	}
}

const TArray<UCrimInteractableComponent*>& UCrimInteractorComponent::GetInteractables() const
{
	return InteractableComponents;
}

void UCrimInteractorComponent::SetInteractable(UCrimInteractableComponent* Interactable)
{
	if (!InteractableComponents.Contains(Interactable))
	{
		return;
	}

	FoundNewInteractable(Interactable);
}

bool UCrimInteractorComponent::IsInteracting() const
{
	return GetOwner()->GetWorldTimerManager().IsTimerActive(TimerHandle_Interact);
}

float UCrimInteractorComponent::GetRemainingInteractTime() const
{
	return GetOwner()->GetWorldTimerManager().GetTimerRemaining(TimerHandle_Interact);
}

void UCrimInteractorComponent::PerformInteractionCheck()
{
	if (Cast<APawn>(GetOwner())->GetController() == nullptr)
	{
		return;
	}

	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();
	if (!GetInteractable())
	{
		CouldntFindInteractable();
	}
}

void UCrimInteractorComponent::CouldntFindInteractable()
{
	//We've lost focus on an interactable. Clear the timer
	if(GetOwner()->GetWorldTimerManager().IsTimerActive(TimerHandle_Interact))
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_Interact);
	}

	//Tell the interactable we've stopped focusing on it and clear the current interactable.
	if(UCrimInteractableComponent* Interactable = GetInteractable())
	{
		Interactable->EndFocus(this);
	
		if(InteractionData.bInteractHeld)
		{
			EndInteract();
		}
	}

	InteractionData.ViewedInteractionComponent = nullptr;
}

void UCrimInteractorComponent::FoundNewInteractable(UCrimInteractableComponent* Interactable)
{
	EndInteract();

	if(UCrimInteractableComponent* OldInteractable = GetInteractable())
	{
		OldInteractable->EndFocus(this);
	}

	InteractionData.ViewedInteractionComponent = Interactable;
	Interactable->BeginFocus(this);
}

void UCrimInteractorComponent::Interact()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandle_Interact);

	if(UCrimInteractableComponent* Interactable = GetInteractable())
	{
		Interactable->Interact(this);
	}
}

void UCrimInteractorComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UCrimInteractableComponent* InteractableComponent = Cast<UCrimInteractableComponent>(OtherComp))
	{
		InteractableComponents.AddUnique(InteractableComponent);
		if (!GetInteractable())
		{
			FoundNewInteractable(InteractableComponent);
		}
	}
}

void UCrimInteractorComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UCrimInteractableComponent* InteractableComponent = Cast<UCrimInteractableComponent>(OtherComp))
	{
		InteractableComponents.Remove(InteractableComponent);
		if (InteractableComponent == GetInteractable())
		{
			CouldntFindInteractable();
		}
		if (InteractableComponents.IsValidIndex(0))
		{
			FoundNewInteractable(InteractableComponents[0]);
		}
	}
}

void UCrimInteractorComponent::Internal_BeginInteract(UCrimInteractableComponent* Interactable)
{
	if(!GetOwner()->HasAuthority())
	{
		Server_Internal_BeginInteract(Interactable);
	}

	if (Interactable != GetInteractable())
	{
		SetInteractable(Interactable);
	}

	/**
	 * As an optimization, the server only checks that we're looking at an item once we begin interacting with it.
	 * This saves the server doing a check every tick for an interactable Item. The exception is a non-instant interact.
	 * In this case, the server will check every tick for the duration of the interact
	 */
	if (GetOwner()->HasAuthority())
	{
		PerformInteractionCheck();
	}

	InteractionData.bInteractHeld = true;

	if (IsValid(GetInteractable()))
	{
		GetInteractable()->BeginInteract(this);

		if (FMath::IsNearlyZero(GetInteractable()->GetInteractionTime()))
		{
			Interact();
		}
		else
		{
			GetOwner()->GetWorldTimerManager().SetTimer(
				TimerHandle_Interact,
				this,
				&UCrimInteractorComponent::Interact,
				GetInteractable()->GetInteractionTime(),
				false
			);
		}
	}
}

void UCrimInteractorComponent::Server_Internal_BeginInteract_Implementation(UCrimInteractableComponent* Interactable)
{
	Internal_BeginInteract(Interactable);
}

void UCrimInteractorComponent::Server_EndInteract_Implementation()
{
	EndInteract();
}

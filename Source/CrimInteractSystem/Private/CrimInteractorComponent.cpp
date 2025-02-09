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
	if(!GetOwner()->HasAuthority())
	{
		Server_BeginInteract();
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

	if (UCrimInteractableComponent* Interactable = GetInteractable())
	{
		Interactable->BeginInteract(this);

		if (FMath::IsNearlyZero(Interactable->GetInteractionTime()))
		{
			Interact();
		}
		else
		{
			GetOwner()->GetWorldTimerManager().SetTimer(
				TimerHandle_Interact,
				this,
				&UCrimInteractorComponent::Interact,
				Interactable->GetInteractionTime(),
				false
			);
		}
	}
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
	OnInteractableUpdated.Broadcast(nullptr);
}

void UCrimInteractorComponent::FoundNewInteractable(UCrimInteractableComponent* Interactable)
{
	EndInteract();

	if(UCrimInteractableComponent* OldInteractable = GetInteractable())
	{
		OldInteractable->EndFocus(this);
	}

	if (IsValid(InteractionData.ViewedInteractionComponent))
	{
		InteractionData.ViewedInteractionComponent->OnInteract.RemoveAll(this);
		InteractionData.ViewedInteractionComponent->OnBeginInteract.RemoveAll(this);
		InteractionData.ViewedInteractionComponent->OnEndInteract.RemoveAll(this);
		InteractionData.ViewedInteractionComponent->OnBeginFocus.RemoveAll(this);
		InteractionData.ViewedInteractionComponent->OnEndFocus.RemoveAll(this);
	}

	Interactable->OnInteract.AddUniqueDynamic(this, &UCrimInteractorComponent::OnInteractWithInteractable);
	Interactable->OnBeginInteract.AddUniqueDynamic(this, &UCrimInteractorComponent::OnBeginInteractWithInteractable);
	Interactable->OnEndInteract.AddUniqueDynamic(this, &UCrimInteractorComponent::OnEndInteractWithInteractable);
	Interactable->OnBeginFocus.AddUniqueDynamic(this, &UCrimInteractorComponent::OnBeginFocusWithInteractable);
	Interactable->OnEndFocus.AddUniqueDynamic(this, &UCrimInteractorComponent::OnEndFocusWithInteractable);

	InteractionData.ViewedInteractionComponent = Interactable;
	OnInteractableUpdated.Broadcast(Interactable);
	Interactable->BeginFocus(this);
}

void UCrimInteractorComponent::Server_BeginInteract_Implementation()
{
	BeginInteract();
}

void UCrimInteractorComponent::Server_EndInteract_Implementation()
{
	EndInteract();
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

void UCrimInteractorComponent::OnBeginInteractWithInteractable(UCrimInteractorComponent* Interactor)
{
	if (Interactor == this)
	{
		OnBeginInteract.Broadcast(InteractionData.ViewedInteractionComponent);
	}
}

void UCrimInteractorComponent::OnEndInteractWithInteractable(UCrimInteractorComponent* Interactor)
{
	if (Interactor == this)
	{
		OnEndInteract.Broadcast(InteractionData.ViewedInteractionComponent);
	}
}

void UCrimInteractorComponent::OnBeginFocusWithInteractable(UCrimInteractorComponent* Interactor)
{
	if (Interactor == this)
	{
		OnBeginFocus.Broadcast(InteractionData.ViewedInteractionComponent);
	}
}

void UCrimInteractorComponent::OnEndFocusWithInteractable(UCrimInteractorComponent* Interactor)
{
	if (Interactor == this)
	{
		OnEndFocus.Broadcast(InteractionData.ViewedInteractionComponent);
	}
}

void UCrimInteractorComponent::OnInteractWithInteractable(UCrimInteractorComponent* Interactor)
{
	if (Interactor == this)
	{
		OnInteract.Broadcast(InteractionData.ViewedInteractionComponent);
	}
}

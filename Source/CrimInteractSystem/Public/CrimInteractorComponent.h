// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CrimInteractorComponent.generated.h"

class UCrimInteractableComponent;

USTRUCT()
struct CRIMINTERACTSYSTEM_API FInteractionData
{
	GENERATED_BODY()

	FInteractionData()
	{
		ViewedInteractionComponent = nullptr;
		LastInteractionCheckTime = 0.f;
		bInteractHeld = false;
	}

	//The current interactable component we're viewing, if there is one
	UPROPERTY()
	TObjectPtr<UCrimInteractableComponent> ViewedInteractionComponent;

	//The time when we last checked for an interactable
	UPROPERTY()
	float LastInteractionCheckTime;

	//Whether the local player is holding the interact key.
	UPROPERTY()
	bool bInteractHeld;
};

/**
 * Grants the ability to interact with InteractableComponents.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRIMINTERACTSYSTEM_API UCrimInteractorComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UCrimInteractorComponent();

	/** Begins interacting with the current interactable. */
	UFUNCTION(BlueprintCallable, Category = "CrimInteractSystem")
	void BeginInteract();
	/** Ends interacting with the current interactable. */
	UFUNCTION(BlueprintCallable, Category = "CrimInteractSystem")
	void EndInteract();

	/**
	 * @return A const reference to all the interactable components.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "CrimInteractSystem")
	const TArray<UCrimInteractableComponent*>& GetInteractables() const;

	/**
	 * Tries to set the current interactable to the passed in interactable. If the Interactable is not in the existing
	 * array of InteractableComponents nothing happens.
	 * @param Interactable The interactable you want to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "CrimInteractSystem")
	void SetInteractable(UCrimInteractableComponent* Interactable);

	//Helper function to make grabbing the current Interactable faster
	FORCEINLINE UCrimInteractableComponent* GetInteractable() const {return InteractionData.ViewedInteractionComponent;}
	
	/**
	 * True if we're interacting with an item that has an interaction time.
	 * For example a light that takes 2 seconds to turn on.
	 */
	bool IsInteracting() const;

	// Gets the time till we interact with the current interactable
	float GetRemainingInteractTime() const;

protected:
	virtual void BeginPlay() override;

	//Information about the current state of the player's interaction
	UPROPERTY()
	FInteractionData InteractionData;
	
	UPROPERTY()
	TArray<TObjectPtr<UCrimInteractableComponent>> InteractableComponents;
	
	FTimerHandle TimerHandle_Interact;

	void PerformInteractionCheck();
	void CouldntFindInteractable();
	void FoundNewInteractable(UCrimInteractableComponent* Interactable);

	void Interact();
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	
	void Internal_BeginInteract(UCrimInteractableComponent* Interactable);

	UFUNCTION(Server, Reliable)
	void Server_Internal_BeginInteract(UCrimInteractableComponent* Interactable);
	UFUNCTION(Server, Reliable)
	void Server_EndInteract();
};

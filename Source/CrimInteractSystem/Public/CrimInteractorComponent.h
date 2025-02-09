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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractorDelegate, UCrimInteractableComponent*, Interactable);

/**
 * Grants the ability to interact with InteractableComponents.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRIMINTERACTSYSTEM_API UCrimInteractorComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UCrimInteractorComponent();

	UFUNCTION(BlueprintCallable)
	void BeginInteract();
	UFUNCTION(BlueprintCallable)
	void EndInteract();

	//Helper function to make grabbing the current Interactable faster
	FORCEINLINE UCrimInteractableComponent* GetInteractable() const {return InteractionData.ViewedInteractionComponent;}
	
	/**
	 * True if we're interacting with an item that has an interaction time.
	 * For example a light that takes 2 seconds to turn on.
	 */
	bool IsInteracting() const;

	// Gets the time till we interact with the current interactable
	float GetRemainingInteractTime() const;

	//[local + server] Called when the player presses the interact key whilst focusing on this interactable actor.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractorDelegate OnBeginInteract;

	//[local + server] Called when the player releases the interact key, stops looking at the interactable actor, or gets too far away after starting an interaction.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractorDelegate OnEndInteract;

	//[local + server] Called when the player presses the interact key whilst focusing on this interactable actor.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractorDelegate OnBeginFocus;

	//[local + server] Called when the player releases the interact key, stops looking at the interactable actor, or gets too far away after starting an interaction.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractorDelegate OnEndFocus;

	//[local + server] Called when the player has interacted with the item for the required amount of time.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractorDelegate OnInteract;

	//[local + server] Called when our current Interactable changes.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractorDelegate OnInteractableUpdated;

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

	UFUNCTION(Server, Reliable, Category = "Interaction")
	void Server_BeginInteract();
	UFUNCTION(Server, Reliable, Category = "Interaction")
	void Server_EndInteract();

	void Interact();
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	/**
	 * Allows the Interactor to respond to interacting with an interactable. 
	 */
	
	UFUNCTION()
	void OnBeginInteractWithInteractable(UCrimInteractorComponent* Interactor);
	UFUNCTION()
	void OnEndInteractWithInteractable(UCrimInteractorComponent* Interactor);
	UFUNCTION()
	void OnBeginFocusWithInteractable(UCrimInteractorComponent* Interactor);
	UFUNCTION()
	void OnEndFocusWithInteractable(UCrimInteractorComponent* Interactor);
	UFUNCTION()
	void OnInteractWithInteractable(UCrimInteractorComponent* Interactor);
};

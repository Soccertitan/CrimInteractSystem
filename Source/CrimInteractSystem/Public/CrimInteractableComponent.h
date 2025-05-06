// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "CrimInteractableViewModel.h"
#include "Components/BoxComponent.h"
#include "CrimInteractableComponent.generated.h"

class UCrimInteractableViewModel;
class UCrimInteractableWidgetComponent;
class UCrimInteractorComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableDelegate, UCrimInteractorComponent*, Interactor);
DECLARE_MULTICAST_DELEGATE_OneParam(FInteractableTextDelegate, const FText&);

/**
 * Allows an Interactor to interact with this component.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRIMINTERACTSYSTEM_API UCrimInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UCrimInteractableComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// Called when the players interaction check begins hitting this component.
	void BeginFocus(UCrimInteractorComponent* Interactor);
	// Called when the players interaction check ends hitting this component.
	void EndFocus(UCrimInteractorComponent* Interactor);

	// Called when the player begins interacting with this component.
	void BeginInteract(UCrimInteractorComponent* Interactor);
	// Called when the player ends interacting with this component.
	void EndInteract(UCrimInteractorComponent* Interactor);

	// Called when the player has interacted with the item for the required amount of time
	void Interact(UCrimInteractorComponent* Interactor);

	/**
	 * Updates the Interactable's Name text.
	 */
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void SetInteractableNameText(const FText& NewNameText);
	UFUNCTION(BlueprintPure, Category = "Interactable")
	FText GetInteractableNameText() const { return InteractableNameText; }

	/**
	 * Updates the Interactable's action text. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void SetInteractableActionText(const FText& NewActionText);
	UFUNCTION(BlueprintPure, Category = "Interactable")
	FText GetInteractableActionText() const { return InteractableActionText; }
	
	/**
	 * Return a value from 0-1 denoting how far through the interaction we are.
	 * On server this is the first Interactor's percentage, on client this is the local Interactor's percentage.
	 */
	UFUNCTION(BlueprintPure, Category = "Interactable")
	float GetInteractPercentage();

	/**
	 * Gets the required interaction time before OnInteract is called.
	 */
	UFUNCTION(BlueprintPure, Category = "Interactable")
	float GetInteractionTime() const { return InteractionTime; }

	/**
	 * Returns true if the Interactor can interact with this.
	 */
	bool CanInteract(UCrimInteractorComponent* Interactor) const;

	//[local + server] Called when the player presses the interact key whilst focusing on this interactable actor.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractableDelegate OnBeginInteract;

	//[local + server] Called when the player releases the interact key, stops looking at the interactable actor, or gets too far away after starting an interaction.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractableDelegate OnEndInteract;

	//[local + server] Called when the player presses the interact key whilst focusing on this interactable actor.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractableDelegate OnBeginFocus;

	//[local + server] Called when the player releases the interact key, stops looking at the interactable actor, or gets too far away after starting an interaction.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractableDelegate OnEndFocus;

	//[local + server] Called when the player has interacted with the item for the required amount of time.
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInteractableDelegate OnInteract;
	
	FInteractableTextDelegate OnInteractableNameTextUpdated;
	FInteractableTextDelegate OnInteractableActionTextUpdated;

protected:
	virtual void BeginPlay() override;
	virtual void Deactivate() override;

	//The time the player must hold the interact key to interact with this object.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable", meta = (ClampMin = 0))
	float InteractionTime = 0.f;

	//The name that will come up when the player looks at the interactable
	UPROPERTY(ReplicatedUsing = OnRep_InteractableNameText, EditAnywhere, BlueprintReadOnly, Category = "Interactable")
	FText InteractableNameText = FText::FromString("Interactable");

	//The verb that describes how the interaction works. ie "Sit" for a chair
	UPROPERTY(ReplicatedUsing = OnRep_InteractableActionText, EditAnywhere, BlueprintReadOnly, Category = "Interactable")
	FText InteractableActionText = FText::FromString("Interact");

	//Whether we allow multiple players to interact with the item, or just one at any given time.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable")
	bool bAllowMultipleInteractors = true;

	/** The view model class to spawn. */
	UPROPERTY(EditAnywhere, NoClear)
	TSubclassOf<UCrimInteractableViewModel> InteractableViewModelClass = UCrimInteractableViewModel::StaticClass();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCrimInteractableViewModel> InteractableViewModel;

	UFUNCTION()
	virtual void OnRep_InteractableNameText();
	UFUNCTION()
	virtual void OnRep_InteractableActionText();
	
private:

	/**
	 *	On the server, this will hold all Interactors. On the local player, this will just hold the local
	 *	player (provided they are an Interactor).
	 */
	UPROPERTY()
	TArray<TObjectPtr<UCrimInteractorComponent>> Interactors;
	
	UPROPERTY()
	TArray<TObjectPtr<UCrimInteractableWidgetComponent>> InteractableWidgetComponents;
};

// Copyright Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "CrimInteractableViewModel.generated.h"

class UCrimInteractorComponent;
class UCrimInteractableComponent;
/**
 * The view model for an interactable
 */
UCLASS()
class CRIMINTERACTSYSTEM_API UCrimInteractableViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void SetInteractable(UCrimInteractableComponent* InInteractable);

	FText GetInteractableNameText() const { return InteractableNameText; }
	void SetInteractableNameText(const FText& Text);
	FText GetInteractableActionText() const { return InteractableActionText; }
	void SetInteractableActionText(const FText& Text);
	bool GetbIsInstantInteract() const { return bIsInstantInteract; }
	void SetbIsInstantInteract(bool bInIsInstantInteract);
	void SetbIsInteracting(bool Value);
	bool GetbIsInteracting() const { return bIsInteracting; }

	// Return a value from 0-1 denoting how far through the interaction we are.
	UFUNCTION(BlueprintPure)
	float GetInteractPercentage() const;
	UFUNCTION(BlueprintPure)
	float GetRemainingInteractTime() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	FText InteractableNameText = FText();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	FText InteractableActionText = FText();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	bool bIsInstantInteract = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	bool bIsInteracting = false;

	UFUNCTION()
	void OnBeginInteract(UCrimInteractorComponent* Interactor);
	UFUNCTION()
	void OnEndInteract(UCrimInteractorComponent* Interactor);

	UPROPERTY()
	TObjectPtr<UCrimInteractableComponent> Interactable;
};

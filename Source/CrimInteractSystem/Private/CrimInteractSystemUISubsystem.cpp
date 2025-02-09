// Copyright Soccertitan


#include "CrimInteractSystemUISubsystem.h"

#include "CrimInteractorComponent.h"
#include "ViewModel/CrimVM_Interactor.h"

UCrimVM_Interactor* UCrimInteractSystemUISubsystem::GetOrCreateInteractorViewModel(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return nullptr;
	}

	if (InteractorViewModel)
	{
		InteractorViewModel->SetInteractor(GetInteractorComponent(PlayerController));
		return InteractorViewModel;
	}

	UCrimVM_Interactor* NewViewModel = CreateInteractorViewModel(PlayerController);
	if (NewViewModel)
	{
		InteractorViewModel = NewViewModel;
	}

	return InteractorViewModel;
}

UCrimVM_Interactor* UCrimInteractSystemUISubsystem::CreateInteractorViewModel(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return nullptr;
	}

	UCrimVM_Interactor* NewViewModel = NewObject<UCrimVM_Interactor>(this);
	NewViewModel->SetInteractor(GetInteractorComponent(PlayerController));
	return NewViewModel;
}

UCrimInteractorComponent* UCrimInteractSystemUISubsystem::GetInteractorComponent(APlayerController* PlayerController) const
{
	if (APawn* Pawn = PlayerController->GetPawn())
	{
		return Pawn->FindComponentByClass<UCrimInteractorComponent>();
	}
	return nullptr;
}

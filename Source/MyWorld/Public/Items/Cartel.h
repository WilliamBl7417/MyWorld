// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/InteractItem.h"
#include "Cartel.generated.h"

/**
 * 
 */
UCLASS()
class MYWORLD_API ACartel : public AInteractItem
{
	GENERATED_BODY()

public:
	// UPROPERTY hace visible la variable al sistema de reflexión de Unreal
	// EditAnywhere: Permite editarlo tanto en el Blueprint como en la instancia del nivel.
	// BlueprintReadWrite: Permite que los Blueprints accedan y modifiquen estos datos.
	// Category: Organiza la variable bajo un título en el editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuracion Cartel")
	TArray<FText> ListaDeTextos;
};

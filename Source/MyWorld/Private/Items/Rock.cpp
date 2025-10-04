// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Rock.h"

ARock::ARock()
{

}

void ARock::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (StaticMesh != nullptr)
	{
		StaticMeshComp->SetStaticMesh(StaticMesh);
		ShadowMeshComp->SetStaticMesh(StaticMesh);

		//FVector BaseScale = StaticMeshScale;
		//FVector NewScale = BaseScale * StaticMeshScaleMultiplier;

		//StaticMeshComp->SetWorldScale3D(NewScale);

		if (RockMaterial != nullptr)
		{
			UMaterialInstanceDynamic* DynamicMaterial = StaticMeshComp->CreateDynamicMaterialInstance(ElementIndex, RockMaterial);

			if (DynamicMaterial != nullptr)
			{

				FVector4 Value;
				Value.X = FMath::FRandRange(MaterialParameterValueMin, MaterialParameterValueMax);
				Value.Y = FMath::FRandRange(MaterialParameterValueMin, MaterialParameterValueMax);
				Value.Z = FMath::FRandRange(MaterialParameterValueMin, MaterialParameterValueMax);
				Value.W = FMath::FRandRange(MaterialParameterValueMin, MaterialParameterValueMax);

				DynamicMaterial->SetVectorParameterValue(MaterialParameterName, Value);
			}
		}

		
		

	}
	DebugMes(1, FString::Printf(TEXT("StaticMesh assigned in %s"), *GetName()), FColor::Green, 5.f);

}


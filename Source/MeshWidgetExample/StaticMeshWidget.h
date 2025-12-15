#pragma once

#include "Components/Widget.h"

#include "StaticMeshWidget.generated.h"

class USlateVectorArtData;

UCLASS(minimalapi)
class UStaticMeshWidget : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle")
	USlateVectorArtData* TrailMeshAsset;

public:
	UStaticMeshWidget();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

private:
	TSharedPtr<class SStaticMeshWidget> MyMesh;

	int32 TrailMeshId;

	friend class SStaticMeshWidget;

private:

	void SlateMeshToSlateRenderData(const USlateVectorArtData& DataSource, TArray<FSlateVertex>& OutSlateVerts, TArray<SlateIndex>& OutIndexes);
	TArray<FSlateVertex> VertexData;
	TArray<SlateIndex> IndexData;
	UMaterialInterface* MaterialFromMesh;

	FVector2D NewLocation;
	FVector2D NewScale;
	FRotator NewRotation;

	void RenderUI(
		class SStaticMeshWidget* Widget,
		const float& ScaleFactor,
		const FVector2D& ParentTopLeft,
		const FVector2D& Location,
		const FVector2D& Scale
	);
};
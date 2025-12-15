// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.


#include "StaticMeshWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SMeshWidget.h"
#include "SlateMaterialBrush.h"
#include "Slate/SlateVectorArtInstanceData.h"
#include "Slate/SlateVectorArtData.h"

class SStaticMeshWidget : public SMeshWidget
{
public:
	SLATE_BEGIN_ARGS(SStaticMeshWidget) {
		}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& Args, UStaticMeshWidget& InThis)
	{
		widgit = &InThis;
	}

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override
	{
		const FSlateLayoutTransform LayoutTransform = AllottedGeometry.GetAccumulatedLayoutTransform();

		const float LayoutScale = LayoutTransform.GetScale();

		const FVector2D ParentTopLeft = FVector2D(MyClippingRect.Left, MyClippingRect.Top);

		const FSlateRenderTransform SlateRenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();

		const FVector2D Location2D = (AllottedGeometry.GetAbsolutePositionAtCoordinates(FVector2D(0.5f, 0.5f)) - ParentTopLeft) / LayoutScale;

		// MeshUIis 100cm * 100cm�Cso take care when producing in Maya
		const FVector2D Scale2D = AllottedGeometry.Size / 100.0f;

		widgit->RenderUI(const_cast<SStaticMeshWidget*>(this), LayoutScale, ParentTopLeft, Location2D, Scale2D);

		return SMeshWidget::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	}

public:
	UStaticMeshWidget* widgit;

	void ClearRenderData()
	{
		RenderData.Reset();
	}

	void AddRenderData(UMaterialInterface* Material, uint32 NumVertices, uint32 NumIndices, FSlateVertex** Vertices, SlateIndex** Indices)
	{
		if (Material == nullptr) return;

		FRenderData& NewRenderData = RenderData[RenderData.Add(FRenderData())];

		NewRenderData.Brush = MakeShareable(new FSlateMaterialBrush(*Material, FVector2D(64, 64)));
		NewRenderData.RenderingResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*NewRenderData.Brush);

		NewRenderData.VertexData.Empty();
		NewRenderData.VertexData.AddUninitialized(NumVertices);
		*Vertices = &NewRenderData.VertexData[0];
		NewRenderData.IndexData.Empty();
		NewRenderData.IndexData.AddUninitialized(NumIndices);
		*Indices = &NewRenderData.IndexData[0];
	}
};

UStaticMeshWidget::UStaticMeshWidget()
	: TrailMeshId(-1)
{
}

void UStaticMeshWidget::RenderUI(
	class SStaticMeshWidget* Widget,
	const float& ScaleFactor,
	const FVector2D& ParentTopLeft,
	const FVector2D& Location,
	const FVector2D& Scale
)
{
	Widget->ClearRenderData();

	FSlateVertex* VertexBuffer;
	SlateIndex* IndexBuffer;
	Widget->AddRenderData(MaterialFromMesh, VertexData.Num(), IndexData.Num(), &VertexBuffer, &IndexBuffer);

	FMemory::Memcpy(VertexBuffer, VertexData.GetData(), VertexData.Num() * sizeof(FSlateVertex));
	FMemory::Memcpy(IndexBuffer, IndexData.GetData(), IndexData.Num() * sizeof(SlateIndex));

	for (uint32 i = 0; i < (uint32)VertexData.Num(); i++) {
		FVector2D Pos = FVector2D(VertexBuffer[i].Position) * ScaleFactor;
		Pos *= FVector2D(Scale.X, Scale.Y);
		Pos += ParentTopLeft;
		Pos += FVector2D(Location.X, Location.Y) * ScaleFactor;

		VertexBuffer[i].Position = FVector2f(Pos);
	}
}

void UStaticMeshWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (TrailMeshAsset) {
		MaterialFromMesh = TrailMeshAsset->GetMaterial();
		SlateMeshToSlateRenderData(*TrailMeshAsset, VertexData, IndexData);
	}
}

/** Populate OutSlateVerts and OutIndexes with data from this static mesh such that Slate can render it. */
void UStaticMeshWidget::SlateMeshToSlateRenderData(const USlateVectorArtData& DataSource, TArray<FSlateVertex>& OutSlateVerts, TArray<SlateIndex>& OutIndexes)
{
	// Populate Index data
	{
		// Note that we do a slow copy because on some platforms the SlateIndex is
		// a 16-bit value, so we cannot do a memcopy.
		const TArray<uint32>& IndexDataSource = DataSource.GetIndexData();
		const int32 NumIndexes = IndexDataSource.Num();
		OutIndexes.Empty();
		OutIndexes.Reserve(NumIndexes);
		for (int32 i = 0; i < NumIndexes; ++i) {
			OutIndexes.Add(IndexDataSource[i]);
		}
	}

	// Populate Vertex Data
	{
		const TArray<FSlateMeshVertex> VertexDataSource = DataSource.GetVertexData();
		const uint32 NumVerts = VertexDataSource.Num();
		OutSlateVerts.Empty();
		OutSlateVerts.Reserve(NumVerts);

		for (uint32 i = 0; i < NumVerts; ++i) {
			const FSlateMeshVertex& SourceVertex = VertexDataSource[i];
			FSlateVertex& NewVert = OutSlateVerts[OutSlateVerts.AddUninitialized()];

			// Copy Position
			{
				NewVert.Position[0] = SourceVertex.Position.X;
				NewVert.Position[1] = SourceVertex.Position.Y;
			}

			// Copy Color
			{
				NewVert.Color = SourceVertex.Color;
			}

			// Copy all the UVs that we have, and as many as we can fit.
			{
				NewVert.TexCoords[0] = SourceVertex.UV0.X;
				NewVert.TexCoords[1] = SourceVertex.UV0.Y;

				NewVert.TexCoords[2] = SourceVertex.UV1.X;
				NewVert.TexCoords[3] = SourceVertex.UV1.Y;

				NewVert.MaterialTexCoords[0] = SourceVertex.UV2.X;
				NewVert.MaterialTexCoords[1] = SourceVertex.UV2.Y;
			}
		}
	}
}

void UStaticMeshWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyMesh.Reset();
}

TSharedRef<SWidget> UStaticMeshWidget::RebuildWidget()
{
	MyMesh = SNew(SStaticMeshWidget, *this);
	return MyMesh.ToSharedRef();
}

#if WITH_EDITOR
const FText UStaticMeshWidget::GetPaletteCategory()
{
	return NSLOCTEXT("UParticleWidget", "Palette Category", "Particle Effects");
}
#endif
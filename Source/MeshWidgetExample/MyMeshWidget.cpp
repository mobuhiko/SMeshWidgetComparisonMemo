// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.


#include "MyMeshWidget.h"
#include "MeshWidgetExample.h"
#include "Slate/SMeshWidget.h"
#include "Slate/SlateVectorArtInstanceData.h"

class SMyMeshWidget : public SMeshWidget
{
public:
	SLATE_BEGIN_ARGS(SMyMeshWidget) { }
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& Args, UMyMeshWidget& InThis)
	{
		This = &InThis;
	}

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override
	{
		const float Scale = AllottedGeometry.Scale;

		int32 MeshID = This->TrailMeshId;
		if (MeshID != INDEX_NONE) {
			FVector2D WindowSpacePos = AllottedGeometry.LocalToAbsolute(AllottedGeometry.GetLocalSize() * 0.5f);

			FSlateInstanceBufferData PerInstaceUpdate;
			int32 Size = PerInstaceUpdate.GetTypeSize();
			FSlateVectorArtInstanceData ArtInstData;
			ArtInstData.SetPosition(WindowSpacePos);
			ArtInstData.SetScale(Scale);
			PerInstaceUpdate.Add(TArray<UE::Math::TVector4<float>>::ElementType(ArtInstData.GetData()));

			const_cast<SMyMeshWidget*>(this)->UpdatePerInstanceBuffer(MeshID, PerInstaceUpdate);
		}

		return SMeshWidget::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	}

public:
	UMyMeshWidget* This;
};

UMyMeshWidget::UMyMeshWidget()
	: TrailMeshId(-1)
{
}

void UMyMeshWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyMesh.IsValid() && TrailMeshAsset != nullptr)
	{
		TrailMeshId = MyMesh->AddMesh(*TrailMeshAsset);
	}
}

void UMyMeshWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyMesh.Reset();
}

TSharedRef<SWidget> UMyMeshWidget::RebuildWidget()
{
	MyMesh = SNew(SMyMeshWidget, *this);
	return MyMesh.ToSharedRef();
}

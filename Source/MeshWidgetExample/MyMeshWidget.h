// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Components/Widget.h"

#include "MyMeshWidget.generated.h"

class USlateVectorArtData;

UCLASS(minimalapi)
class UMyMeshWidget : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Particle")
	USlateVectorArtData* TrailMeshAsset;

public:
	UMyMeshWidget();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

private:
	TSharedPtr<class SMyMeshWidget> MyMesh;

	int32 TrailMeshId;

	friend class SMyMeshWidget;
};

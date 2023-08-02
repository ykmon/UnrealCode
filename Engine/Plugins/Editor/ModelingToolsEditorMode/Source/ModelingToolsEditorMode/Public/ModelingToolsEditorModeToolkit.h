// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Toolkits/BaseToolkit.h"
#include "StatusBarSubsystem.h"
#include "UObject/GCObject.h"

#include "ModelingToolsEditorModeToolkit.generated.h"

class IAssetViewport;
class SEditableTextBox;
class STextComboBox;
namespace ETextCommit { enum Type : int; }

#define LOCTEXT_NAMESPACE "ModelingToolsEditorModeToolkit"

class STransformGizmoNumericalUIOverlay;
class IDetailsView;
class SButton;
class STextBlock;
class UInteractiveToolsPresetCollectionAsset;

UCLASS()
class UPresetSettingsProperties : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Presets", meta=(MetaClass = "InteractiveToolsPresetCollectionAsset"))
	TArray< FSoftObjectPath > ActivePresetCollectionsPaths;
};

struct FToolPresetOption
{
	FString PresetLabel;
	FString PresetTooltip;
	FSlateIcon PresetIcon;
	FString PresetName;
	FSoftObjectPath PresetCollection;
};

class FModelingToolsEditorModeToolkit : public FModeToolkit, public FGCObject
{
public:

	FModelingToolsEditorModeToolkit();
	~FModelingToolsEditorModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override;

	// initialize toolkit widgets that need to wait until mode is initialized/entered
	virtual void InitializeAfterModeSetup();

	// set/clear notification message area
	virtual void PostNotification(const FText& Message);
	virtual void ClearNotification();

	// set/clear warning message area
	virtual void PostWarning(const FText& Message);
	virtual void ClearWarning();

	/** Returns the Mode specific tabs in the mode toolbar **/ 
	virtual void GetToolPaletteNames(TArray<FName>& InPaletteName) const;
	virtual FText GetToolPaletteDisplayName(FName PaletteName) const; 
	virtual void BuildToolPalette(FName PaletteName, class FToolBarBuilder& ToolbarBuilder);
	virtual void OnToolPaletteChanged(FName PaletteName) override;
	virtual bool HasIntegratedToolPalettes() const { return false; }
	virtual bool HasExclusiveToolPalettes() const { return false; }

	virtual FText GetActiveToolDisplayName() const override { return ActiveToolName; }
	virtual FText GetActiveToolMessage() const override { return ActiveToolMessage; }

	virtual void ShowRealtimeAndModeWarnings(bool bShowRealtimeWarning);

	virtual void OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool) override;
	virtual void OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool) override;

	virtual void CustomizeModeDetailsViewArgs(FDetailsViewArgs& ArgsInOut) override;

	void OnActiveViewportChanged(TSharedPtr<IAssetViewport>, TSharedPtr<IAssetViewport> );

	virtual void InvokeUI() override;

	// Read and write Autogen Subfolder quick setting in restrictive mode.
	virtual FText GetRestrictiveModeAutoGeneratedAssetPathText() const;
	virtual void OnRestrictiveModeAutoGeneratedAssetPathTextCommitted(const FText& InNewText, ETextCommit::Type InTextCommit) const;

	virtual void ForceToolPaletteRebuild();

	// The mode is expected to call this after setting up the gizmo context object so that any subsequently
	// created gizmos are bound to the numerical UI.
	void BindGizmoNumericalUI();

	// This is exposed only for the convenience of being able to create the numerical UI submenu
	// in a non-member function in ModelingModeToolkit_Toolbars.cpp
	TSharedPtr<STransformGizmoNumericalUIOverlay> GetGizmoNumericalUIOverlayWidget() { return GizmoNumericalUIOverlayWidget; }

	/** GCObject interface */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;

private:
	const static TArray<FName> PaletteNames_Standard;

	FText ActiveToolName;
	FText ActiveToolMessage;
	FStatusBarMessageHandle ActiveToolMessageHandle;
	const FSlateBrush* ActiveToolIcon = nullptr;

	TSharedPtr<SWidget> ToolkitWidget;
	void UpdateActiveToolProperties();
	void InvalidateCachedDetailPanelState(UObject* ChangedObject);

	TSharedPtr<SWidget> ToolShutdownViewportOverlayWidget;
	void MakeToolShutdownOverlayWidget();

	TSharedPtr<SWidget> SelectionPaletteOverlayWidget;
	void MakeSelectionPaletteOverlayWidget();

	void MakeGizmoNumericalUIOverlayWidget();
	TSharedPtr<STransformGizmoNumericalUIOverlay> GizmoNumericalUIOverlayWidget;

	TSharedRef<SWidget> MakeMenu_ModelingModeConfigSettings();

	TSharedPtr<STextBlock> ModeWarningArea;
	TSharedPtr<STextBlock> ModeHeaderArea;
	TSharedPtr<STextBlock> ToolWarningArea;
	TSharedPtr<SButton> AcceptButton;
	TSharedPtr<SButton> CancelButton;
	TSharedPtr<SButton> CompletedButton;

	TSharedPtr<SWidget> MakeAssetConfigPanel();

	FText ActiveWarning{};

	TArray<TSharedPtr<FString>> AssetLocationModes;
	TArray<TSharedPtr<FString>> AssetSaveModes;
	TSharedPtr<STextComboBox> AssetLocationMode;
	TSharedPtr<STextComboBox> AssetSaveMode;
	TSharedPtr<SEditableTextBox> NewAssetPath;
	void UpdateAssetLocationMode(TSharedPtr<FString> NewString);
	void UpdateAssetSaveMode(TSharedPtr<FString> NewString);
	void UpdateAssetPanelFromSettings();
	void UpdateObjectCreationOptionsFromSettings();
	void OnAssetSettingsModified();
	FDelegateHandle AssetSettingsModifiedHandle;
	void OnShowAssetSettings();
	void SelectNewAssetPath() const;

	// Presets
	TObjectPtr<UPresetSettingsProperties> PresetSettings;
	TSharedPtr<SWidget> MakePresetPanel();
	FSoftObjectPath CurrentPreset;
	FString NewPresetLabel;
	FString NewPresetTooltip;
	FSlateIcon NewPresetIcon;

	TArray<TSharedPtr<FToolPresetOption>> AvailablePresetsForTool;
	TSharedPtr<SEditableComboBox<TSharedPtr<FString>>> PresetComboBox;

	TSharedRef<SWidget> GetPresetSettingsButtonContent();
	TSharedRef<SWidget> GetPresetCreateButtonContent();

	void CreateNewPresetInCollection(const FString& PresetLabel, FSoftObjectPath CollectionPath, const FString& ToolTip, FSlateIcon Icon);
	void LoadPresetFromCollection(const FString& PresetName, FSoftObjectPath CollectionPath);

	FString GetCurrentPresetPath() { return CurrentPreset.GetAssetPathString(); }
	void HandlePresetAssetChanged(const FAssetData& InAssetData);
	bool HandleFilterPresetAsset(const FAssetData& InAssetData);
	void SaveActivePreset();
	
	void RebuildPresetListForTool(bool bSettingsOpened);
	TSharedRef<SWidget> MakePresetComboWidget(TSharedPtr<FString> InItem);
	bool IsPresetEnabled() const;
	void ClearPresetComboList();


	TArray<TSharedPtr<FString>> AssetLODModes;
	TSharedPtr<STextBlock> AssetLODModeLabel;
	TSharedPtr<STextComboBox> AssetLODMode;

	bool bFirstInitializeAfterModeSetup = true;
	bool bShowActiveSelectionActions = true;
};

#undef LOCTEXT_NAMESPACE

#if UE_ENABLE_INCLUDE_ORDER_DEPRECATED_IN_5_2
#include "CoreMinimal.h"
#include "ModelingToolsEditorMode.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/STextComboBox.h"
#endif

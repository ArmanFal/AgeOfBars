// Copyright (c) 2026 Arman Shajiee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/StyleDefaults.h"
#include "TableWidget.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSlateRegularText
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CellText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor CellColor = FLinearColor::White;
};

USTRUCT(BlueprintType)
struct FTableRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Table")
	TArray<FSlateRegularText> Cells;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IdentifierID = 0;
};

struct FSlateTableRowData
{
	TArray<FSlateRegularText> Cells;
};


UCLASS()
class AGEOFWAR_API UTableWidget : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Table")
	TArray<FText> ColumnTitles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Table")
	TArray<FTableRow> Rows;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Table|Style")
	FSlateFontInfo CellFont = FCoreStyle::GetDefaultFontStyle("Regular", 24);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Table|Style")
	FSlateFontInfo HeaderFont = FCoreStyle::GetDefaultFontStyle("Bold", 12);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Table|Style")
	FMargin CellPadding = FMargin(4.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Table|Style")
	FMargin HeaderPadding = FMargin(4.0f);

	UFUNCTION(BlueprintCallable, Category="Table")
	void RefreshTableRows();
	
	UFUNCTION(BlueprintCallable, Category="Table")
	void RefreshTableColumns();

	UFUNCTION(BlueprintCallable, Category="Table")
	void AddTableRow(const FTableRow& NewRow);

	UFUNCTION(BlueprintCallable, Category="Table")
	void ClearTable();

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	using FRowPtr = TSharedPtr<FSlateTableRowData>;

	TArray<FRowPtr> SlateRows;
	TSharedPtr<SListView<FRowPtr>> SlateList;
	TSharedPtr<SHeaderRow> SlateHeader;

	TSharedRef<ITableRow> GenerateRow(FRowPtr Item, const TSharedRef<STableViewBase>& OwnerTable);
	
	int SortedColumnIndex = -1;
	EColumnSortMode::Type SortMode = EColumnSortMode::None;
	
	void OnSortColumn(EColumnSortPriority::Type Priority, const FName& ColumnID, EColumnSortMode::Type SortMode);
	void SortTableRow();
};

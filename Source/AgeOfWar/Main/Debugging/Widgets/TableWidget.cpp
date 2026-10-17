// Copyright (c) 2026 Arman Shajiee. All rights reserved.


#include "TableWidget.h"

#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Text/STextBlock.h"

class SGenericTableRow : public SMultiColumnTableRow<TSharedPtr<FSlateTableRowData>>
{
    
public:
    using FRowPtr = TSharedPtr<FSlateTableRowData>;

    SLATE_BEGIN_ARGS(SGenericTableRow) {}
        SLATE_ARGUMENT(FRowPtr, Item)
        SLATE_ARGUMENT(FSlateFontInfo, Font)
        SLATE_ARGUMENT(FMargin, Padding)
        SLATE_ARGUMENT(bool, ShowSelection)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTable)
    {
        Item = InArgs._Item;
        Font = InArgs._Font;
        Padding = InArgs._Padding;
        ShowSelection = InArgs._ShowSelection;
        SMultiColumnTableRow<FRowPtr>::Construct(FSuperRowType::FArguments().Padding(FMargin(4.0f, 2.0f)), OwnerTable);
    }

    virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
    {
        if (!Item.IsValid())
        {
            return SNew(STextBlock);
        }
        
        FString Prefix;
        FString IndexString;

        if (!ColumnName.ToString().Split(TEXT("_"), &Prefix, &IndexString))
        {
            return SNew(STextBlock);
        }
        
        if (Prefix != TEXT("Column") || !IndexString.IsNumeric())
        {
            return SNew(STextBlock);
        }

        const int32 Index = FCString::Atoi(*IndexString);
        if (!Item->Cells.IsValidIndex(Index))
        {
            return SNew(STextBlock);
        }
        
        return SNew(SBorder)
        .BorderImage(FStyleDefaults::GetNoBrush())
        .Padding(Padding)
        [
            SNew(STextBlock)
            .Text(Item->Cells[Index].CellText)
            .Font(Font)
            .ColorAndOpacity(Item->Cells[Index].CellColor)
        ];
    }
    
private:
    
    FRowPtr Item;
    FSlateFontInfo Font;
    FMargin Padding;
    bool ShowSelection{false};
};

TSharedRef<SWidget> UTableWidget::RebuildWidget()
{
    SlateRows.Empty();
    SlateHeader.Reset();

    for (const FTableRow& Row : Rows)
    {
        FRowPtr Item = MakeShared<FSlateTableRowData>();
        Item->Cells = Row.Cells;
        SlateRows.Add(Item);
    }

    // Dynamically construct the column headers.
    SAssignNew(SlateHeader, SHeaderRow);
    RefreshTableColumns();

    SAssignNew(SlateList, SListView<FRowPtr>)
        .ListItemsSource(&SlateRows)
        .SelectionMode(ESelectionMode::Single)
        .OnGenerateRow_UObject(this, &UTableWidget::GenerateRow)
        .HeaderRow(SlateHeader.ToSharedRef());

    return SlateList.ToSharedRef();
}



TSharedRef<ITableRow> UTableWidget::GenerateRow(FRowPtr Item, const TSharedRef<STableViewBase>& OwnerTable)
{
    return SNew(SGenericTableRow, OwnerTable)
        .Item(Item)
        .Font(CellFont)
        .Padding(CellPadding)
        ;
}

void UTableWidget::RefreshTableRows()
{
    if (SlateList.IsValid())
    {
        SlateList->ClearSelection();
    }

    SlateRows.Empty();

    for (const FTableRow& Row : Rows)
    {
        FRowPtr Item = MakeShared<FSlateTableRowData>();
        Item->Cells = Row.Cells;
        SlateRows.Add(Item);
    }

    if (SlateList.IsValid()) SlateList->RequestListRefresh();
    
}

void UTableWidget::RefreshTableColumns()
{
    if (!SlateHeader.IsValid()) return;

    SlateHeader->ClearColumns();

    for (int32 Index = 0; Index < ColumnTitles.Num(); ++Index)
    {
        const FName ColumnID(*FString::Printf(TEXT("Column_%d"), Index));

        SlateHeader->AddColumn(SHeaderRow::Column(ColumnID).FillWidth(1.0f).HeaderContent()
        [
            SNew(SBorder).BorderImage(FStyleDefaults::GetNoBrush()).Padding(HeaderPadding)
            [
                SNew(STextBlock).Text(ColumnTitles[Index]).Font(HeaderFont)
            ]
        ]);
    }

    if (SlateList.IsValid()) SlateList->RebuildList();
}

void UTableWidget::AddTableRow(const FTableRow& NewRow)
{
    Rows.Add(NewRow);
    RefreshTableRows();
}

void UTableWidget::ClearTable()
{
    Rows.Empty();
    RefreshTableRows();
}

void UTableWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    SlateList.Reset();
    SlateHeader.Reset();
}


#include "serenity_stock_selector.h"

static const float kMaxPeRatio = 20.0f;
static const float kMinRoe = 12.0f;
static const float kMinRevenueGrowth = 8.0f;
static const float kMaxDebtRatio = 60.0f;

int is_serenity_candidate(const Stock *stock)
{
    if (stock == 0) {
        return 0;
    }

    return stock->pe_ratio > 0.0f &&
           stock->pe_ratio <= kMaxPeRatio &&
           stock->roe >= kMinRoe &&
           stock->revenue_growth >= kMinRevenueGrowth &&
           stock->debt_ratio <= kMaxDebtRatio &&
           stock->has_positive_cash_flow;
}

size_t select_serenity_stocks(
    const Stock *stocks,
    size_t stock_count,
    Stock *selected_stocks,
    size_t max_selected_count)
{
    size_t stock_index = 0;
    size_t selected_count = 0;

    if (stocks == 0 || selected_stocks == 0 || max_selected_count == 0) {
        return 0;
    }

    for (stock_index = 0; stock_index < stock_count; ++stock_index) {
        if (selected_count >= max_selected_count) {
            break;
        }

        if (is_serenity_candidate(&stocks[stock_index])) {
            selected_stocks[selected_count++] = stocks[stock_index];
        }
    }

    return selected_count;
}

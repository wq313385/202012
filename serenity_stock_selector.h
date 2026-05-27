#ifndef SERENITY_STOCK_SELECTOR_H
#define SERENITY_STOCK_SELECTOR_H

#include <stddef.h>

typedef struct Stock {
    const char *code;
    const char *name;
    float pe_ratio;
    float roe;
    float revenue_growth;
    float debt_ratio;
    int has_positive_cash_flow;
} Stock;

int is_serenity_candidate(const Stock *stock);
size_t select_serenity_stocks(
    const Stock *stocks,
    size_t stock_count,
    Stock *selected_stocks,
    size_t max_selected_count);

#endif

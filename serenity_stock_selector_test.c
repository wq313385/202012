#include <assert.h>
#include <string.h>

#include "serenity_stock_selector.h"

int main(void)
{
    const Stock stocks[] = {
        {"600519", "贵州茅台", 18.6f, 31.2f, 15.4f, 24.8f, 1},
        {"300750", "宁德时代", 27.3f, 19.5f, 28.0f, 41.0f, 1},
        {"601318", "中国平安", 8.2f, 11.0f, 6.3f, 67.2f, 1},
        {"600276", "恒瑞医药", 19.7f, 14.8f, 9.1f, 18.5f, 1}
    };
    Stock selected[4];
    size_t selected_count = select_serenity_stocks(stocks, 4, selected, 4);

    assert(selected_count == 2);
    assert(strcmp(selected[0].code, "600519") == 0);
    assert(strcmp(selected[1].code, "600276") == 0);
    assert(is_serenity_candidate(&stocks[0]) == 1);
    assert(is_serenity_candidate(&stocks[1]) == 0);
    assert(select_serenity_stocks(NULL, 4, selected, 4) == 0);
    assert(select_serenity_stocks(stocks, 4, NULL, 4) == 0);
    assert(select_serenity_stocks(stocks, 4, selected, 0) == 0);

    return 0;
}

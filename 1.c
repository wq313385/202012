#include <stdio.h>

#include "serenity_stock_selector.h"

int main(void)
{
    const Stock universe[] = {
        {"600519", "贵州茅台", 18.6f, 31.2f, 15.4f, 24.8f, 1},
        {"000333", "美的集团", 14.9f, 22.7f, 10.1f, 52.4f, 1},
        {"300750", "宁德时代", 27.3f, 19.5f, 28.0f, 41.0f, 1},
        {"601318", "中国平安", 8.2f, 11.0f, 6.3f, 67.2f, 1},
        {"600276", "恒瑞医药", 19.7f, 14.8f, 9.1f, 18.5f, 1}
    };
    Stock selected[sizeof(universe) / sizeof(universe[0])];
    size_t selected_count = select_serenity_stocks(
        universe,
        sizeof(universe) / sizeof(universe[0]),
        selected,
        sizeof(selected) / sizeof(selected[0]));
    size_t index = 0;

    printf("Serenity选股结果（低估值、高盈利、稳增长、低负债、现金流为正）：\n");
    for (index = 0; index < selected_count; ++index) {
        printf("%s %s\n", selected[index].code, selected[index].name);
    }

    if (selected_count == 0) {
        printf("未筛选到符合条件的股票\n");
    }

    return 0;
}

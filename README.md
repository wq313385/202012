# 202012

## Serenity选股

仓库现在提供了一个简单的 `Serenity选股` 示例程序，用于按照以下条件筛选股票：

- 市盈率（PE）不高于 20
- 净资产收益率（ROE）不低于 12%
- 营收增速不低于 8%
- 资产负债率不高于 60%
- 经营现金流为正

### 运行示例

```sh
gcc -Wall -Wextra -std=c11 -o serenity_demo 1.c serenity_stock_selector.c
./serenity_demo
```

### 运行测试

```sh
gcc -Wall -Wextra -std=c11 -o serenity_stock_selector_test serenity_stock_selector.c serenity_stock_selector_test.c
./serenity_stock_selector_test
```

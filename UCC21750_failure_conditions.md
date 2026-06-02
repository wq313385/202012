# UCC21750 失效条件分析

## 概述

UCC21750 是 TI 推出的隔离栅极驱动器，适用于 SiC MOSFET 和 IGBT。其"失效"分为两大类：

1. **功能性失效/误动作**：芯片未损坏，但会拉故障、关断、误触发。
2. **器件损坏性失效**：芯片被永久损坏，无法正常驱动。

---

## 1. 供电异常（UVLO 触发）

UCC21750 对输入侧 **VCC** 和输出侧 **VDD** 均有 UVLO（欠压锁定）保护。

- VDD UVLO 典型阈值约 **12 V**，滞回约 **800 mV**
- VCC 或 VDD 低于阈值时，驱动输出被强制关断
- 只有两者都退出 UVLO 后，输出才允许正常拉高

**常见场景：**
- 隔离电源带载能力不足
- 开关瞬间电流过大，导致 VDD 塌陷
- 去耦电容不够大或放置距离过远
- VEE / COM 回路布局差，参考点抖动

**表现：** OUT 不再正常输出、RDY 拉低、APWM 停止工作

---

## 2. DESAT 过流/短路保护触发

DESAT 检测阈值典型约 **9 V**，用于检测 SiC MOSFET 的 VDS 或 IGBT 的 VCE 异常升高。

检测到故障后：
- 触发 **soft turn-off**（软关断）
- **FLT 引脚** 拉低
- 必须通过 **RST/EN** 复位后才能恢复

**常见触发场景：**
- 功率管真正短路或过流
- 开通太快，DESAT 检测窗口设计不合理
- DESAT 二极管、R、Cblk 选值不当
- Kelvin source/emitter 布局不佳，检测点漂移
- 高 dv/dt 将 DESAT 节点电位打飞

---

## 3. 高噪声环境下的误故障（False Fault）

TI E2E 论坛明确记录：系统开关噪声过高时，可能出现 **false FLT（假故障）**，甚至在 INP=Low / EN=Low 时也会发生。

**布局相关关键点：**
- 高电流路径需与 gate driver 区域隔离
- **DC- 与 COM 要分开，并通过星形连接**
- 板级差异/布局寄生参数是导致个别板子更容易出问题的主要原因

**常见触发场景：**
- 半桥/全桥 dv/dt 很大
- COM 参考点不干净
- FLT、RST/EN、DESAT 走线过长
- 探头接法不对，测量结果被噪声淹没
- 功率回路和驱动回路未分区布局

---

## 4. VDD 开路或输出悬空

当 VDD 开路时，OUTH/OUTL 进入高阻状态。芯片内部设有 **active pulldown**，防止输出失控或误开通。

**导致此类问题的场景：**
- 隔离电源虚焊
- VDD 走线电阻过大
- VEE / COM 接触不良
- 去耦电容焊接异常

**表现：** 栅极驱动时有时无，某一侧门极电压异常，上下桥某管更容易误动作。

---

## 5. Miller 干扰（米勒电流误开通）

UCC21750 内置 **active Miller clamp**，当门极电压低于阈值（约 VEE 上方 2 V）时，提供低阻路径抑制高 dv/dt 引起的误开通。

**仍可能出现米勒问题的场景：**
- 负压关断不足
- 外部门阻过大
- 功率环路寄生电感大
- SiC 开关速度过快
- Clamp 回路布局差

**严重后果：** 对侧器件误开通 → 交叉导通 → DESAT/过流保护动作 → 功率管和驱动器同时损坏。

---

## 6. 超绝对额定值（器件永久损坏）

| 参数 | 限制 |
|------|------|
| VDD 相对 COM | -0.3 V 至 +36 V |
| VEE 相对 COM | -17.5 V 至 +0.3 V |
| VDD - VEE | 最大 36 V |
| 结温 TJ | 最大 150°C |

**常见超限场景：**
- VDD/VEE 供电超压
- 负压关断过深，VEE 超限
- OUTH/OUTL 被外部尖峰拉出额定范围
- DESAT 脚被高压脉冲直接灌入
- 结温长期过高
- ESD/EOS 冲击

---

## 7. 隔离边界过应力

UCC21750 为电容隔离器件，主要参数：

| 参数 | 典型值 |
|------|--------|
| 工作隔离电压 | 最高约 1.5 kVrms |
| 浪涌耐受 | 约 12.8 kVpk |
| CMTI | 最低 150 V/ns |

长期超出设计余量会从"偶发误码"发展到"隔离永久退化"。

---

## 8. 输入控制异常

- IN+、IN-、RST/EN 均有约 **40 ns 去抖滤波**
- **RST/EN 悬空时默认为禁用状态**（内部有下拉）
- 复位故障需满足最小脉宽条件

**常见场景：**
- MCU 输出悬空或上电时序不对
- RST/EN 没有被可靠上拉
- PWM 脉冲太窄，被去抖滤波器滤掉
- 逻辑侧地参考不稳

**表现：** 明明有 PWM 但门极不动作，FLT 无法清除，极窄脉冲"丢失"。

---

## 现场排查顺序

1. **查 VDD、VEE、COM 波形**：开关瞬间有无塌陷/抬升
2. **查 FLT 触发前 DESAT 波形**：有无被尖峰打到 9 V 附近
3. **查布局**：DESAT 回路、Kelvin source、COM 与功率地是否分开
4. **查 RST/EN**：有无悬空、脉宽不足、被噪声串扰
5. **查门极回路**：门阻、负压、Miller clamp 回路是否合理
6. **查绝对额定值和温升**：尤其 VEE、VDD-VEE、OUTH/OUTL 尖峰和芯片壳温

---

## 参考资料

- [UCC21750 数据手册 (TI)](https://www.ti.com/lit/ds/symlink/ucc21750.pdf)
- [TI E2E 论坛 - FLT 假故障讨论](https://e2e.ti.com/support/power-management-group/power-management/f/power-management-forum/1512039/ucc21750-flt-false-trigger-when-en-is-low)
- [TI E2E 论坛 - NFLT 引脚故障条件](https://e2e.ti.com/support/power-management-group/power-management/f/power-management-forum/1565744/ucc21750-q1-nflt-pin-fault-conditions)

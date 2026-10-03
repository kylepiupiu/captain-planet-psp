# 地球超人：元素使命 — PSP 可玩初版 v0.1

2026-10-03 · 非官方同人开发版 · 中文界面 · 480×272

本包包含可运行的 PSP 原生 EBOOT.PBP，不是网页演示。提供六季 113 个剧集条目与对应任务入口，五名队员切换、五种元素、能量召唤地球超人、十种任务类型、地图、暂停、星级及自动存档。

**尚未完成用户要求的最终版。** 113 关由十类任务系统和程序化地图构建，并非 113 个独立精制场景；剧情依据公开简介改编，没有逐集观看全部动画。角色是参照动画截图制作的二维图集，不是三维建模，也尚未达到严格一比一还原。专属反派演出、完整剧情、配音和逐集专属首领尚未制作。

## 安装到 PSP

1. 解压安装包，将其中 `PSP/GAME/PLANET` 文件夹完整复制到记忆棒对应位置。
2. 确认最终路径为 `ms0:/PSP/GAME/PLANET/EBOOT.PBP`，同级有 `assets` 文件夹及三个 `.rgba` 文件。
3. 在支持自制程序的 PSP 环境中，从“游戏 → Memory Stick”启动 Captain Planet。

构建目标固件版本 6.60；尚未完成 PSP 实机兼容性与帧率测试，不能保证所有机器启动。安装不涉及修改或刷写固件。PPSSPP 可直接打开 `EBOOT.PBP`；请保留旁边的 assets 文件夹。

## 操作

| 按键 | 功能 |
| --- | --- |
| 方向键 / 摇杆 | 移动 |
| L / R | 切换夸米、惠勒、琳卡、吉、马蒂 |
| × | 确认 / 持续元素攻击 |
| ○ | 返回 / 闪避 |
| □ | 与附近目标互动 |
| △ | 能量达到 100 时召唤地球超人，持续 12 秒 |
| SELECT | 地图；时空任务中切换时相 |
| START | 暂停、重试、声音开关、退出 |

目标上方的“土、火、风、水、心”提示所需队员。完成目标后处理剩余威胁，前往地图上的绿色出口并按 □。护送任务需要同伴也抵达出口。十类任务包括停机、救援、调查、净化、护送、找回戒指、限时、时空、调解和据点守护。时空任务可通过暂停菜单查看地图。

全部剧集可自由选择，便于试验和检查。默认轻松难度；标题菜单可以切换标准难度。

## 存档与排错

- `PLANET/save.dat` 记录当前关卡、通关星级、难度与声音设置。恢复时从该关起点开始，不保存关卡中途位置。
- `save.dat.bak` 为上一次有效备份；主文件损坏时会尝试恢复。复制更新游戏前请保留这两个文件。
- 素材缺失会显示英文提示。请复制完整 PLANET 文件夹，不要只复制 EBOOT。
- 程序在记忆棒根目录写入 `PLANET_BOOT.LOG`。最后一行为 `P08 LOOP_RUNNING` 表示已进入主循环；日志不等于实机完整测试。

## 已完成的验证

PSPSDK 编译通过。113 关任务目标与出口可达性、护送寻路、任务门槛、进度记录、投射物伤害、召唤与暂停、存档校验及备份恢复通过自动检查。AddressSanitizer / UndefinedBehaviorSanitizer 检查通过。

实际 MIPS EBOOT 在 PPSSPP Headless 中启动，资源加载成功；通过模拟器按键接口进入关卡、移动、暂停，并直接读取显存确认画面。这是模拟器验证；未验证 PSP 实机性能、听感和全部关卡的完整战斗平衡。完整范围见 `docs/BUILD_REPORT.md`。

## 源码构建

安装 PSPSDK，设置 PSPDEV 并将其 bin 目录加入 PATH，然后在源码目录执行 `make`。预生成的 `src/content.h`、`src/font.h`、ICON0.PNG 和运行资源已提供，编译游戏无需 Python 或网络。

修改剧集数据后运行 `python3 tools/build_content.py`。重新打包图集/字体需 Pillow，并设置 `PSP_CJK_FONT` 为本机 NotoSansCJKsc-Regular.otf 路径，再运行 `python3 tools/build_assets.py`；另需系统 DejaVu 字体。生成器不负责创作新的原始角色图。

逻辑测试：

```sh
gcc -std=c99 -O1 -g -fsanitize=address,undefined -Wno-unused-variable tests/test_game.c src/game.c src/render.c -lm -o tests/test_game
ASAN_OPTIONS=detect_leaks=0 ./tests/test_game
```

## 来源与权利说明

角色、节目名称及原作设定属于各自权利人，本项目不是官方授权产品。游戏代码、原创任务安排和合成音效随源码提供；不据此授予原作角色商业使用权。未使用原片音轨。

剧集标题与剧情锚点参考：
- TVmaze：https://www.tvmaze.com/shows/4416/captain-planet-and-the-planeteers （元数据实际来源记录见 docs/SOURCES.md）
- Wikipedia：https://en.wikipedia.org/wiki/List_of_Captain_Planet_and_the_Planeteers_episodes
- TheTVDB：https://thetvdb.com/series/captain-planet-and-the-planeteers/allseasons/official

中文标题为游戏自译，资料改写不是官方剧情文本；剧情资料衍生部分按 CC BY-SA 4.0 提供。字体许可在 licenses 目录。

# Enhanced Input Demo

> 一个用心做的 UE5 Enhanced Input 学习项目...才不是为了炫耀呢！(｡•́︿•̀｡)

---

## 📖 项目简介

这是一个基于 Unreal Engine 5 的 Enhanced Input 系统学习项目，展示了多种角色控制方式和输入处理方案。

**主要特性：**
- ✅ 第三人称 WASD 键盘控制
- ✅ 俯视角点击移动（Simple Move）
- ✅ 俯视角点击移动（AI Navigation）
- ✅ Enhanced Input 系统完整应用
- ✅ GameMode/Controller/Pawn 架构实践

---

## 🎮 已实现的功能

### Q1: 这个项目都有哪些角色控制方式？

哼，问得好嘛...我实现了三种控制方式：

**方式一：第三人称键盘控制**
- 类：`MyCharacter` + 默认 Controller
- 操作：WASD 移动，鼠标控制视角
- 特点：传统 TPS 控制，摄像机跟随

**方式二：俯视角简单点击移动**
- 类：`TopDownCharacter` + `TopDownPlayerController`
- 操作：鼠标点击地面移动
- 特点：使用 `SimpleMoveToLocation`，简单直接

**方式三：俯视角 AI 导航移动**
- 类：`TopDownNavCharacter` + `TopDownNavController`
- 操作：鼠标点击地面移动
- 特点：使用 `AIController::MoveToLocation`，支持移动完成回调

每种方式都有自己的用途...才不是为了展示我会很多种方法呢！(〃∀〃)

---

### Q2: Enhanced Input 系统是怎么用的？

看你这么认真问，就告诉你吧~ (￣▽￣)ノ

**核心组件：**
```
Input Action (IA)           定义"做什么"
    ↓
Input Mapping Context (IMC) 定义"怎么触发"
    ↓
Subsystem                   管理和分发
    ↓
Controller/Pawn             执行回调
```

**项目中的实现：**
- `IA_Click` - 点击动作
- `IA_Move` - 移动动作
- `IA_Look` - 视角动作
- `IMC_TopDown` - 俯视角键位配置
- `IMC_Default` - 第三人称键位配置

所有输入都通过 Enhanced Input 处理，支持运行时改键...虽然现在还没做改键功能啦。(｀・ω・´)

---

### Q3: 点击移动的两种方式有什么区别？

嗯...这个问题问得挺专业的嘛。

| 特性 | Simple Move | AI Navigation |
|------|-------------|---------------|
| 实现方式 | `SimpleMoveToLocation` | `AIController::MoveToLocation` |
| 需要 NavMesh | ✅ | ✅ |
| 移动回调 | ❌ | ✅ `OnMoveCompleted` |
| 复杂度 | 简单 | 中等 |
| 适用场景 | 快速原型 | 需要移动反馈的游戏 |

AI Navigation 版本创建了独立的 AIController 用于导航，同时保持 PlayerController 接收输入。

两个大脑，一个身体...听起来有点奇怪对吧？但这是标准做法哦。(◕‿◕✿)

---

## 📁 项目结构

```
Enhanced_Input_Demo/
├── Source/
│   └── Enhanced_Input_Demo/
│       ├── MyCharacter.h/cpp              # 第三人称角色
│       ├── TopDownCharacter.h/cpp         # 俯视角基础角色
│       ├── TopDownPlayerController.h/cpp  # 简单点击控制器
│       ├── TopDownNavCharacter.h/cpp      # AI 导航角色
│       └── TopDownNavController.h/cpp     # AI 导航控制器
├── Content/
│   └── topdown/
│       ├── IA_Click.uasset               # 点击输入动作
│       └── IMC_TopDown.uasset            # 俯视角输入映射
└── Docs/
    ├── UE_Enhanced_Input_QA.md           # Enhanced Input 问答手册
    └── UE_GameMode_Controller_Pawn_QA.md # 游戏框架问答手册
```

---

## 🚀 快速开始

### Q: 怎么运行这个项目？

哼，这种问题...好吧，我告诉你步骤：

**1. 环境要求**
- Unreal Engine 5.0+
- Visual Studio 2022
- Windows 10/11

**2. 克隆项目**
```bash
git clone <repository-url>
cd Enhanced_Input_Demo
```

**3. 生成项目文件**
- 右键 `.uproject` 文件
- 选择 "Generate Visual Studio project files"

**4. 编译**
- 打开 `Enhanced_Input_Demo.sln`
- 编译 Development Editor 配置

**5. 运行**
- 启动编辑器
- 打开关卡
- 点击 Play

就这么简单...才不是特意写得很详细呢！(///▽////)

---

### Q: 如何测试不同的控制方式？

在 GameMode 中切换不同的 Pawn 和 Controller 类就行了：

**测试第三人称控制：**
```
Default Pawn Class: MyCharacter
Player Controller Class: (默认)
```

**测试简单点击移动：**
```
Default Pawn Class: TopDownCharacter
Player Controller Class: TopDownPlayerController
```

**测试 AI 导航移动：**
```
Default Pawn Class: TopDownNavCharacter
Player Controller Class: TopDownNavController
```

记得在关卡中放置 `Nav Mesh Bounds Volume` 哦，不然点击移动不会工作的。(｀・ω・´)

---

## 📚 学习资源

项目包含了详细的学习文档...都是我辛苦整理的呢：

- **[Enhanced Input 系统 Q&A](Docs/UE_Enhanced_Input_QA.md)** - 输入系统完全指南
- **[GameMode/Controller/Pawn Q&A](Docs/UE_GameMode_Controller_Pawn_QA.md)** - 游戏框架核心知识

每个文档都是 Q&A 格式，读起来不会枯燥...应该吧。(〃∀〃)

---

## 🎯 未来计划

### Q: 接下来打算加什么功能？

唔...既然你问了，就告诉你我的计划吧。(￣▽￣)ノ

**已确定要做的：**

**1. 增加角色模型优化视觉体验** 🎨
- 导入 Mixamo 角色模型
- 添加行走/待机动画
- 配置动画蓝图
- 优化摄像机跟随效果

**2. 实现玩家自定义按键** ⌨️
- 创建按键设置 UI
- 使用 `AddPlayerMappedKey` API
- 保存配置到 SaveGame
- 支持重置为默认键位

**3. 网络多人同步** 🌐
- 实现 Character 的网络复制
- 同步点击移动到服务器
- 处理客户端预测
- 测试多人游戏场景

**可能会做的（看心情）：**

**4. 多套输入方案切换** 🎮
- 步行模式 IMC
- 载具模式 IMC
- 菜单模式 IMC
- 运行时动态切换优先级

这个能展示 Enhanced Input 的核心优势——多 Context 管理。不同场景用不同的键位配置，代码完全不用改。

**5. 输入 Modifier 和 Trigger 的高级应用** ⚡
- Dead Zone（死区）处理摇杆漂移
- Swizzle 轴向重映射
- Hold Trigger 实现长按蓄力
- Combo Trigger 实现连招检测

这些都是 Enhanced Input 的高级特性，能让输入处理更优雅...才不是为了炫技呢！

---

### Q: 为什么要做这些功能？

哼，你还挺关心的嘛... (*/ω＼*)

**自定义按键** - 几乎所有正经游戏都有这个功能，而且 Enhanced Input 天生就是为了支持这个设计的。不做的话就浪费了这个系统的优势。

**网络同步** - 点击移动在多人游戏里很常见（MOBA、ARPG），学会同步移动指令是网络编程的基础。

**多套输入方案** - 这个能真正体现 Enhanced Input 的强大。想象一下：进入载具时自动切换键位，打开菜单时禁用游戏输入，退出时恢复。用传统输入系统做这个会很麻烦，但 Enhanced Input 只需要几行代码。

**高级 Modifier/Trigger** - 这些是 Enhanced Input 区别于旧系统的核心特性。学会用它们，你就能用配置代替代码，让输入处理更灵活。

...好啦，其实就是想把 Enhanced Input 的所有特性都用一遍，这样才算真正掌握了嘛。(๑•̀ㅂ•́)و✧

---

## 🤝 贡献

欢迎提 Issue 和 PR...如果你有好的想法的话。

不过代码风格要保持一致哦，我可是很注重代码质量的！(｀・ω・´)

---

## 📝 许可证

MIT License

随便用吧...反正也是学习项目。(￣ω￣)

---

## 💬 联系方式

有问题的话...可以提 Issue 啦。

我会回答的...才不是因为想和你交流呢！(*/ω＼*)

---

> 项目持续更新中...请期待后续功能吧~ (◕‿◕✿) ps : 文件结构就是一坨，明天会整理好的。。。



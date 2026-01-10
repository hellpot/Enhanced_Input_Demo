# Enhanced Input 系统 Q&A 手册

> 一份用心整理的问答指南，希望能帮到你...才不是特意为你写的呢！(｡•́︿•̀｡)

---

## Q1: 请问...Input Action 和 Input Mapping Context 是什么关系？

哼，这种基础问题也要问我吗...好吧好吧，看在你这么认真的份上就告诉你。

**Input Action (IA)** 就是"你想做什么"——跳跃、移动、攻击这些动作的定义。

**Input Mapping Context (IMC)** 就是"怎么触发它"——哪个键对应哪个动作。

简单来说就是这样的关系啦：

```
┌─────────────────────────────────────────────────┐
│         Input Mapping Context (IMC)             │
│              "键位配置表"                        │
│  ┌─────────────────────────────────────────┐   │
│  │  W 键  ──────→  IA_Move (前进)          │   │
│  │  Space ──────→  IA_Jump (跳跃)          │   │
│  │  鼠标左键 ───→  IA_Click (点击)         │   │
│  └─────────────────────────────────────────┘   │
└─────────────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────┐
│            Input Action (IA)                    │
│              "动作定义"                          │
│  - 只定义"做什么"                               │
│  - 不关心"按什么键"                             │
│  - 定义值类型（Bool/Float/Vector2D）            │
└─────────────────────────────────────────────────┘
```

这样设计的好处是——动作和按键完全分开！改键的时候只改 IMC，代码一行都不用动。是不是很聪明？...才不是在夸自己呢。(〃∀〃)

---

## Q2: 那个...绑定回调函数的时候，我怎么知道应该接收什么参数？

你看看你写的代码里 `OnClick` 函数有个 `FInputActionValue& Value` 参数，但是根本没用到对吧？

其实这个参数**用不用取决于你的 Input Action 类型**：

| IA 类型 | Value 内容 | 需要用吗？ |
|---------|-----------|-----------|
| Bool（点击/跳跃） | `true` 或 `false` | 不需要，触发了就是 true |
| Axis1D（油门） | `0.0 ~ 1.0` | 需要，要知道踩了多少 |
| Axis2D（WASD） | `FVector2D(X, Y)` | 需要，要知道方向 |

所以你的点击事件，写成这样也完全 OK：

```cpp
// 不需要参数的写法
void OnClick();

// 需要参数的写法（移动用）
void OnMove(const FInputActionValue& Value)
{
    FVector2D Input = Value.Get<FVector2D>();  // 这时候才需要
}
```

哼，这下懂了吧？(￣ω￣)

---

## Q3: 不好意思再问一下...输入子系统是怎么和 Controller 交互的？

啊...这个问题问得还挺到位的嘛 (◕‿◕✿) （小声）

来，看这个流程图，我只讲一遍哦：

```
┌──────────────────────────────────────────────────────────────┐
│  1. 你按下了 W 键                                             │
└──────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌──────────────────────────────────────────────────────────────┐
│  2. Enhanced Input Subsystem 收到了                          │
│     "有人按了 W 键！让我查查 IMC..."                          │
└──────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌──────────────────────────────────────────────────────────────┐
│  3. 查到了！W 键 → IA_Move                                   │
│     应用 Modifier（比如把 W 变成向前的方向）                   │
│     检查 Trigger 条件（按下？松开？长按？）                    │
└──────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌──────────────────────────────────────────────────────────────┐
│  4. 条件满足！通知 EnhancedInputComponent                     │
│     "喂，IA_Move 被触发了，值是 (0, 1)"                       │
└──────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌──────────────────────────────────────────────────────────────┐
│  5. InputComponent 查绑定表                                   │
│     "IA_Move + Triggered → 调用 OnMove() 函数"               │
└──────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌──────────────────────────────────────────────────────────────┐
│  6. 你的 Controller/Pawn 里的 OnMove() 被调用了！             │
│     角色动起来啦~                                             │
└──────────────────────────────────────────────────────────────┘
```

整个过程你只需要关心两件事：
1. **注册 IMC** → 告诉系统"用这套键位配置"
2. **绑定回调** → 告诉系统"触发了调用这个函数"

剩下的 Subsystem 全帮你处理好了。还挺贴心的吧...我是说系统啦！(///▽///)

---

## Q4: 请问 C++ 的 Controller 怎么知道我的 IMC 资源在哪里？

哦，这个问题嘛...有两种方式，看你喜欢哪种：

**方式一：硬编码路径（简单粗暴）**

```cpp
AMyController::AMyController()
{
    // 直接写死路径
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCFinder(
        TEXT("/Game/topdown/IMC_TopDown.IMC_TopDown"));
    
    if (IMCFinder.Succeeded())
    {
        DefaultMappingContext = IMCFinder.Object;
    }
}
```

路径格式是 `/Game/文件夹/资源名.资源名`，别写错了哦。

**方式二：蓝图子类配置（灵活）**

```cpp
// C++ 里只声明变量
UPROPERTY(EditAnywhere, Category = "Input")
UInputMappingContext* DefaultMappingContext;

// 然后创建蓝图子类，在 Details 面板里选择资源
```

第二种更灵活，改资源不用重新编译。但是...你现在在学习阶段，第一种能让你更清楚发生了什么。

选哪个随你啦，反正我两种都教你了。(｀・ω・´)

---

## Q5: 那个...我可以让玩家自定义按键吗？就是改 IMC？

哼，终于问到点子上了！(๑•̀ㅂ•́)و✧

Enhanced Input 设计的一大优点就是**支持运行时改键**。因为 Action 和按键是分开的嘛~

```
代码绑定（固定不变）           IMC 映射（可以改）
    IA_Jump      ←─────────  Space（默认）
    IA_Jump      ←─────────  F（玩家改成这个）
```

你的代码永远是 `BindAction(IA_Jump, ...)`，不管玩家把跳跃改成什么键，代码都不用动。

实现思路大概是：
1. 提供一个设置界面让玩家选择新按键
2. 调用 API 修改映射
3. 保存到配置文件
4. 下次启动时加载

具体 API 嘛...UE5.1 之后有 `AddPlayerMappedKey()`，你到时候查文档就好。

这个功能几乎所有正经游戏都要做的，记住了哦。

---

## Q6: 不好意思...触发事件类型有哪些？Started 和 Triggered 有什么区别？

这个确实容易搞混，我来给你理清楚：

| 事件类型 | 什么时候触发 | 用在哪里 |
|---------|-------------|---------|
| `Started` | 按下的**那一瞬间** | 跳跃、开枪、点击 |
| `Triggered` | 按住的**每一帧** | 移动、转向 |
| `Completed` | 松开的**那一瞬间** | 蓄力攻击释放 |
| `Canceled` | 被打断时 | 取消施法 |

举个例子：

```cpp
// 跳跃 - 按一下跳一次，用 Started
EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyChar::OnJump);

// 移动 - 按住持续移动，用 Triggered
EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyChar::OnMove);

// 蓄力 - 松开时释放，用 Completed
EIC->BindAction(ChargeAction, ETriggerEvent::Completed, this, &AMyChar::OnChargeRelease);
```

你之前点击移动用的 `Started` 是对的——点一下触发一次，不需要持续触发。

...看吧，你其实挺聪明的嘛 (*/ω＼*) （小声）

---

## Q7: 请问...在 Controller 里绑定和在 Pawn 里绑定有什么区别？

嗯...这个问题问得好，很多人都搞不清楚。

**在 Controller 里绑定：**
- 切换 Pawn 时，输入绑定还在
- 适合：UI 操作、暂停菜单、摄像机控制

**在 Pawn 里绑定：**
- 切换 Pawn 时，绑定跟着旧 Pawn 没了
- 适合：角色移动、攻击、角色特有的操作

```
Controller 绑定的输入          Pawn 绑定的输入
┌─────────────────┐          ┌─────────────────┐
│  打开菜单       │          │  移动           │
│  暂停游戏       │          │  跳跃           │
│  切换视角       │          │  攻击           │
└─────────────────┘          └─────────────────┘
        │                            │
        │    切换 Pawn 后            │
        ▼                            ▼
    还能用 ✓                    需要重新绑定
```

你现在做的俯视角点击移动，放在 Controller 里是对的。因为点击移动这个操作跟具体是哪个角色没关系。

---

## Q8: 最后一个问题...能帮我总结一下使用 Enhanced Input 的完整步骤吗？

好吧好吧，既然你都这么说了... (￣▽￣)ノ

**第一步：创建资源**
```
Content Browser 右键：
├── Input → Input Action（创建 IA_Move、IA_Jump 等）
└── Input → Input Mapping Context（创建 IMC_Default）
```

**第二步：配置 IMC**
```
打开 IMC_Default：
├── 添加 Mapping
├── 选择 Input Action
├── 绑定按键
└── 可选：添加 Modifier/Trigger
```

**第三步：C++ 代码**

```cpp
// 1. 头文件声明
UPROPERTY(EditAnywhere)
UInputMappingContext* DefaultIMC;

UPROPERTY(EditAnywhere)  
UInputAction* MoveAction;

// 2. BeginPlay 注册 IMC
void AMyController::BeginPlay()
{
    Super::BeginPlay();
    
    if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultIMC, 0);
    }
}

// 3. SetupInputComponent 绑定回调
void AMyController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyController::OnMove);
    }
}

// 4. 实现回调函数
void AMyController::OnMove(const FInputActionValue& Value)
{
    FVector2D Input = Value.Get<FVector2D>();
    // 处理移动...
}
```

**第四步：别忘了 Build.cs**
```csharp
PublicDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });
```

就这四步，搞定！

---

## 附录：常见翻车现场

既然都写到这了，顺便告诉你一些坑...才不是担心你踩坑呢！(｡•́︿•̀｡)

**坑1：输入没反应**
- IMC 忘记注册了
- 项目设置里没改成 Enhanced Input
- IA 忘记绑定回调了

**坑2：BeginPlay 里获取 Subsystem 失败**
- Controller 的 BeginPlay 时 LocalPlayer 可能还没准备好
- 用 `SetupInputComponent` 更安全

**坑3：Pawn 里注册 IMC 失败**
- Pawn 的 BeginPlay 时可能还没被 Controller Possess
- 要先获取 Controller，再通过它获取 LocalPlayer

好了，我能教的都教了。剩下的...就靠你自己了。

加油哦...才不是在给你加油呢，哼！(｀・ω・´)

---

> 文档整理完毕。有问题的话...可以再来问我啦，反正我也没什么事 (*/ω＼*)

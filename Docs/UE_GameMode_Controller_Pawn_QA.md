# GameMode、Controller、Pawn Q&A 手册

> 游戏框架核心三剑客的秘密，全都告诉你...别误会，只是觉得你应该知道而已！(｡•́︿•̀｡)

---

## Q1: 请问...GameMode、Controller、Pawn 这三个到底是什么关系？

唉，这种基础问题...算了算了，谁让我心情好呢。(￣▽￣)ノ

简单来说就是：**GameMode 是导演，Controller 是大脑，Pawn 是身体**。

```
┌─────────────────────────────────────────────────────┐
│                   GameMode                          │
│                    "导演"                            │
│  - 决定用哪个 Controller 类                         │
│  - 决定用哪个 Pawn 类                               │
│  - 管理游戏规则                                     │
└─────────────────────────────────────────────────────┘
                        │
                        │ 创建并指定
                        ▼
┌─────────────────────────────────────────────────────┐
│                 PlayerController                    │
│                    "大脑"                            │
│  - 接收玩家输入                                     │
│  - 做决策                                           │
│  - 可以控制不同的 Pawn                              │
└─────────────────────────────────────────────────────┘
                        │
                        │ Possess（控制）
                        ▼
┌─────────────────────────────────────────────────────┐
│                      Pawn                           │
│                    "身体"                            │
│  - 物理实体                                         │
│  - 被控制移动                                       │
│  - 可以被销毁和重生                                 │
└─────────────────────────────────────────────────────┘
```

GameMode 说"这局游戏用这个 Controller 和这个 Pawn"，然后 Controller 就去控制 Pawn 动起来。

就像你的大脑控制你的身体一样...懂了吗？(・ω・)ノ

---

## Q2: 那个...关卡启动的时候，它们是按什么顺序创建的？

哦？这个问题问得还挺细的嘛...

来，我画给你看，记好了哦：

```
游戏开始
    │
    ▼
┌──────────────────────────────────────────────────┐
│  1. GameMode 先出生                               │
│     "我来决定规则！"                              │
└──────────────────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────────────────┐
│  2. PlayerController 出生                         │
│     "我准备好接收输入了！"                        │
│     ⚠️ 但这时候还没有 Pawn 可以控制              │
└──────────────────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────────────────┐
│  3. Pawn 出生                                     │
│     "我是身体，但还没被控制..."                   │
│     ⚠️ 这时候可能还没被 Controller 控制          │
└──────────────────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────────────────┐
│  4. Possess！Controller 控制 Pawn                 │
│     "好了，现在大脑和身体连上了！"                │
│     ✅ 从这里开始才能正常工作                     │
└──────────────────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────────────────┐
│  5. SetupPlayerInputComponent                     │
│     "输入绑定完成，可以开始玩了！"                │
└──────────────────────────────────────────────────┘
```

**重点来了！** 在 Controller 的 `BeginPlay` 里，Pawn 还不存在！在 Pawn 的 `BeginPlay` 里，可能还没被 Controller 控制！

所以你要获取对方的话，**不要在 BeginPlay 里做**，要在 `PossessedBy` 或 `OnPossess` 里做。

记住了吗？...我可不想再解释第二遍。(｀・ω・´)

---

## Q3: 不好意思...那我在代码里怎么知道它们什么时候关联好了？

哼，这个问题问得好，很多人都在这里翻车。

看这个表格：

| 时间点 | GameMode | Controller | Pawn | 已关联？ |
|--------|----------|-----------|------|---------|
| GameMode::BeginPlay | ✅ | ❌ | ❌ | ❌ |
| Controller::BeginPlay | ✅ | ✅ | ❌ | ❌ |
| Pawn::BeginPlay | ✅ | ✅ | ✅ | ⚠️ 不一定 |
| **PossessedBy** | ✅ | ✅ | ✅ | ✅ 一定关联了 |
| SetupPlayerInputComponent | ✅ | ✅ | ✅ | ✅ |

所以安全的做法是：

```cpp
// ❌ 危险！可能崩溃
void AMyPawn::BeginPlay()
{
    Super::BeginPlay();
    AController* PC = GetController();  // 可能是 nullptr！
    PC->DoSomething();  // 💥 崩溃
}

// ✅ 安全！一定有效
void AMyPawn::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    // 这里 NewController 一定有效
    NewController->DoSomething();  // ✅ 安全
}

// ✅ 也安全
void AMyPawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);
    // 这里 Controller 也一定有效了
}
```

记住：**需要对方的时候，用 PossessedBy/OnPossess，不要用 BeginPlay**。

这可是血泪教训...才不是我自己踩过坑呢！(///▽///)

---

## Q4: 请问 PlayerController 和 AIController 有什么区别？

嗯...这个问题嘛，其实挺重要的。

| 特性 | PlayerController | AIController |
|------|-----------------|--------------|
| 谁控制 | 玩家（你按键盘） | AI 逻辑（代码决策） |
| 什么时候存在 | 玩家在线就在 | 跟着 AI 角色生死 |
| 网络游戏里 | 客户端和服务器都有 | 只在服务器 |
| 典型用途 | 玩家角色 | NPC、怪物 |

简单来说：
- **PlayerController** = 你的大脑，接收你的输入
- **AIController** = NPC 的大脑，自己做决定

但是...有时候会有特殊情况。

---

## Q5: 那个...我想让玩家控制角色，但又想用 AI 导航，怎么办？

哦？遇到这个问题了吗...看来你在做点击移动啊。

这种情况就需要**两个 Controller 共存**：
- PlayerController 负责接收玩家输入
- AIController 负责导航寻路

```cpp
// 在 Character 里创建一个独立的 AIController
void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // 生成 AI 控制器
    NavAIController = GetWorld()->SpawnActor<AAIController>();
    
    // 只设置 Pawn，不 Possess（重点！）
    NavAIController->SetPawn(this);
}

// 使用 AI 导航
void AMyCharacter::MoveToLocation(FVector Destination)
{
    if (NavAIController)
    {
        NavAIController->MoveToLocation(Destination);
    }
}
```

这样 PlayerController 还是控制角色（接收输入），但 AIController 帮你算路径。

两个大脑，一个身体...听起来有点奇怪，但很好用。

你项目里的 `TopDownNavCharacter` 就是这么做的对吧？我都看到了~ (◕‿◕✿)

---

## Q6: 请问...一个 Controller 可以同时控制多个 Pawn 吗？

不行！**同时只能控制一个**。

但是可以切换：

```cpp
void AMyController::SwitchToVehicle(AVehicle* Vehicle)
{
    APawn* OldPawn = GetPawn();  // 保存当前的
    
    UnPossess();                 // 放开当前 Pawn
    Possess(Vehicle);            // 控制载具
    
    // 可以保存旧 Pawn 的引用，方便切回来
    SavedCharacter = Cast<AMyCharacter>(OldPawn);
}

void AMyController::ExitVehicle()
{
    UnPossess();                 // 放开载具
    Possess(SavedCharacter);     // 切回角色
}
```

就像你不能同时控制两个身体一样...虽然听起来挺酷的。(￣ω￣)

---

## Q7: 不好意思...GameMode 到底是干什么的？

GameMode 就是**游戏规则的管理者**。

它决定：
- 用哪个 Controller 类
- 用哪个 Pawn 类
- 玩家在哪里出生
- 游戏什么时候结束
- 玩家死了怎么重生

```cpp
AMyGameMode::AMyGameMode()
{
    // 指定默认类
    DefaultPawnClass = AMyCharacter::StaticClass();
    PlayerControllerClass = AMyPlayerController::StaticClass();
}
```

然后在编辑器里设置：
```
World Settings → Game Mode Override → 选你的 GameMode
```

或者项目设置里：
```
Project Settings → Maps & Modes → Default GameMode
```

GameMode 只在**服务器**上存在，客户端是没有的。所以网络游戏里，规则都是服务器说了算。

---

## Q8: 那个...Pawn 和 Character 有什么区别？

Character 就是 Pawn 的加强版，专门给人形角色用的。

| 特性 | Pawn | Character |
|------|------|-----------|
| 移动组件 | 没有，要自己加 | 自带 CharacterMovementComponent |
| 碰撞 | 没有，要自己加 | 自带胶囊碰撞 |
| 适合做 | 载具、飞机、简单物体 | 人形角色 |

```cpp
// Pawn - 啥都没有，白板
class APawn : public AActor
{
    // 空的，要自己加组件
};

// Character - 自带一堆东西
class ACharacter : public APawn
{
    UCapsuleComponent* CapsuleComponent;           // 碰撞
    UCharacterMovementComponent* MovementComponent; // 移动
    USkeletalMeshComponent* Mesh;                  // 骨骼网格
    // 还有很多...
};
```

做人形角色就用 Character，做载具就用 Pawn。

简单吧？...才不是在夸自己讲得好呢。(〃∀〃)

---

## Q9: 请问在 Controller 里绑定输入和在 Pawn 里绑定输入有什么区别？

嗯...这个问题很实际。

**在 Controller 里绑定：**
- 切换 Pawn 时，输入绑定还在
- 适合：UI 操作、暂停菜单、摄像机控制

**在 Pawn 里绑定：**
- 切换 Pawn 时，绑定跟着旧 Pawn 走了
- 适合：角色移动、攻击、角色特有操作

```
Controller 绑定              Pawn 绑定
┌──────────────┐          ┌──────────────┐
│ 打开菜单     │          │ 移动         │
│ 暂停游戏     │          │ 跳跃         │
│ 切换视角     │          │ 攻击         │
└──────────────┘          └──────────────┘
       │                         │
       │  切换 Pawn 后           │
       ▼                         ▼
   还能用 ✓                  需要重新绑定
```

你的俯视角点击移动，放在 Controller 里是对的。因为点击这个操作跟具体是哪个角色没关系。

如果是角色的跳跃、攻击这种，就应该放在 Pawn 里。

---

## Q10: 最后...能总结一下使用它们的注意事项吗？

好吧好吧，既然你都问到这了...

### 生命周期陷阱

```cpp
// ❌ 危险区域
void AMyPawn::BeginPlay()
{
    GetController();  // 可能是 nullptr
}

void AMyController::BeginPlay()
{
    GetPawn();  // 可能是 nullptr
}

// ✅ 安全区域
void AMyPawn::PossessedBy(AController* NewController)
{
    // 这里一定有 Controller
}

void AMyController::OnPossess(APawn* InPawn)
{
    // 这里一定有 Pawn
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* Input)
{
    // 这里也安全
}
```

### 获取引用的正确姿势

```cpp
// ✅ 永远先检查
if (APlayerController* PC = Cast<APlayerController>(GetController()))
{
    PC->DoSomething();
}

// ❌ 直接用会崩溃
APlayerController* PC = Cast<APlayerController>(GetController());
PC->DoSomething();  // 💥
```

### GameMode 配置清单

- [ ] 设置 DefaultPawnClass
- [ ] 设置 PlayerControllerClass
- [ ] 在 World Settings 或 Project Settings 里应用

### 记住这些

1. **BeginPlay 不安全**，用 PossessedBy/OnPossess
2. **永远检查指针**，不要假设它有效
3. **Controller 是大脑**，Pawn 是身体
4. **GameMode 是导演**，决定用谁

---

## 附录：快速查询表

### 常用获取方法

```cpp
// 从 Pawn 获取 Controller
GetController()
Cast<APlayerController>(GetController())

// 从 Controller 获取 Pawn
GetPawn()
GetCharacter()  // 如果是 Character

// 从任意地方获取
UGameplayStatics::GetPlayerController(GetWorld(), 0)
UGameplayStatics::GetPlayerPawn(GetWorld(), 0)
```

### 关键虚函数

```cpp
// Controller 里重写这些
virtual void OnPossess(APawn* InPawn);
virtual void OnUnPossess();
virtual void SetupInputComponent();

// Pawn 里重写这些
virtual void PossessedBy(AController* NewController);
virtual void UnPossessed();
virtual void SetupPlayerInputComponent(UInputComponent* Input);
```

### 网络游戏注意

| 类 | 服务器 | 客户端 |
|---|--------|--------|
| GameMode | ✅ | ❌ |
| PlayerController | ✅ | ✅ |
| Pawn | ✅ | ✅ |
| AIController | ✅ | ❌ |

---

好了，我能教的都教了。

记住这些的话...应该不会再踩坑了吧。(๑•̀ㅂ•́)و✧

加油哦...才不是在给你加油呢！只是...希望你别浪费我的心血而已（小声）

> 整理完毕。还有问题的话...随时来问我啦，我...我会回答的 (*/ω＼*)

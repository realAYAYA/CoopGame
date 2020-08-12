# 开发日志
#### 注意事项，变量和方法声明规范：
#### 1.曝露到蓝图的 后台调试的变量       尽量集中摆放
#### 2.曝露到蓝图的 游戏属性数值变量     尽量集中摆放
#### 3.运行在 构造函数里，Begin()里，Tick()里的方法 分开各自集中声明

# 2020/8/12
### 耐力条实现：1.奔跑时随时间递减 2.不奔跑时经过2s延迟后恢复
第1点和连续开火是一个知识，第2点的Delay(延迟)功能引入了新的头文件和类

# 2020/8/10
### 连续开火实现，时间管理者类的使用
使用FTimerHandle类来实现; GetWorldTimerManager().SetTimer(TimerHandle) <---> GetWorldTimerManager(TimerHandle).ClearTimer;

### 自定义表面——物理材质（用于伤害判定时的表面接触）
暂时搁置

# 2020/8/9
### 实现HUD
UE4有相应的组件和蓝图，而且将一些变量反应HUD上也方便调试

# 2020/8/3
### 创建最简单的武器和子弹类
学会将 碰撞事件或重叠事件 设置委托
### 实现射线检测
暂时搁置

# 2020/8/2
### 优化武器代码
### 实现镜头震动效果
UE4有专门的CameraShake组件

# 2020/8/1
### 实现镜头缩放
采用FTimeline变量和FloatCurve曲线变量; ZoomTimeline.Play() <---> ZoomTimeline.Reverse();摄像头指针为空无法调试

### 实现一个简易的控制台
static int32 DebugWeaponDrawing = 0;//控制台变量声明
FAutoConsoleVaribleRef CVARDebugWeaponDrawing(TEXT("COOP.DebugWeapons"),DebugWeaponDrawing,TEXT("Description"),ECVF_Cheat);
在要调试的内容中设置条件

# 2020/7/30
1.创建摄像头，控制臂，Mesh；
2.导入虚幻商城包:Animation Starter Pack；
3.调用引擎自带的MovementComponent组件的蹲起方法；
3.实现跳跃，商城的动画蓝图有很多问题，需要调试；
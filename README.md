# 开发日志
## 注意事项，变量声明和方法声明规范：
1.曝露到蓝图的 后台调试的变量       尽量集中摆放
2.曝露到蓝图的 游戏属性数值变量     尽量集中摆放
3.运行在 构造函数里，Begin()里，Tick()里的方法 分开各自集中声明

# 2020/8/10
## 自定义表面——物理材质（用于伤害判定时的表面接触）
## 连续开火实现，时间管理者类的使用
武器指针为空，无法测试

# 2020/8/3
## 创建最简单的武器和子弹类
## 实现射线检测
暂时搁置

## 优化武器代码
## 实现镜头震动效果
武器指针为空，无法测试

## 实现一个简易的控制台
static int32 DebugWeaponDrawing = 0;//控制台变量声明
FAutoConsoleVaribleRef CVARDebugWeaponDrawing(TEXT("COOP.DebugWeapons"),DebugWeaponDrawing,TEXT("Description"),ECVF_Cheat);
在要调试的内容中设置条件

## 实现镜头缩放
设置一个目标值，根据是否来决定目标值，然后在Tick里利用DletaTime渐变
或采用Timeline
摄像头指针为空无法调试

# 2020/7/30
1.创建摄像头，控制臂，Mesh,
2.导入虚幻商城包:Animation Starter Pack
3.调用引擎自带的MovementComponent组件的蹲起方法。//不能蹲起时寻找问题：动画蓝图的类型转换。
3.跳，商城的动画蓝图有很多问题，需要调试







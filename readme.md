# QR_TGC V1.2.1 beta
> 一个QRSpeed的对话式文字游戏生成器  
> A QRSpeed Text Game Converter

![](https://s1.328888.xyz/2022/06/24/sa9om.png)

## 使用文档
QR_TGC（以下简称QTG）的运行由 **事件(event)** 实现，每个事件又由 **关键字(Keyword)** 组成。

一个最基本的QTG文件示例：
```
_begin
a
{
event1
}

event1
q
{
1+1=?
}
o
{
2
3
}
r
{
答对了 :)
答错了 :(
}
```

此示例由一个 **主事件(MainEvent)** 和一个 **次事件(MinorEvent)** 构成，主事件的作用是 **引出次事件** 
- 由于目前版本生成特性，若QTG文件中不存在主事件，则需要发送任意数字才能激活进程

主事件的名称需和QR_TGC设置一栏内的 **Main事件名称** 相一致，否则会被认为是次事件

最基础的主事件格式为
```
主事件名称
a
{
引出的次事件名称
}
```

"a"即为关键字之一
关键字所包含的内容由大括号声明，内容以行分割，分割后的内容称为 **分支(branch)** （一行为一个分支），分别对应不同的选项，如
```
o
{
o1
o2
}
a
{
a1
a2
}
```

这里的分支a1,a2就分别对应着分支o1,o2，回答选项o1时触发a1，回答o2时触发a2

如需要在分支中插入换行，请使用换行符 `\n` 
- - - 
### 以下是目前版本所有的关键字：
- **q** 
即"Question"，针对该事件选项的 **询问内容** 
- **o** 
即"Option"，针对该事件询问内容的 **选项** 
- **r** 
即"Return"，针对该事件指定选项的 **回答** 
 ~~不要问我为什么不是Answer，问就是被Action占了~~
- **a** 
即"Action"，针对该事件指定回答的 **行动** ，用于继续引用事件
- **c** 
即"Command"，针对该事件指定行动后执行的 **qr代码** 生成时会放在最后
- - -
关键字还有一个附加属性，即 **附属关键字(Additional Keyword)** ，添加在关键字后，用于对该关键字进行补充说明
- - -
### 以下是目前版本所有的附属关键字

- **.all** 
声明以下内容适用于任何分支，如在
```
o
{
o1
o2
}
```

的情况下，
```
a.all
{
event1
}
```

等同于
```
a
{
event1
event1
}
```

- - -
## 示例文件
Main事件名称为"main"
#### 附加指令：
```
[内部]好感度 (.*)
分:$读 进程/%QQ%/好感度 a 0$
$写 进程/%QQ%/好感度 a [%分%+%括号1%]$

[内部]好感度
a:$读 进程/%QQ%/好感度 a 0$
如果:%a%<0
rnm
返回
如果尾
如果:%a%<5
Feeling Kind
返回
如果尾
如果:%a%>5&%a%<10
Feeling Good
返回
如果尾
如果:%a%>10
Robot Likes U so Much
返回
如果尾

&好感度
$读 进程/%QQ%/好感度 a 0$
```

#### QTG文件：
```
main
a
{
e1
}
c
{
$写 进程/%QQ%/好感度 a 0$
}

e1
q
{
Hello, world!\nI'm Robot\nWhat do U wanna do?
}
o
{
拥抱
踢
}
r
{
Oh! Thanks for your love ;)
Ouch! It's not good :(
}
a
{
e2
e2
}
c
{
$回调 好感度 %随机数1-10%$
$回调 好感度 -5$
}

e2
q
{
What do U think of Me? :P
}
o
{
可爱
傻逼
}
r
{
Ohhh...Thanks!
Huh? WTF!
}
a
{
Null
e4
}
c
{
$回调 好感度 %随机数1-10%$\n$回调 好感度$
$回调 好感度 -5$
}

e4
q
{
U'd better say sorry!
}
o
{
Sorry
F**k U
}
r
{
Well,Okay
nmsl
}
a
{
e2
Null
}
c
{
$回调 空$
$回调 好感度$
}
```
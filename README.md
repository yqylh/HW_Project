## Todo

迁移 : 没有区分单核心双核心进行单独分析

三个策略是相辅相成的

## 买 : 规划问题

策略一 : 买总消耗最便宜的 13亿 消耗越小大概率越匹配 产生的碎片可能无法利用

解决方案 : 每次买的时候按照n（＞1）倍大小买性价比最高试一试

n应该取决于这台机器会买多少台 ? 现在按照2挺好的



~~策略二 : 买单核心均价最便宜的 43亿~~

~~策略三 : 统计一天的需求量 , 尽量买大的~~

~~可以改正这个虚拟机最近的需求量 ? 按照这个来考虑吧~~

~~还要考虑虚拟机会不会超过最大的服务器数量~~

~~**md 这是什么辣鸡策略 滚**~~

## 部署虚拟机 : 碎片整理

策略一 : 找核心和内存都尽量满的服务器

策略二 : 找核心和内存使用比例高的服务器

## 迁移 : 碎片整理

策略一 : 把利用率低的服务器上的虚拟机迁移到利用率高的服务器上

策略二 : 把剩余空间大的服务器上的虚拟机迁移到剩余空间小的服务器上

## 对比 

购买策略1 + 部署策略1 + 迁移策略1  : 1176902058 (目前的最优策略)

购买策略1 + 部署策略1 + 迁移策略2  : 1187972969

购买策略1 + 部署策略2 + 迁移策略1  : 1178665293

购买策略1 + 部署策略2 + 迁移策略2  : 1177417634 

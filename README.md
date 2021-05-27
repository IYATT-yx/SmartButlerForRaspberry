# 树梅派智能贴身管家程序

# 开始

克隆源代码
```bash
git clone https://github.com/IYATT-yx/SmartButlerForRaspberry --depth=1
```

构建
```bash
cd SmartButlerForRaspberry && mkdir build cd build

cmake -DCMAKE_BUILD_TYPE=RELEASE ..

cmake --build . --target install
```

# 测试环境

* 树梅派 4B 4G; Raspberry Pi OS 64位
* gcc 10.2.1
* cmake 3.18.4
* wiringPi 库 64 位
* YahBoom 树梅派风扇扩展版

# 版本

0.1、0.2 为 C++ 所写，也存在问题，运行一段时间后，会终止  
目前 main 分支为最新的，改用 C 语言写，同时不再使用单独的 py 安装脚本，直接 cmake 构建的同时安装  

# 版权
Copyright (c) 2021 IYATT-yx  
基于 [AGPL-3.0](LICENSE) 许可
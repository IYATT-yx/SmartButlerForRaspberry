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

# 版权
Copyright (c) 2021 IYATT-yx  
基于 [AGPL-3.0](LICENSE) 许可
<p align="center">CommLite</p>

<p align="center">
<a href="https://github.com/itas109/CommLite/releases"><img alt="Version" src="https://img.shields.io/github/release/itas109/CommLite"/></a>
<a href="https://github.com/itas109/CommLite/stargazers"><img alt="Stars" src="https://img.shields.io/github/stars/itas109/CommLite"/></a>
<a href="https://gitee.com/itas109/CommLite"><img alt="Stars" src="https://gitee.com/itas109/CommLite/badge/star.svg?theme=dark"/></a>
<a href="https://github.com/itas109/CommLite/network/members"><img alt="Forks" src="https://img.shields.io/github/forks/itas109/CommLite"/></a>
<a href="https://github.com/itas109/CommLite/blob/master/LICENSE"><img alt="License" src="https://img.shields.io/badge/License-LGPL%203.0-orange"/></a>
<img alt="language" src="https://img.shields.io/badge/language-c++-red"/>
<img alt="platform" src="https://img.shields.io/badge/platform-windows%20%7C%20linux%20%7C%20macos%20%7C%20raspberrypi%20%7C%20freebsd-lightgrey"/>
<img alt="appveyor-ci" src="https://ci.appveyor.com/api/projects/status/po449dxu00oqfpif?svg=true"/>
<img alt="travis-ci" src="https://app.travis-ci.com/itas109/CommLite.svg?branch=master"/>
<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/itas109/CommLite">
</p>

<p align="center">
Language: <strong>English</strong> / <a href="README.md">中文</a>
</p>

---

CommLite is tui's serial port tool based CSerialPort

# Platform
CommLite has been tested to the following platforms:

   - DOS ( x86_64 )
   - Windows ( x86_64 )
   - Linux ( x86_64, aarch64, mips64el, sw_64, s390x, ppc64le )
   - macOS ( x86_64 )
   - Raspberry Pi ( armv7l )
   - FreeBSD ( x86_64 )
   - ...

# Quick Start

```
$ git clone --depth=1 https://github.com/itas109/CommLite.git
$ cd CommLite
$ mkdir bin && cd bin
$ cmake ..
$ cmake --build .
$ ./commlite
```
linux package deb
```
$ cd bin
$ cpack .
```
ubuntu 20.04 cross-compile aarch64/arm/mips64el
```
$ sudo apt-get install g++-aarch64-linux-gnu
$ ./cmake_build_aarch64.sh

$ sudo apt-get install g++-arm-linux-gnueabi
$ ./cmake_build_arm.sh

$ sudo apt-get install g++-mips64el-linux-gnuabi64
$ ./cmake_build_mips64el.sh
```

# Screenshot

![image](./pic/commlite.gif)

# Contacting

* Email : itas109@qq.com

* QQ Group : [129518033](http://shang.qq.com/wpa/qunwpa?idkey=2888fa15c4513e6bfb9347052f36e437d919b2377161862948b2a49576679fc6)

# Donate

[CSDN](https://blog.csdn.net/itas109)

# License

[GNU Lesser General Public License v3.0](LICENSE)
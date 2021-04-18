#!/usr/bin/python3
'''
 * @copyright (c) 2021 IYATT-yx
 * @file Oled.hpp
 * @author IYATT-yx 2514374431@qq.com
 * @date 2021-2-27
 * @brief 安装部署脚本
'''
import sys
import os

# 参数错误输出
def perr():
    print()
    print('[build.py] 传入命令行参数错误!')
    print()
    print('\tinstall\t部署安装')
    print('\tremove\t移除')
    print()
    sys.exit(1)

def install():
    # build文件夹存放构建时产生的文件
    if not(os.path.exists('build')):
        os.mkdir('build')
        print('[build.py] 创建build文件夹')
    
    # 切换工作路径
    os.chdir('build')

    # 清空build
    os.system('rm -rf *')

    # 生成构建档
    os.system('cmake ..')
    if not(os.path.exists('Makefile')):
        print('[build.py]生成组态档失败!')
        sys.exit(1)

    # 构建二进制文件
    os.system('make -j4')
    os.chdir('..')
    if not(os.path.exists('bin/.SmartButler')):
        print('[build.py] 构建程序失败!')
        sys.exit(1)

    # 拷贝二进制文件到home目录
    if os.path.exists('/home/pi/.SmartButler'):
        os.remove('/home/pi/.SmartButler')
    os.system('cp bin/.SmartButler /home/pi')
    if not(os.path.exists('/home/pi/.SmartButler')):
        print('[build.py] 拷贝程序到home目录失败!')
        sys.exit(1)

    # 配置自启动
    if not(os.path.exists('/home/pi/.config/autostart')):
        os.makedirs('/home/pi/.config/autostart')
    with open('/home/pi/.config/autostart/SmartButler.desktop', 'w') as f:
        f.write('[Desktop Entry]\nType=Application\nExec=/home/pi/.SmartButler')
    if not(os.path.exists('/home/pi/.config/autostart/SmartButler.desktop')):
        print('[build.py] 配置自启动失败!')
        sys.exit(1)

    # 启动程序
    os.system('/home/pi/.SmartButler&')
    
    print('[build.py] 部署完成,欢迎使用!')
    print(40 * '+')
    print('author: IYATT-yx\nsite: iyatt.com')
    print(40 * '+')



def remove():
    if os.path.exists('/home/pi/.SmartButler'):
        os.remove('/home/pi/.SmartButler')
        print('[build.py] remove: /home/pi/.SmartButler')
    if os.path.exists('/home/pi/.config/autostart/SmartButler.desktop'):
        os.remove('/home/pi/.config/autostart/SmartButler.desktop')
        print('[build.py] remove: /home/pi/.config/autostart/SmartButler.desktop')
    
    print('[build.py] 移除成功,欢迎下次使用!')
    print(40 * '+')
    print('author: IYATT-yx\nsite: iyatt.com')
    print(40 * '+')

if __name__ == '__main__':
    # 检查参数个数
    if len(sys.argv) != 2:
        perr()

    # 检查参数是否正确
    if sys.argv[1] != 'install' and sys.argv[1] != 'remove':
        perr()

    # 根据命令行参数执行不同的操作
    if sys.argv[1] == 'install':
        install()
    elif sys.argv[1] == 'remove':
        remove()
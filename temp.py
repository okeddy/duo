context = '''/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：filename.h
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

'''

import os

for root,dirs,files in os.walk('.'):
    for name in files:
        if name.endswith('.cc') or name.endswith('.h'):
            with open(root + '/' + name, 'r+') as f:
                file = f.readline()
                if file != '/*\n':
                    f.seek(0, 0)
                    file = f.read()
                    f.write(context + file)
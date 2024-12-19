# IOT_ADD_System

CSproject/README UTF-8

103rxcode/407maincode GB2312

物联网农业检测系统，原裸机项目是在校学习与大创实践项目            
                
20/12/24    更新BH1750光照强度检测,已修改CSproject_test/cprj/client接收函数适配数据包,103test未修改接收适配,
            具体实现详情见 Using_RTOS/IOT_ADD_System/Readme.txt;
            后续动态:
                        1.先弄NekoWaifu_Core的赛博老婆;
                        2.Readme.txt写的bug弄赛博老婆的时候顺便修,用现有工程修改和适配新功能;
            后续优化:
                        1.使用触摸屏替换资源较为紧张的KEY,顺便再学个lvgl
                        2.ASRPRO语音识别模块,该项目暂时使用成品语音识别模块,AI语音识别将在该主页的NekoWaifu_Core项目仓库中实现。



17/12/24    更新USRAT3/u3_printf函数与接收框架、ESP8266-01s连接服务器AT指令,具体实现详情见 Using_RTOS/IOT_ADD_System/Readme.txt;
            后续动态:
                        1.实现BH1750光照强度检测,并更改csprj与103rx的接收函数;
                        2.暂停移植NCLLDS与RELAY类似按键与点灯内容。如有需求可参考KEY与LED新增相关内容,暂停SOIL_MOISTURE的ADC检测移植(懒得去装土);
            后续优化:
                        1.使用触摸屏替换资源较为紧张的KEY,顺便再学个lvgl
                        2.ASRPRO语音识别模块,该项目暂时使用成品语音识别模块,AI语音识别将在该主页的NekoWaifu_Core项目仓库中实现。
            PS:
                还是更想去弄赛博老婆,移植完BH1750就先去弄NekoWaifu_Core,再然后就是找个能吃上窝囊费的地方,再继续优化这两个项目，
                不过重心应该是在NekoWaifu_Core,而且接下来这个项目优化的点在NekoWaifu_Core也是覆盖到的,到时候比较完善了再移植过来。



13/12/24    更新移植RTOS版本、显示改用800*480LCD屏幕、新增Flash驱动与SDRAM显存,具体实现详情见 Using_RTOS/IOT_ADD_System/Readme.txt;
            USART3相关重定向与u3_printf函数暂未移植、ESP8266的AT指令控制未优化未启用,预计2天内更新。
            103RCV_test与CSproject_test测试用例可正常上下行。
            后续将新增:
                        1.触摸屏替换资源较为紧张的KEY:
                        2.BH1750光照强度检测;
                        3.ASRPRO语音识别模块,该项目暂时使用成品语音识别模块,AI语音识别将在该主页的NekoWaifu_Core项目仓库中实现。



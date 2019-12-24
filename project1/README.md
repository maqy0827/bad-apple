# 说明
使用VS winform编写上位机<br>
使用keil为stm32f103vet6写下位机（标准库）<br>
通过UART将数据发到单片机中<br>
实现单片机和电脑同步显示播放bad apple<br>
在机械硬盘中的读取延迟可能造成画面卡顿的现象<br>
#### 使用说明：
在使用前请解压file-data文件夹中的84x64data.zip和picture.zip压缩包<br>
在将signalchipproject中的程序下载至单片机中后按如下说明接线：<br>
oled屏幕接线：<br>
	SCL--B10<br>
	SDA--B11<br>
usb转串口接线：<br>
	RXD--C10<br>
	TXD--C11<br>
	usb一端连接电脑<br>
打开bad apple upper computer!!.exe文件<br>
点开始播放即可观看视频hhhhh<br>
在暂停播放的时候可以任意拖动进度条<br>
或者通过其他按钮控制当前显示的帧数。<br>
# PC上测试

## 编译
`./build.sh`

## 编码测试
注意输入`bmp`图片必须是`24`位格式,且一行数据量要是`4`的倍数,即`widthx3`是`4`的倍数.
`./encode_test testimg.bmp testimg.jpg 320 240 50`

## 解码测试
`./decode_test testimg.jpg testimg.rgb 320`
使用`YUView`软件查看输出文件`testimg.rgb`

## DEBUG（以解码为例）
`gdb --args ./decode_test testimg.jpg testimg.rgb 320`
`b jfread`
`r`

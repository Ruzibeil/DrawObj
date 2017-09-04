# -*- coding:UTF-8 -*-
'''
python 3不用第一行
with open这一行修改自己的路径 inf是读取的文件 out是把内容新写入的文件
作用：处理用3d-max提取出来的的单个零件obj的文件形式中的face（f）格式
过程：原先单个零件obj中 face的格式为：f 1/1/1 2/2/2 3/3/1
需要改变成为：f 1 2 3
所以先根据 空格 分为  f  1/1/1 2/2/2  3/3/1  \n
然后根据‘/‘  分割得到每个分割的第一个元素  并去除\n
最后分别写入到out
'''
cout_v = -1  
with open('F:/3Dmodel-2dimage/OpenGLDrawObj/obj/L_shape.obj','r+')as inf,open('face_test.obj','w')as  out:
    f_lines = inf.readlines()
    for f_line in f_lines:
        
        if f_line[0] == 'f':

           
            a = f_line.split(" ")
            l_vertex= [m_a.split('/')[0] for m_a in a if m_a!='\n' ]
            for vertex in l_vertex:
                cout_v += 1
                if(cout_v==3):
                   out.write(vertex+'\n')
                   cout_v=-1
                else:
                   out.write(vertex+" ")
                 
                

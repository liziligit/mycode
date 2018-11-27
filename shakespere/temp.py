# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import os
import shutil
import re
dir="C:\\Users\\neutron\\Desktop\\Python作业\\原始文件夹"
obj_txt="C:\\Users\\neutron\\Desktop\\Python作业\\目标文件夹\\after-combine\\莎士比亚14行诗2.txt"
obj_txt_f="C:\\Users\\neutron\\Desktop\\Python作业\\目标文件夹\\after-combine\\莎士比亚14行诗f.txt"
obj_txt_ff="C:\\Users\\neutron\\Desktop\\Python作业\\目标文件夹\\after-combine\\莎士比亚14行诗ff.txt"
obj_txt_fff="C:\\Users\\neutron\\Desktop\\Python作业\\目标文件夹\\after-combine\\莎士比亚14行诗.txt"
before_file="C:\\Users\\neutron\\Desktop\\Python作业\\目标文件夹\\before-combine"
##############################################搜寻并复制文本文件到before-combine
def detect_walk(dir_path):
    flist=[]
    for root,dirs,files in os.walk(dir_path):
        for file in files:
           if os.path.splitext(file)[1]==".txt": 
               allpath = os.path.join(root,file)
               #print (allpath)
               flist.append(allpath)         
    print (flist)
    for sname in flist:
        shutil.copy(sname,before_file)
##############################################写入到新文本文件《莎士比亚14行诗2.txt》
    fobj = open(obj_txt,'w')#追加模式
    for line in flist:
        for txt in open(line, 'r'):
            fobj.write(txt)
        fobj.write('\n')
    fobj.close()
    
if __name__ == "__main__":
    detect_walk(dir)
    
 
 #############################################删除空白行，生成《莎士比亚14行诗f.txt》
def delblankline(infile,outfile):
    infopen = open(infile,'r')
    outfopen = open(outfile,'w')
    lines = infopen.readlines()
    for line in lines:
        if line.split():  #对line进行分割，默认以空格分割，分割成一个个单词
            outfopen.writelines(line)
        else:
            outfopen.writelines("")
    infopen.close()
    outfopen.close()
    
if __name__ == "__main__":    
    delblankline(obj_txt,obj_txt_f)
    
#############################################以首字母进行行的排序,生成《莎士比亚14行诗ff.txt》
fp01 = open(obj_txt_f, "r")
fp02 = open(obj_txt_ff, "w")
a = [] 
b = []      
for line01 in fp01:
    a.append(line01.strip('\n'))      # 去除首尾的回车  
b=sorted(a, key=lambda x:int(x.split()[0]))
for line02 in b:
    fp02.write(line02)
    fp02.write('\n')
fp01.close()
fp02.close()
#############################################删除行首的数字，生成《莎士比亚14行诗.txt》
fp02 = open(obj_txt_ff, "r")
fp03 = open(obj_txt_fff, "w")
#c = []    
lines2 = fp02.readlines()
for line2 in lines2:                                 #flist.append(allpath)
	fp03.writelines(re.sub('\d+'+" ","",line2,1))	 #replace不能用正则表达式
	#fp03.write('\n')

fp02.close()
fp03.close()   

os.remove(obj_txt)      #删除《莎士比亚14行诗2.txt》
os.remove(obj_txt_f)    #删除《莎士比亚14行诗f.txt》 
os.remove(obj_txt_ff)   #删除《莎士比亚14行诗ff.txt》

 
        
        
        
        
        

import os,shutil

#find_txt和find_txt_recursive方法用来找txt文件，前者用walk方法，后者用递归操作，两者功能一样，任选一个
def find_txt(d):
    if not os.path.isdir(d):
        print('dir wrong!')
        return
    txt_list = []
    for root, _, files in os.walk(d):
        for file in files:
            if file.endswith('.txt'):
                txt_list.append(os.path.join(root,file))
    return txt_list

def find_txt_recursive(d,txt_list):
    if not os.path.isdir(d):
        print('dir wrong!')
        return
    fd = os.listdir(d)
    if 0 == len(fd):
        return
    files = [os.path.join(d,f) for f in fd if os.path.isfile(os.path.join(d,f))]
    txt_list += files
    dirs =  [os.path.join(d,f) for f in fd if os.path.isdir(os.path.join(d,f))]
    if 0 == len(dirs):
        return
    for dd in dirs:
        find_txt_recursive(dd,txt_list)   #对子文件夹做递归操作，调用自身函数
    return txt_list
    

def main():
    d=r'D:\myDocuments\Python作业'  #原始文件夹所在的路径，改成你自己的
    txt_list = [] #空列表，用来保存找到的txt文件的完整路径
    
    #下面的两个方法都可以，任选一个
    txt_list = find_txt(os.path.join(d,'原始文件夹'))
    #txt_list = find_txt_recursive(os.path.join(d,'原始文件夹'),txt_list)
    
    #新建两个文件夹
    before_combine_dir = os.path.join(d,r'目标文件夹\before-combine')
    after_combine_dir  = os.path.join(d,r'目标文件夹\after-combine')
    if not os.path.isdir(before_combine_dir):
        os.makedirs(before_combine_dir)
    if not os.path.isdir(after_combine_dir):
        os.makedirs(after_combine_dir)
        
    #把找到的txt文件移动到刚才新建的文件夹‘目标文件夹\before-combine’中
    for txt in txt_list:
        shutil.move(txt,before_combine_dir)
    
    #读取所有txt文件
    txt_files = os.listdir(before_combine_dir)
    texts = []
    for file in txt_files:
        with open(os.path.join(before_combine_dir,file),'r') as f:
            texts += f.readlines()
            
    #有的行结尾没有换行符，需要添加换行符，以便后面写入新文件时格式不变
    texts = [t if t.endswith('\n') else t+'\n' for t in texts]
    #将每一行前面的数字与后面文本分开然后存储为一个元组
    texts = [(int(t.split(' ',1)[0]),t.split(' ',1)[1]) for t in texts]
        
    #按照元组的第0个排序后，只保留元组的第1个
    texts.sort()
    texts = [t[1] for t in texts]
    
    #写入新文件
    with open(os.path.join(after_combine_dir,'莎士比亚14行诗.txt'),'w') as f:
        f.writelines(texts)
        
    #此时旧文件夹都为空，直接删除
    shutil.rmtree(os.path.join(d,'原始文件夹'))
    return
    
if __name__ == "__main__":
    main()


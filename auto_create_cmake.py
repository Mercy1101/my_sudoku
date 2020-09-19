# 自动生成 CMakeLists.txt，把该文件放到项目根目录中
import os

project_name = input("请输入项目名称: ")  # TestAlgorithm
cur_abs_path = os.getcwd()
input_dir_name = input("请输入要遍历的项目根目录中的文件目录名(之后会遍历该目录中所有 c 和 cpp 文件: ")
input_dir_abs_path = os.path.join(cur_abs_path, input_dir_name)
main_file_name = input("请输入根目录 main 函数所在的文件名: ")
cmakeFileAbsPath = os.path.join(cur_abs_path, "CMakeLists.txt")
target_dir_list_result = []

def listTargetDir(path):
    print("listTargetDir path = " + path)
    if not (os.path.isdir(path)):  # 是文件
        if os.path.basename(path).find(".c") != -1:
            print("filePathList.append path=" + path)
            target_dir_list_result.append(path)
    else:  # 是目录
        for childDir in os.listdir(path):
            listTargetDir(os.path.join(path,childDir))

listTargetDir(input_dir_abs_path)

# 添加根目录
target_dir_list_result.append(os.path.join(cur_abs_path,main_file_name))

# 添加遍历后的 .c / .cpp 文件到 cmake_file_list_result 中
cmake_file_list_result = []
for absFilePath in target_dir_list_result:  # 遍历文件夹
    result = absFilePath.split(project_name + "\\")[1]
    cmake_file_list_result.append(result)

cmake_content = '''cmake_minimum_required(VERSION 3.16)
project(''' + project_name + ''')

set(CMAKE_CXX_STANDARD 14)

add_executable( ''' + project_name + " "

for filePath in cmake_file_list_result:
    print("filePath = " + filePath)
    cmake_content += filePath
    cmake_content += " "

cmake_content += ")"

with open(cmakeFileAbsPath, 'w') as f:
    print("cmake_content = " + cmake_content)
    f.write(cmake_content)
#!/bin/zsh

#
#   COMMAND
#

# Clean command
if [ "$1" = "--clean" ]; then
    # 특정 폴더 내의 모든 파일 제거
    cd kernel
    
    # 지울 파일들의 갯수 저장
    file_count_before=$(find . -maxdepth 3 -type f | wc -l)

    # 파일 제거
    rm -rf *.o lib/graphics/*.o lib/terminal/terminal.o
    rm -rf .main.d
    rm -rf kernel.elf
    rm -rf lib/io/pci.o
    rm -rf lib/io/io_func.o

    # 제거된 파일들의 갯수 계산
    file_count_after=$(find . -maxdepth 1 -type f | wc -l)
    files_removed=$((file_count_before - file_count_after))


    echo "Remove $files_removed files"
    cd ..
    exit 0
fi



##
# build.sh  
##


### deprecated!!! ###
# Step 1 - generate alias (run qemu)
# echo -e "\nGenerate qemu command >> type \033[1mqemu_charon\033[0m\n"
# source ./source.sh

# 더 이상 지원하지 않는 방법. 터미널에서 source ./source.sh를 실행할 것


# Step 2 - excute buildenv.sh
source ./source.sh
    if [ $? -eq 0 ]; then
    echo "buildenv.sh execute. \033[1mSuccess..\033[0m"
    else
    echo -e "\e[1;31mError\e[0m: buildenv.sh failed to execute."
    exit 0
    fi

# Step 3 - excute makefile
cd kernel
make

    # check make process
    if [ $? -eq 0 ]; then
        echo "\n\033[1mBuild Success\033[0m"
    else
        echo "\n\033[1mfailed to make... Stop.\033[0m"
        exit 0
    fi
cd ..


# Step 4 - mv font files
cp kernel/kernelFont.bin kernel/lib/font
cp kernel/kernelFont.o kernel/lib/font
cp kernel/.kernelFont.d kernel/lib/font

    # check mv process
    if [ $? -eq 0 ]; then
        echo -e "\n-"
    else
        # 오류 코드와 함께 실패 메시지 출력
        error_code=$?
        echo "failed to move file (error code: $error_code)"
        case $error_code in
            1) echo "1";;
            2) echo "2";;
            256) echo "256";;
            *) echo "undefined error";;
        esac
    fi


# print success message
echo -e "\n\033[1mAll Done!!\033[0m"





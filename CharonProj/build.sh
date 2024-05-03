#!/bin/zsh

#
#   COMMAND
#

### Config vars ###
clean=false
dclean=false
build=false
run=false
help=false


### define funcs ###
clean_func() {

    # 특정 폴더 내의 모든 파일 제거
    cd kernel
    
    # 지울 파일들의 갯수 저장
    file_count_before=$(find . -maxdepth 3 -type f | wc -l)

    # 파일 제거
    rm -rf *.o lib/graphics/*.o lib/font/*.o lib/terminal/terminal.o
    rm -rf kernelFont.bin kernelFont.o
    rm -rf kernel.elf
    rm -rf lib/font/kernelFont.bin
    rm -rf lib/io/pci.o
    rm -rf lib/io/io_func.o
    rm -rf lib/mouse/mouse.o
    rm -rf lib/log/logger.o

    # 제거된 파일들의 갯수 계산
    file_count_after=$(find . -maxdepth 1 -type f | wc -l)
    files_removed=$((file_count_before - file_count_after))


    echo "Remove $files_removed files"
    cd ..

}

dclean_func() {

    # 특정 폴더 내의 모든 파일 제거
    cd kernel
    
    # 지울 파일들의 갯수 저장
    file_count_before=$(find . -maxdepth 3 -type f | wc -l)

    # 의존성 파일(.d) 제거
    rm .main.d
    rm .newlib_support.d
    rm .libcxx_support.d
    rm lib/font/.font.d lib/font/.kernelFont.d
    rm lib/graphics/.graphics.d
    rm lib/io/.pci.d
    rm lib/log/.logger.d
    rm lib/mouse/.mouse.d
    rm lib/terminal/.terminal.d

    # 제거된 파일들의 갯수 계산
    file_count_after=$(find . -maxdepth 1 -type f | wc -l)
    files_removed=$((file_count_before - file_count_after))


    echo "Remove $files_removed .d files"
    cd ..

}


### BUILD PROCESS ###

build_func() {

    # Step 0 - Take permission
    chmod +x tools/makefont.py


    # Step 1 - make font
    python3 tools/makefont.py -o kernelFont.bin kernel/lib/font/kernelFont.txt
    objcopy -I binary -O elf64-x86-64 -B i386:x86-64 kernelFont.bin kernelFont.o
    mv -f kernelFont.bin kernel/lib/font
    mv -f kernelFont.o kernel/lib/font



    # Step 2 - excute buildenv.sh %% source.sh
    source devenv/buildenv.sh
        if [ $? -eq 0 ]; then
        echo "buildenv.sh execute. \033[1mSuccess..\033[0m"
        else
        echo -e "\e[1;31mError\e[0m: buildenv.sh failed to execute."
        exit 0
        fi

    source ./source.sh
        if [ $? -eq 0 ]; then
        echo "source.sh execute. \033[1mSuccess..\033[0m"
        else
        echo -e "\e[1;31mError\e[0m: source.sh failed to execute."
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


    # deprecated!! 이 방법은 step 0의 단계로 대체됨
    # Step 4 - mv font files
    #    mv kernel/kernelFont.bin kernel/lib/font
    #    mv kernel/kernelFont.o kernel/lib/font
    #    mv kernel/.kernelFont.d kernel/lib/font

            # check mv process
    #        if [ $? -eq 0 ]; then
    #            echo -e "\n-"
    #        else
    #           # 오류 코드와 함께 실패 메시지 출력
    #            error_code=$?
    #            echo "failed to move file (error code: $error_code)"
    #            case $error_code in
    #                1) echo "1";;
    #                2) echo "2";;
    #                256) echo "256";;
    #                *) echo "undefined error";;
    #            esac
    #        fi

}

run_func() {
    $HOME/workspace/CharonProj/devenv/run_qemu.sh $HOME/edk2/Build/CharonLoaderX64/DEBUG_GCC/X64/Loader.efi $HOME/workspace/CharonProj/kernel/kernel.elf
}



### set options ###
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --clean)  clean=true;;
        --dclean) dclean=true;;
        --build)  build=true;;
        --run)    run=true;;
        --help)   help=true;;
        *) echo "Unknown option: $1"; exit 1;;
    esac
    shift
done



# 옵션에 따라 실행
if [ "$clean" = true ]; then
    clean_func
fi

if [ "$dclean" = true ]; then
    dclean_func
fi

if [ "$build" = true ]; then
    build_func
fi

if [ "$run" = true ]; then
    run_func
fi


if [ "$help" = true ]; then
    echo -e "\n\033[1mOptions...\n\033[0m"

    echo -e "1. \033[1m--build\033[0m: build CharonOS\n"
    echo -e "2. \033[1m--run\033[0m: run CharonOS from Qemu\n"
    echo -e "3. \033[1m--clean\033[0m: clean all of object files(.o file)\n"
    echo -e "4. \033[1m--dclean\033[0m: clean all of .d files"
fi

# alert end of process
echo -e "\n\n\033[1mProcess end!!\033[0m"





#!/bin/zsh
# zsh 옵션 설정
setopt nullglob

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
    mv *.o                          ../trash 2>/dev/null
    mv lib/graphics/*.o             ../trash 2>/dev/null
    mv lib/font/*.o                 ../trash 2>/dev/null
    mv lib/terminal/terminal.o      ../trash 2>/dev/null 
    mv kernelFont.bin kernelFont.o  ../trash 2>/dev/null
    mv kernel.elf                   ../trash 2>/dev/null
    mv lib/font/kernelFont.bin      ../trash 2>/dev/null
    mv lib/io/pci.o                 ../trash 2>/dev/null
    mv lib/io/io_func.o             ../trash 2>/dev/null
    mv lib/mouse/mouse.o            ../trash 2>/dev/null
    mv lib/log/logger.o             ../trash 2>/dev/null

    # 제거된 파일들의 갯수 계산
    file_count_after=$(find . -maxdepth 1 -type f | wc -l)
    files_removed=$((file_count_before - file_count_after))


    echo "Remove $files_removed .o files"
    cd ..

}

dclean_func() {

    # 특정 폴더 내의 모든 파일 제거
    cd kernel
    
    # 지울 파일들의 갯수 저장
    file_count_before=$(find . -maxdepth 3 -type f | wc -l)

    # 의존성 파일(.d) 제거
    mv .main.d                              ../trash 2>/dev/null
    mv .newlib_support.d                    ../trash 2>/dev/null
    mv .libcxx_support.d                    ../trash 2>/dev/null
    mv lib/font/.font.d                     ../trash 2>/dev/null
    mv lib/font/.kernelFont.d               ../trash 2>/dev/null
    mv lib/graphics/.graphics.d             ../trash 2>/dev/null
    mv lib/log/.logger.d                    ../trash 2>/dev/null
    mv lib/mouse/.mouse.d                   ../trash 2>/dev/null
    mv lib/terminal/.terminal.d             ../trash 2>/dev/null
    mv lib/pci/.pci.d                       ../trash 2>/dev/null
    mv lib/interrupt/.interrupt.d           ../trash 2>/dev/null
    mv lib/memory/MMR/.memory_manager.d     ../trash 2>/dev/null
    

    # 제거된 파일들의 갯수 계산
    file_count_after=$(find . -maxdepth 1 -type f | wc -l)
    files_removed=$((file_count_before - file_count_after))


    echo "Remove $files_removed .d files"
    cd ..

}

# 현재 터미널의 가로 길이를 가져오는 함수
get_terminal_width() {
    tput cols
}

# 가로 길이를 이용하여 '-'로 채운 라인을 생성하는 함수
generate_horizontal_line() {
    local width=$1
    printf "%-${width}s" "-" | tr ' ' '-'
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
        exit 1
        fi

    source ./source.sh
        if [ $? -eq 0 ]; then
        echo "source.sh execute. \033[1mSuccess..\033[0m"
        else
        echo -e "\e[1;31mError\e[0m: source.sh failed to execute."
        exit 1
        fi


    # Step 3 - excute makefile
    cd kernel
    make

        # check make process
        if [ $? -eq 0 ]; then
            echo "\n\033[1mBuild Success\033[0m"
        else
            echo "\n\033[1mfailed to make... Stop.\033[0m"
            exit 1
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
    if [ $? -ne 0 ]; then
        echo -e "\033[1m\nfailed to excute QEMU | Error code: $?\033[0m" >&2
        exit 1
    fi
    exit 0
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




### 쓰레기통 기능 추가 ###

# 쓰레기통 폴더 경로 설정
TRASH_DIR="$(dirname "$0")/trash"

# /tmp 내부의 manage_trash 파일 경로 설정
TRASH_COUNT_FILE="/tmp/manage_trash"

# manage_trash 파일이 존재하지 않는 경우 새로 생성하고 0으로 초기화
if [ ! -f "$TRASH_COUNT_FILE" ]; then
    echo 0 > "$TRASH_COUNT_FILE"
fi

# manage_trash 파일에서 build.sh 실행 횟수 얻기
count=$(cat "$TRASH_COUNT_FILE")

# 현재 터미널의 가로 길이 계산
width=$(get_terminal_width)
line=$(generate_horizontal_line "$width")


# count가 3인 경우, 쓰레기통 폴더 내 모든 내용 삭제 및 count를 0으로 재설정
if [ "$count" -eq 3 ]; then
    file_count=$(find "$TRASH_DIR" -type f | wc -l)
    if [ "$file_count" -eq 0 ]; then
        echo ""
        echo 0 > "$TRASH_COUNT_FILE"
    else

        echo 0 > "$TRASH_COUNT_FILE"
        echo -e "\n"
        echo -e "$line"
        echo "Clean Trash...\n"
        echo "delete " $TRASH_DIR/.*.d
        echo $TRASH_DIR/*.o   
        echo $TRASH_DIR/*.elf
        echo $TRASH_DIR/*.bin

        rm -rf "$TRASH_DIR"/.*.d 2>/dev/null ; rm -rf "$TRASH_DIR"/*.o 2>/dev/null ; rm -rf "$TRASH_DIR"/*.elf 2>/dev/null ; rm -rf "$TRASH_DIR"/*.bin 2>/dev/null    
        echo -e "$line"

    fi

elif [ "$count" -eq 2 ]; then
    # 쓰레기통 디렉토리에 파일이 있는지 확인
    file_count=$(find "$TRASH_DIR" -type f | wc -l)
    if [ "$file_count" -eq 0 ]; then
        # 파일이 없는 경우 경고 메시지를 출력하지 않음
        ((count++))
        echo $count > "$TRASH_COUNT_FILE"
    else
        # 파일이 있는 경우 경고 메시지 출력
        echo "The next time you run the build.sh file, the contents(old .o, .d files) of the trash folder will be deleted."
        ((count++))
        echo $count > "$TRASH_COUNT_FILE"
    fi

else
    # 그 외의 경우, count를 1 증가시키고 파일에 저장
    ((count++))
    echo $count > "$TRASH_COUNT_FILE"
fi





# alert end of process
echo -e "\n\n\033[1mProcess end!!\033[0m"
clear
read -p "file name: " INPUT
test_count=0 # 循环测试次数
change_count=0 # 文件大小改变次数
if [ -e $INPUT ]
then
    status=$(ls -l $INPUT)
    echo "file status: $status"
    size=$(ls -l $INPUT | cut -d " " -f 5)
    while [ $test_count -lt 10 -a $change_count -lt 2 ]
    do
        size2=$(ls -l $INPUT | cut -d " " -f 5)
        if [ $size -eq $size2 ]
        then
            echo "test file's status ..."
            test_count=`expr $test_count + 1`
        else
            test_count=0
            change_count=`expr $change_count + 1`
            size=$size2
            echo "file [$INPUT] size changed"
        fi
        sleep 5
    done

    if [ $test_count -eq 10 ]
    then
        echo "test number exceed 10!"
    else
        echo "file size change exceed 2!"
    fi
else
    echo "can not find the file [$INPUT]!"
fi

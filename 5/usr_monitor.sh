if [ $# -eq 1 ] # 判断参数1是否存在
then
    echo "current user list:"
    who
    name=`who|grep $1` # 获取指定用户的登录信息
    while [ -z "$name" ]
    do
        echo "waiting user [$1] …"
        sleep 5
        name=`who|grep $1`
    done
    echo "user [$1] is logon"
else
    echo "Usage: usr_monitor <username>"
fi
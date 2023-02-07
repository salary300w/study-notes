//实现TCP通信的服务器端

#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
int main()
{
    auto listen_fd = socket(AF_INET, SOCK_STREAM, 0); //创建监听socket文件
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    inet_pton(AF_INET, "192.168.204.133", &addr.sin_addr.s_addr);
    addr.sin_port = htons(9999);
    bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)); //链接地址和端口号
    listen(listen_fd, 5); //设置监听是否连接
    struct sockaddr_in addr_out;
    int addr_len = sizeof(addr_out);
    auto connect_fd = accept(listen_fd, (struct sockaddr *)&addr_out, (socklen_t *)&addr_len); //与客户端建立连接
    char write_buf[1024];
    char read_buf[1024];
    auto fork_key = fork();
    if (fork_key > 0)
    {
        while (1)
        {
            std::cin >> write_buf;
            if (strlen(write_buf) > 0)
            {
                auto write_key = write(connect_fd, &write_buf, strlen(write_buf));
                bzero(write_buf, sizeof(write_buf));
            }
        }
    }
    else if (fork_key == 0)
    {
        while (1)
        {
            auto read_key = read(connect_fd, &read_buf, sizeof(read_buf));
            if (read_key > 0)
            {
                std::cout << "收到的数据为：" << read_buf << "\n";
                bzero(read_buf, sizeof(read_buf));
            }
            else if (read_key == -1)
                break;
        }
    }
    else
    {
        exit(0);
    }
    return 0;
}
#include "../include/H_thread_pool.h"

#define THREAD_NUM "5"
#define MAX_CAPACITY 1024

int main(int argc, char* argv[])
{
    int sfd = tcp_init(ROUTE_IP, ROUTE_PORT);

    int epfd = epoll_create1(0);

    int thread_num = atoi(THREAD_NUM);

    thread_pool_t *p_manager = thread_pool_init(thread_num);

    struct epoll_event evs[MAX_CAPACITY] = {0};

    epoll_add(epfd, sfd);

    while(1)
    {
        int ready_fd_num = epoll_wait(epfd, evs, MAX_CAPACITY, 1000);
        
        time_t cur_time = time(NULL);

        for(int i = 0; i < ready_fd_num; i++)
        {
            if(evs[i].data.fd == sfd)
            {
                // 有新客户端连接
                elem_t new_client = {0};
                
                new_client.fd = tcp_accept(sfd);
                
                if(new_client.fd != -1)
                {
                    new_client.conn_time = time(NULL);
                    distribute_task(p_manager, new_client);
                }
            }
        }// end search evs[i].data.fd

        // TIME_OUT
        for(int i = 0; i < MAX_CAPACITY; i++)
        {
            
        }
    }// end while(1)

}
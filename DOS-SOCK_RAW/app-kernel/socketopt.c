#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/types.h>  
#include <linux/string.h>  
#include <linux/netfilter_ipv4.h>  
#include <linux/init.h>  
#include <asm/uaccess.h>  
  
#define SOCKET_OPS_BASE          128  
#define SOCKET_OPS_SET       (SOCKET_OPS_BASE)  
#define SOCKET_OPS_GET      (SOCKET_OPS_BASE)  
#define SOCKET_OPS_MAX       (SOCKET_OPS_BASE + 1)  
  
#define KMSG          "a message from kernel"  
#define KMSG_LEN      sizeof("a message from kernel")  
  
MODULE_LICENSE("GPL");  
  
static int recv_msg(struct sock *sk, int cmd, void *user, unsigned int len)  
{  
    int ret = 0;  
    printk(KERN_INFO "sockopt: recv_msg()\n");   
    /* 
    switch(cmd) 
    { 
    case IMP1_SET: 
    { 
        char umsg[64]; 
        memset(umsg, 0, sizeof(char)*64); 
        copy_from_user(umsg, user, sizeof(char)*64); 
        printk("umsg: %s", umsg); 
    } 
    break; 
    } 
    */  
    if (cmd == SOCKET_OPS_SET)  
    {  
        char umsg[64];  
        int len = sizeof(char)*64;  
        memset(umsg, 0, len);  
        ret = copy_from_user(umsg, user, len);  
        printk("recv_msg: umsg = %s. ret = %d\n", umsg, ret);          
    }  
    return 0;  
}  
  
static int send_msg(struct sock *sk, int cmd, void *user, int *len)  
{  
    int ret = 0;  
    printk(KERN_INFO "sockopt: send_msg()\n");   
    if (cmd == SOCKET_OPS_GET)  
    {  
        ret = copy_to_user(user, KMSG, KMSG_LEN);  
        printk("send_msg: umsg = %s. ret = %d. success\n", KMSG, ret);      
    }  
    return 0;  
}  
  
static struct nf_sockopt_ops test_sockops =  
{  
    .pf = PF_INET,  
    .set_optmin = SOCKET_OPS_SET,  
    .set_optmax = SOCKET_OPS_MAX,  
    .set = recv_msg,  
    .get_optmin = SOCKET_OPS_GET,  
    .get_optmax = SOCKET_OPS_MAX,  
    .get = send_msg,  
};  
  
static int __init init_sockopt(void)  
{  
    printk(KERN_INFO "sockopt: init_sockopt()\n");   
    return nf_register_sockopt(&test_sockops);  
}  
  
static void __exit fini_sockopt(void)  
{  
    printk(KERN_INFO "sockopt: fini_sockopt()\n");   
    nf_unregister_sockopt(&test_sockops);  
}  
  
module_init(init_sockopt);   
module_exit(fini_sockopt);  
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/tcp.h>
#include <linux/ip.h>
#include <linux/byteorder/generic.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jussi Hietanen");
MODULE_DESCRIPTION("Ignores TCP FIN flags in TCP streams");

static struct nf_hook_ops nfhook;

static unsigned int ignored_fins; 

unsigned int ignore_func(void* priv, struct sk_buff* skb, const struct nf_hook_state* state)
{
  struct iphdr* iph;
  struct tcphdr* tcph;

  if (!skb)
    return NF_ACCEPT;

  iph = (struct iphdr*)skb_network_header(skb);

  if(iph->protocol == IPPROTO_TCP)
  {
    tcph = (struct tcphdr*)skb_transport_header(skb);
    if(tcph->fin == 1)
    {
      tcph->fin = 0;
      ignored_fins++;
    }
  }
  return NF_ACCEPT;
}

static int __init loader(void)
{
	nfhook.hook = ignore_func;
  nfhook.hooknum = NF_INET_PRE_ROUTING;
  nfhook.pf = PF_INET;
  nfhook.priority = NF_IP_PRI_FIRST;
  
  nf_register_net_hook(&init_net, &nfhook);

  ignored_fins = 0;

  printk(KERN_INFO "Started ignoring FIN in TCP.\n");

  return 0;
}


static void __exit unloader(void)
{
  nf_unregister_net_hook(&init_net, &nfhook);
  printk(KERN_INFO "FIN ignore Module removed\n");
  printk(KERN_INFO "Total removed FINs: %d\n", ignored_fins);
}


module_init(loader);
module_exit(unloader);
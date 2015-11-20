#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

#define LIST_SIZE 5

static struct my_list 
{
	int number;
	struct list_head list;
};

static struct my_list klist;

static int klist_init(void)
{
	struct my_list *tmp, *ptr;
	unsigned int i;

	printk(KERN_INFO "initialize kernel module\n");
	INIT_LIST_HEAD(&klist.list);

	for (i = 0; i < LIST_SIZE; i++) {
		tmp = (struct my_list*) kmalloc(sizeof(struct my_list), GFP_KERNEL);
		if (likely(tmp)) {
			tmp->number = i;
			INIT_LIST_HEAD(&tmp->list);
			list_add_tail(&(tmp->list), &(klist.list));
		}
	}

	printk(KERN_INFO "traversing the list\n");

	list_for_each_entry(ptr, &klist.list, list) {
		printk(KERN_INFO "%d ", ptr->number);
	}
	printk(KERN_INFO "\n");

	return 0;
}

static void klist_cleanup(void)
{
	struct my_list *tmp, *ptr;
	printk(KERN_INFO "Cleanup klist module\n");
	printk(KERN_INFO "deleting the list\n");

	list_for_each_entry_safe(tmp, ptr, &klist.list, list) {
		printk(KERN_INFO "free node\n");
		list_del(&tmp->list);
		kfree(tmp);
	}
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Miroslav Tisma <tisma@linux.com>");
MODULE_DESCRIPTION("Simple kernel module which demonstrates using of kernel list");
module_init(klist_init);
module_exit(klist_cleanup);

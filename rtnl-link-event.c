/* This example is placed in the public domain. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <libmnl/libmnl.h>
#include <linux/if.h>
#include <linux/if_link.h>
#include <linux/rtnetlink.h>

static int data_attr_cb(const struct nlattr *attr, void *data)
{
	const struct nlattr **tb = data;
	int type = mnl_attr_get_type(attr);

	/* skip unsupported attribute in user-space */
	if (mnl_attr_type_valid(attr, IFLA_MAX) < 0)
		return MNL_CB_OK;

	switch(type) {
	case IFLA_MTU:
		if (mnl_attr_validate(attr, MNL_TYPE_U32) < 0) {
			perror("mnl_attr_validate");
			return MNL_CB_ERROR;
		}
		break;
	case IFLA_IFNAME:
		if (mnl_attr_validate(attr, MNL_TYPE_STRING) < 0) {
			perror("mnl_attr_validate2");
			return MNL_CB_ERROR;
		}
		break;
	}
	tb[type] = attr;
	return MNL_CB_OK;
}

static int data_cb(const struct nlmsghdr *nlh, void *data)
{
	struct nlattr *tb[IFLA_MAX+1] = {};
	struct ifinfomsg *ifm = mnl_nlmsg_get_payload(nlh);

	printf("index=%d type=%d flags=%d family=%d ", 
		ifm->ifi_index, ifm->ifi_type,
		ifm->ifi_flags, ifm->ifi_family);

	if (ifm->ifi_flags & IFF_RUNNING)
		printf("[RUNNING] ");
	else
		printf("[NOT RUNNING] ");

	mnl_attr_parse(nlh, sizeof(*ifm), data_attr_cb, tb);
	if (tb[IFLA_MTU]) {
		printf("mtu=%d ", mnl_attr_get_u32(tb[IFLA_MTU]));
	}
	if (tb[IFLA_IFNAME]) {
		printf("name=%s", mnl_attr_get_str(tb[IFLA_IFNAME]));
	}
	printf("\n");
	return MNL_CB_OK;
}

struct mnl_socket *
dp_netlink_open(void)
{
	struct mnl_socket *nl;
        nl = mnl_socket_open(NETLINK_ROUTE);
        if (nl == NULL) {
                perror("mnl_socket_open");
                exit(EXIT_FAILURE);
        }
	printf("socket open successful\n");
        if (mnl_socket_bind(nl, RTMGRP_LINK, MNL_SOCKET_AUTOPID) < 0) {
                perror("mnl_socket_bind");
                exit(EXIT_FAILURE);
        }
	printf("socket bind successful\n");
        if (fcntl(mnl_socket_get_fd(nl), F_SETFL, O_NONBLOCK) < 0){
                perror("unable to set socket as non-blocking");
                exit(EXIT_FAILURE);
	}
	printf("set socket non-blocking successful\n");
	return nl;
}

int main(void)
{
	struct mnl_socket *nl;
	char buf[MNL_SOCKET_BUFFER_SIZE];
	int ret;

	nl = dp_netlink_open();
	while (1){
		do{
			ret = mnl_socket_recvfrom(nl, buf, sizeof(buf));
			printf("retry\n");
			sleep(1);
		}while(ret < 0 && errno == EAGAIN);
		printf("out of do while\n");
		ret = mnl_cb_run(buf, ret, 0, 0, data_cb, NULL);
		if (ret <= 0) {
			mnl_socket_close(nl);
			perror("error");
			exit(EXIT_FAILURE);
		}
	}
}

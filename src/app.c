/*
 * Class A LoRaWAN sample application
 *
 * Copyright (c) 2020 Manivannan Sadhasivam <mani@kernel.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <lorawan/lorawan.h>
#include <zephyr.h>
#include <logging/log.h>
#include "lorawanConst.h"

#define DEFAULT_LORA_NODE DT_ALIAS(lora0)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_LORA_NODE, okay),
             "LoRa not specified in the device tree");

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL

LOG_MODULE_REGISTER(simple_lorawan_abp_example);

char data[] = {'h','i','\0'};




void main(void)
{
    const struct device *lora_dev;
    struct lorawan_join_config join_cfg;
    uint8_t dev_eui[] = LORAWAN_DEV_EUI;
    uint8_t apps_key[] = LORAWAN_APS_KEY;
    uint8_t ns_key[] = LORAWAN_NS_KEY;
    uint8_t join_eui[] = LORAWAN_JOIN_EUI;

    int err;

    lora_dev = device_get_binding(DT_LABEL(DEFAULT_LORA_NODE));
    if (!lora_dev)
    {
        LOG_ERR("%s device not found", DEFAULT_RADIO);
        return;
    }

    err = lorawan_start();
    if (err < 0)
    {
        LOG_ERR("start lorawan failed: %d", ret);
        return;
    }


    join_cfg.mode = LORAWAN_ACT_ABP;

    join_cfg.dev_eui = dev_eui;
    join_cfg.abp.app_eui = join_eui;
    join_cfg.abp.app_skey = apps_key;
    join_cfg.abp.nwk_skey = ns_key;
    join_cfg.abp.dev_addr = LORAWAN_DEVADDR;

    LOG_INF("Joining network over ABP");
    err = -1;
    while (err < 0)
    {
        err = lorawan_join(&join_cfg);
        k_sleep(K_MSEC(10000));
    }
   
    if (err < 0)
    {
        LOG_ERR("lorawan_join_network failed: %d", err);
        return;
    }

    LOG_INF("Sending data...");
    while (1)
    {
        err = lorawan_send(2, data, sizeof(data),
                           LORAWAN_MSG_CONFIRMED);
        if (err < 0)
        {
            LOG_ERR("lorawan_send failed: %d. Continuing...", err);
            k_sleep(K_MSEC(10000));
            continue;
        }


        LOG_INF("Data sent!");
        k_sleep(K_MSEC(10000));
    }
}

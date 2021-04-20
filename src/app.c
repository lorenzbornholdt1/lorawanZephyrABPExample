/*
 * Class A LoRaWAN sample application
 *
 * Copyright (c) 2020 Manivannan Sadhasivam <mani@kernel.org>
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <lorawan/lorawan.h>
#include <zephyr.h>
#include "lorawanConst.h"

#define DEFAULT_RADIO_NODE DT_ALIAS(lora0)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_RADIO_NODE, okay),
             "No default LoRa radio specified in DT");
#define DEFAULT_RADIO DT_LABEL(DEFAULT_RADIO_NODE)

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(lorawan_class_a);

char data[] = {'h','i','\0'};


#define DELAY K_MSEC(10000)

void main(void)
{
    const struct device *lora_dev;
    struct lorawan_join_config join_cfg;
    uint8_t dev_eui[] = LORAWAN_DEV_EUI;
    uint8_t apps_key[] = LORAWAN_APS_KEY;
    uint8_t ns_key[] = LORAWAN_NS_KEY;
    uint8_t join_eui[] = LORAWAN_JOIN_EUI;

    int ret;

    lora_dev = device_get_binding(DEFAULT_RADIO);
    if (!lora_dev)
    {
        LOG_ERR("%s Device not found", DEFAULT_RADIO);
        return;
    }

    ret = lorawan_start();
    if (ret < 0)
    {
        LOG_ERR("lorawan_start failed: %d", ret);
        return;
    }



    join_cfg.mode = LORAWAN_ACT_ABP;
    join_cfg.dev_eui = dev_eui;
    join_cfg.abp.app_eui = join_eui;
    join_cfg.abp.app_skey = apps_key;
    join_cfg.abp.nwk_skey = ns_key;
    join_cfg.abp.dev_addr = LORAWAN_DEVADDR;

    LOG_INF("Joining network over ABP");
    ret = -1;
    while (ret < 0)
    {
        ret = lorawan_join(&join_cfg);
        k_sleep(DELAY);
    }
   
    if (ret < 0)
    {
        LOG_ERR("lorawan_join_network failed: %d", ret);
        return;
    }

    LOG_INF("Sending data...");
    while (1)
    {
        ret = lorawan_send(2, data, sizeof(data),
                           LORAWAN_MSG_CONFIRMED);
        if (ret < 0)
        {
            LOG_ERR("lorawan_send failed: %d. Continuing...", ret);
            k_sleep(DELAY);
            continue;
        }


        LOG_INF("Data sent!");
        k_sleep(DELAY);
    }
}

#include "mBle.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h" 
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h" 

void mBle_host_task(void *param)
{
	nimble_port_run();
	//nimble_port_freertos_deinit();
}

void mBle_hs_sync()
{
	ESP_LOGI("mBle","synced\n");
}

void mBle_hs_reset(int reason)
{
	ESP_LOGI("mBle","reset reason %d\n", reason);
}

void mBle_init()
{
	// esp_nimble_hci_and_controller_init();
	// nimble_port_init();

	// ble_hs_cfg.sync_cb = mBle_hs_sync;
	// ble_hs_cfg.reset_cb = mBle_hs_reset;
	// nimble_port_freertos_init(mBle_host_task);
}
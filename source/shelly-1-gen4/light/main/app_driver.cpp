/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <esp_matter.h>
#include <app_priv.h>
#include <common_macros.h>
#include <driver/gpio.h>
#include <device.h>
#include <thermal.h>

using namespace chip::app::Clusters;
using namespace esp_matter;

static const char *TAG = "app_driver";
extern uint16_t light_endpoint_id;

// Shelly 1 Gen4 GPIO assignments
#define RELAY_GPIO          GPIO_NUM_5

static esp_err_t app_driver_relay_set_power(bool power)
{
    if (thermal_is_faulted() && power) {
        ESP_LOGW(TAG, "Thermal fault active relay turn on blocked");
        return ESP_ERR_INVALID_STATE;
    }
    ESP_LOGI(TAG, "Setting relay: %s", power ? "ON" : "OFF");
    return gpio_set_level(RELAY_GPIO, power ? 1 : 0);
}

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;
    if (endpoint_id == light_endpoint_id) {
        if (cluster_id == OnOff::Id) {
            if (attribute_id == OnOff::Attributes::OnOff::Id) {
                err = app_driver_relay_set_power(val->val.b);
            }
        }
    }
    return err;
}

esp_err_t app_driver_light_set_defaults(uint16_t endpoint_id)
{
    esp_err_t err = ESP_OK;
    esp_matter_attr_val_t val = esp_matter_invalid(NULL);

    attribute_t *attribute = attribute::get(endpoint_id, OnOff::Id, OnOff::Attributes::OnOff::Id);
    attribute::get_val(attribute, &val);
    err = app_driver_relay_set_power(val.val.b);

    return err;
}

app_driver_handle_t app_driver_light_init()
{
    // Initialize relay GPIO
    gpio_config_t relay_cfg = {
        .pin_bit_mask = (1ULL << RELAY_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&relay_cfg);
    gpio_set_level(RELAY_GPIO, 0);

    return (app_driver_handle_t)1;
}
// Temporary shim used by the thermal monitor (thermal.cpp) until the
// relay code is extracted into its own module. When relay extraction is
// complete, this function should be deleted and thermal.cpp should call
// relay_set(false) directly.
void app_driver_force_relay_off(void)
{
    // 1. Direct hardware: drive relay GPIO low. This bypasses every
    //    layer above the GPIO peripheral and ensures the relay
    //    physically de-energizes regardless of Matter stack state.
    gpio_set_level(RELAY_GPIO, 0);

    // 2. Update Matter attribute so connected ecosystems see the change.
    //    This will trigger the attribute callback, which would normally
    //    call app_driver_relay_set_power(false), but the GPIO is already
    //    off so that's a no-op.
    uint32_t cluster_id = OnOff::Id;
    uint32_t attribute_id = OnOff::Attributes::OnOff::Id;
    esp_matter_attr_val_t val = esp_matter_bool(false);
    attribute::update(light_endpoint_id, cluster_id, attribute_id, &val);
}
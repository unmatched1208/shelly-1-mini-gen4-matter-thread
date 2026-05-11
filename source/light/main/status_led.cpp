#include "status_led.h"

#include <esp_log.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char *TAG = "status_led";

#define STATUS_LED_GPIO     GPIO_NUM_15
#define LED_ON_LEVEL        0   // Active-low: pin LOW = LED ON
#define LED_OFF_LEVEL       1   // Active-low: pin HIGH = LED OFF

static volatile led_state_t current_state = LED_STATE_OFF;

static void status_led_task(void *arg)
{
    bool led_on = false;
    uint32_t period_ms = 100;  // How often the task wakes up

    while (true) {
        led_state_t state = current_state;
        uint32_t now = xTaskGetTickCount() * portTICK_PERIOD_MS;

        switch (state) {
            case LED_STATE_OFF:
                gpio_set_level(STATUS_LED_GPIO, LED_OFF_LEVEL);
                break;

            case LED_STATE_BLE_ADVERTISING:
                // Rapid blink: 200ms on, 200ms off (400ms period)
                led_on = (now % 400) < 200;
                gpio_set_level(STATUS_LED_GPIO, led_on ? LED_ON_LEVEL : LED_OFF_LEVEL);
                break;

            case LED_STATE_THREAD_CONNECTING:
                // Slow blink: 500ms on, 1500ms off (2000ms period)
                led_on = (now % 2000) < 500;
                gpio_set_level(STATUS_LED_GPIO, led_on ? LED_ON_LEVEL : LED_OFF_LEVEL);
                break;

            case LED_STATE_THREAD_CONNECTED:
                gpio_set_level(STATUS_LED_GPIO, LED_ON_LEVEL);
                break;

            case LED_STATE_FAULT:
                // Very rapid blink: 100ms on, 100ms off (200ms period)
                led_on = (now % 200) < 100;
                gpio_set_level(STATUS_LED_GPIO, led_on ? LED_ON_LEVEL : LED_OFF_LEVEL);
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(period_ms));
    }
}

esp_err_t status_led_init(void)
{
    gpio_config_t led_cfg = {
        .pin_bit_mask = (1ULL << STATUS_LED_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    esp_err_t err = gpio_config(&led_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure LED GPIO: %d", err);
        return err;
    }

    // Start with LED off
    gpio_set_level(STATUS_LED_GPIO, LED_OFF_LEVEL);

    // Spawn LED task
    BaseType_t result = xTaskCreate(status_led_task, "status_led", 2048, NULL, 5, NULL);
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create LED task");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Status LED initialized on GPIO%d", STATUS_LED_GPIO);
    return ESP_OK;
}

void status_led_set_state(led_state_t state)
{
    if (state != current_state) {
        ESP_LOGI(TAG, "LED state change: %d -> %d", current_state, state);
        current_state = state;
    }
}
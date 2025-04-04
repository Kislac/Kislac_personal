#include <Arduino.h>
#include "esp_partition.h"

void print_partition_table() {
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);
    while (it != NULL) {
        const esp_partition_t *partition = esp_partition_get(it);
        Serial.printf("Name: %s, Type: %d, SubType: %d, Offset: 0x%08x, Size: 0x%08x\n",
                      partition->label, partition->type, partition->subtype, partition->address, partition->size);
        it = esp_partition_next(it);
    }
    esp_partition_iterator_release(it);

    it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
    while (it != NULL) {
        const esp_partition_t *partition = esp_partition_get(it);
        Serial.printf("Name: %s, Type: %d, SubType: %d, Offset: 0x%08x, Size: 0x%08x\n",
                      partition->label, partition->type, partition->subtype, partition->address, partition->size);
        it = esp_partition_next(it);
    }
    esp_partition_iterator_release(it);
}

void setup() {
    Serial.begin(115200);
    print_partition_table();
}

void loop() {
    // Nincs szükség loop kódra
}
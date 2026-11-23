/*!
* @file storage.cpp
* @brief Implementation for data storage class
* @authors John Doe
* @date 2026-11-09
* @version 1.0.0
*/
#include "storage.h"
#include <at24c256.h>

/*!
* @brief User's data slice
*/
struct DataSlice {
    /*!
    * @brief Slice status (see #SLOT_ACTIVE and #SLOT_EMPTY)
    */
    uint8_t Status;
    /*!
    * @brief Write operations counter
    */
    uint32_t WriteCounter;
    /*!
    * @brief Device user's configuration
    */
    GrowConfig Conf;
};

uint16_t Storage::alignSize(uint16_t sz) {
    if (sz <= 16) {
        return 16;
    }
    if (sz <= 32) {
        return 32;
    }
    if (sz <= 64) {
        return 64;
    }
    return ((sz + 31) / 32) * 32;
}

Storage::Storage() {
    size_t sz = sizeof(DataSlice);
    if (sz > END_ADDRESS) {
        this->isInited = false;
        return;
    }
    this->lastWriteCount = 0;
    this->curSlot = -1;
    this->slotSize = Storage::alignSize(sz);
    if (this->slotSize > END_ADDRESS) {
        this->isInited = false;
        return;
    }
    this->totalSlots = END_ADDRESS / this->slotSize;
    this->isInited = true;
}

bool Storage::IsInited() {
    return this->isInited;
}

uint8_t Storage::Read(GrowConfig &dest) {
    this->curSlot = -1;
    this->lastWriteCount = 0;
    uint32_t maxCounter = 0;
    int bestSlot = -1;
    
    for (uint16_t i = 0; i < this->totalSlots; i++) {
        uint16_t addr = i * this->slotSize;
        uint8_t status;
        uint8_t rc = EepromRead(addr, &status, 1);
        if (rc != 0) {
            return rc;
        }
        if (status == SLOT_ACTIVE) {
            uint32_t counter;
            rc = EepromRead(addr + offsetof(DataSlice, WriteCounter), (uint8_t*)&counter, sizeof(counter));
            if (rc != 0) {
                return rc;
            }
            if (counter >= maxCounter) {
                maxCounter = counter;
                bestSlot = i;
            }
        }
    }
    if (bestSlot > -1) {
        this->curSlot = bestSlot;
        this->lastWriteCount = maxCounter;
        DataSlice slice;
        uint8_t rc = EepromRead(this->curSlot * this->slotSize, (uint8_t*)&slice, sizeof(DataSlice));
        if (rc != 0) {
            return rc;
        } 
        dest = slice.Conf;
        return 0;
    }

    return 4;
}

uint8_t Storage::Write(const GrowConfig &src) {
    int nextSlot = (this->curSlot + 1) % this->totalSlots;
    this->lastWriteCount++;
    DataSlice slice;
    slice.Status = SLOT_ACTIVE;
    slice.WriteCounter = this->lastWriteCount;
    slice.Conf = src;
    uint16_t nextAddr = nextSlot * this->slotSize;
    uint8_t rc = EepromWrite(nextAddr, (const uint8_t*)&slice, sizeof(DataSlice));
    if (rc != 0) {
        this->lastWriteCount--;
        return rc;
    }
    DataSlice vfy_slice;
    rc = EepromRead(nextAddr, (uint8_t*)&vfy_slice, sizeof(DataSlice));
    if (rc != 0) {
        this->lastWriteCount--;
        return rc;
    }
    if (memcmp((uint8_t*)&slice, (uint8_t*)&vfy_slice, sizeof(DataSlice)) != 0) {
        this->curSlot = nextSlot;
        this->lastWriteCount--;
        return 4;
    }

    if (this->curSlot != -1 && this->curSlot != nextSlot) {
        uint16_t oldAddr = this->curSlot * this->slotSize;
        uint8_t disable = SLOT_EMPTY;
        EepromWrite(oldAddr + offsetof(DataSlice, Status), &disable, 1);
    }
    this->curSlot = nextSlot;
    return 0;
}


/*!
* @file controller.cpp
* @brief Implementation for the main Growbox controller class
* @authors John Doe
* @date 2026-11-10
* @version 1.0.0
*/
#include "controller.h"
#include "../include/common/datetime.h"
#include "common/commands.h"
#include "version.h"

/*!
* @brief Check sensors frequency in milliseconds
*/
#define CHECK_SENSORS_FREQ 1000

void Controller::onReceive(void *context, const uint8_t cmd, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len) {
    Controller *instance = (Controller*)context;
    if (instance == nullptr) {
        return;
    }
    switch (cmd) {
    case CMD_INFO:
        Controller::getInfo(instance, asw_data, asw_len);
        break;
    case CMD_GET_BT_INFO:
        Controller::getBTInfo(instance, asw_data, asw_len);
        break;
    case CMD_SET_BT_NAME:
        Controller::setBTName(instance, req_data, req_len, asw_data, asw_len);
        break;
    case CMD_SET_BT_PASS:
        Controller::setBTPass(instance, req_data, req_len, asw_data, asw_len);
        break;
    case CMD_GET_TIME:
        Controller::getTime(instance, asw_data, asw_len);
        break;
    case CMD_SET_TIME:
        Controller::setTime(instance, req_data, req_len, asw_data, asw_len);
        break;
    case CMD_GET_STATUS:
        Controller::getStatus(instance, req_data, req_len, asw_data, asw_len);
        break;
    case CMD_SET_STATUS:
        Controller::setStatus(instance, req_data, req_len, asw_data, asw_len);
        break;
    case CMD_GET_MODE:
        Controller::getMode(instance, asw_data, asw_len);
        break;
    case CMD_SET_MODE:
        Controller::setMode(instance, req_data, req_len, asw_data, asw_len);
        break;
    }
}

void Controller::getInfo(Controller *instance, uint8_t *asw_data, size_t *asw_len) {
    CmdInfoReponse resp;
    *asw_len = sizeof(CmdInfoReponse);
    strcpy(resp.Name, FIRMWARE_NAME);
    strcpy(resp.FWVer, FIRMWARE_VERSION);
    strcpy(resp.BTVer, instance->bt->Version);
    resp.Devices = instance->Devices();
    resp.Mode = !instance->IsStdMode();
    memcpy(asw_data, &resp, sizeof(CmdInfoReponse));
}

void Controller::getBTInfo(Controller *instance, uint8_t *asw_data, size_t *asw_len) {
    CmdBTInfoResponse resp;
    *asw_len = sizeof(CmdBTInfoResponse);
    strcpy(resp.Name, instance->bt->Name);
    strcpy(resp.Addr, instance->bt->Address);
    memcpy(asw_data, &resp, sizeof(CmdBTInfoResponse));
}

void Controller::setBTName(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len) {
    CmdBTSetNameRequest req;
    CmdResponse resp;

    if (req_data == nullptr || req_len < 32) {
        resp.Code = 0x01;
    } else {
        memcpy(&req, req_data, sizeof(CmdBTSetNameRequest));
        size_t len = strlen(req.Name);
        if (len > 32 || len < 1) {
            resp.Code = 0x01;
        } else {
            if (!instance->bt->ChangeMode(true)) {
                if (!instance->bt->SetName(req.Name)) {
                    instance->bt->ChangeMode(false);
                    resp.Code = 0x00;
                } else {
                    resp.Code = 0x03;
                }               
            } else {
                resp.Code = 0x02;
            }
        }
    }
    *asw_len = sizeof(CmdResponse);
    memcpy(asw_data, &resp, sizeof(CmdResponse));
}

void Controller::setBTPass(Controller * instance, const uint8_t * req_data, const size_t req_len, uint8_t * asw_data, size_t * asw_len) {
    CmdBTSetPassRequest req;
    CmdResponse resp;

    if (req_data == nullptr || req_len < 4) {
        resp.Code = 0x01;
    } else {
        memcpy(&req, req_data, sizeof(CmdBTSetPassRequest));
        size_t len = strlen(req.Pass);
        if (len > 4 || len < 1) {
            resp.Code = 0x01;
        } else {
            bool is_valid = true;
            for (uint8_t i = 0; i < len; i++) {
                if (req.Pass[i] < '0' || req.Pass[i] > '9') {
                    is_valid = false;
                    break;
                }
            }
            if (!is_valid) {
                resp.Code = 0x01;
            } else if (!instance->bt->ChangeMode(true)) {
                if (!instance->bt->SetPassword(req.Pass)) {
                    instance->bt->ChangeMode(false);
                    resp.Code = 0x00;
                } else {
                    resp.Code = 0x03;
                }               
            } else {
                resp.Code = 0x02;
            }
        }
    }
    *asw_len = sizeof(CmdResponse);
    memcpy(asw_data, &resp, sizeof(CmdResponse));
}

void Controller::getTime(Controller *instance, uint8_t *asw_data, size_t *asw_len) {
    CmdGetTimeResponse resp;

    if (!(instance->Devices() & SENS_RTC)) {
        resp.Code = 0x01;
    } else {
        resp.Code = 0x00;
        resp.Time = instance->pars.Now;
    }
    *asw_len = sizeof(CmdGetTimeResponse);
    memcpy(asw_data, &resp, sizeof(CmdGetTimeResponse));
}

void Controller::setTime(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len) {
    CmdSetTimeRequest req;
    CmdResponse resp;

    if (req_data == nullptr || req_len < sizeof(CmdSetTimeRequest)) {
        resp.Code = 0x01;
    } else {
        memcpy(&req, req_data, sizeof(CmdSetTimeRequest));
        if (!req.Time) {
            resp.Code = 0x01;
        } else {
            if (!(instance->Devices() & SENS_RTC)) {
                resp.Code = 0x02;
            } else {
                if (!instance->rtc->SetTime(req.Time)) {
                    resp.Code = 0x02;
                } else {
                    instance->pars.Now = req.Time;
                    resp.Code = 0x00;
                }
            }
        }
    }

    *asw_len = sizeof(CmdResponse);
    memcpy(asw_data, &resp, sizeof(CmdResponse));
}

void Controller::getStatus(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len) {
    CmdGetStatusRequest req;
    CmdResponse respHdr;
    AirSensorInfo *respAir = nullptr;
    SoilSensorInfo *respSoil1 = nullptr;
    SoilSensorInfo *respSoil2 = nullptr;
    RelayInfo *lampRelay = nullptr;
    PumpRelayInfo *pumpRelay = nullptr;
    RelayInfo *fanRelay = nullptr;
    RelayInfo *humRelay = nullptr;

    if (req_data == nullptr || req_len < sizeof(CmdGetStatusRequest)) {
        respHdr.Code = 0x01;
    } else {
        memcpy(&req, req_data, sizeof(CmdGetStatusRequest));
        if (req.Mask == 0x0000) {
            respHdr.Code = 0x02;
        } else {
            respHdr.Code = 0x00;
        }
    }

    if (respHdr.Code != 0x00) {
        *asw_len = sizeof(CmdResponse);
        memcpy(asw_data, &respHdr, sizeof(CmdResponse));
        return;
    }

    // Air humidity and temperature sensor
    if ((req.Mask & SENS_AIR) && (instance->device_mask & SENS_AIR)) {
        respAir = new AirSensorInfo();
        respAir->DeviceID = SENS_AIR;
        respAir->Hum = instance->pars.AirSensor.Humidity;
        respAir->Temp = instance->pars.AirSensor.Temperature;
        respAir->HumStartVent = instance->cfg.AirSensor.HumStartVent;
        respAir->HumEndVent = instance->cfg.AirSensor.HumEndVent;
        respAir->TempStartVent = instance->cfg.AirSensor.TempStartVent;
        respAir->TempEndVent = instance->cfg.AirSensor.TempEndVent;
        respAir->HumStartHum = instance->cfg.AirSensor.HumStartHum;
        respAir->HumEndHum = instance->cfg.AirSensor.HumEndHum;
    }

    // Soil moisture sensor 1
    if ((req.Mask & SENS_SOIL1) && (instance->device_mask & SENS_SOIL1)) {
        respSoil1 = new SoilSensorInfo();
        respSoil1->DeviceID = SENS_SOIL1;
        respSoil1->Hum = instance->pars.SoilSensor1;
        respSoil1->MinValue = instance->cfg.SoilSensor1.MinValue;
        respSoil1->MaxValue = instance->cfg.SoilSensor1.MaxValue;
        respSoil1->StartWatering = instance->cfg.SoilSensor1.StartWatering;
        respSoil1->EndWatering = instance->cfg.SoilSensor1.EndWatering;
    }

    // Soil moisture sensor 2
    if ((req.Mask & SENS_SOIL2) && (instance->device_mask & SENS_SOIL2)) {
        respSoil2 = new SoilSensorInfo();
        respSoil2->DeviceID = SENS_SOIL2;
        respSoil2->Hum = instance->pars.SoilSensor2;
        respSoil2->MinValue = instance->cfg.SoilSensor2.MinValue;
        respSoil2->MaxValue = instance->cfg.SoilSensor2.MaxValue;
        respSoil2->StartWatering = instance->cfg.SoilSensor2.StartWatering;
        respSoil2->EndWatering = instance->cfg.SoilSensor2.EndWatering;
    }

    // Lamp relay
    if ((req.Mask & DEV_LAMP) && (instance->device_mask & DEV_LAMP)) {
        lampRelay = new RelayInfo();
        lampRelay->DeviceID = DEV_LAMP;
        if (instance->cfg.Lamp.ActivityPeriod.StartH >= 0 && instance->cfg.Lamp.ActivityPeriod.StartH <= 23) {
            lampRelay->Activity.StartH = (uint8_t)instance->cfg.Lamp.ActivityPeriod.StartH;
        } else {
            lampRelay->Activity.StartH = 100;
        }
        if (instance->cfg.Lamp.ActivityPeriod.StartM >= 0 && instance->cfg.Lamp.ActivityPeriod.StartM <= 59) {
            lampRelay->Activity.StartM = (uint8_t)instance->cfg.Lamp.ActivityPeriod.StartM;
        } else {
            lampRelay->Activity.StartM = 100;
        }
        if (instance->cfg.Lamp.ActivityPeriod.EndH >= 0 && instance->cfg.Lamp.ActivityPeriod.EndH <= 23) {
            lampRelay->Activity.EndH = (uint8_t)instance->cfg.Lamp.ActivityPeriod.EndH;
        } else {
            lampRelay->Activity.EndH = 100; 
        }
        if (instance->cfg.Lamp.ActivityPeriod.EndM >= 0 && instance->cfg.Lamp.ActivityPeriod.EndM <= 59) {
            lampRelay->Activity.EndM = (uint8_t)instance->cfg.Lamp.ActivityPeriod.EndM;
        } else {
            lampRelay->Activity.EndM = 100;
        }
        lampRelay->IsActive = (uint8_t)instance->pars.LampActive;
        lampRelay->IsAuto = (uint8_t)instance->cfg.Lamp.IsAuto;
    }

    // Pump relay
    if ((req.Mask & DEV_PUMP) && (instance->device_mask & DEV_PUMP)) {
        pumpRelay = new PumpRelayInfo();
        pumpRelay->Common.DeviceID = DEV_PUMP;
        if (instance->cfg.Pump.ActivityPeriod.StartH >= 0 && instance->cfg.Pump.ActivityPeriod.StartH <= 23) {
            pumpRelay->Common.Activity.StartH = (uint8_t)instance->cfg.Pump.ActivityPeriod.StartH;
        } else {
            pumpRelay->Common.Activity.StartH = 100;
        }
        if (instance->cfg.Pump.ActivityPeriod.StartM >= 0 && instance->cfg.Pump.ActivityPeriod.StartM <= 59) {
            pumpRelay->Common.Activity.StartM = (uint8_t)instance->cfg.Pump.ActivityPeriod.StartM;
        } else {
            pumpRelay->Common.Activity.StartM = 100;
        }
        if (instance->cfg.Pump.ActivityPeriod.EndH >= 0 && instance->cfg.Pump.ActivityPeriod.EndH <= 23) {
            pumpRelay->Common.Activity.EndH = (uint8_t)instance->cfg.Pump.ActivityPeriod.EndH;
        } else {
            pumpRelay->Common.Activity.EndH = 100; 
        }
        if (instance->cfg.Pump.ActivityPeriod.EndM >= 0 && instance->cfg.Pump.ActivityPeriod.EndM <= 59) {
            pumpRelay->Common.Activity.EndM = (uint8_t)instance->cfg.Pump.ActivityPeriod.EndM;
        } else {
            pumpRelay->Common.Activity.EndM = 100;
        }
        pumpRelay->Common.IsActive = (uint8_t)instance->pars.PumpActive;
        pumpRelay->Common.IsAuto = (uint8_t)instance->cfg.Pump.IsAuto;
        pumpRelay->FreqWatering = instance->cfg.Pump.FreqWatering;
        pumpRelay->MaxWorkTime = instance->cfg.Pump.MaxWorkTime;
        pumpRelay->PauseBetweenStart = instance->cfg.Pump.PauseBetweenStart;
    }

    // Fan relay
    if ((req.Mask & DEV_FAN) && (instance->device_mask & DEV_FAN)) {
        fanRelay = new RelayInfo();
        fanRelay->DeviceID = DEV_FAN;
        if (instance->cfg.Fan.ActivityPeriod.StartH >= 0 && instance->cfg.Fan.ActivityPeriod.StartH <= 23) {
            fanRelay->Activity.StartH = (uint8_t)instance->cfg.Fan.ActivityPeriod.StartH;
        } else {
            fanRelay->Activity.StartH = 100;
        }
        if (instance->cfg.Fan.ActivityPeriod.StartM >= 0 && instance->cfg.Fan.ActivityPeriod.StartM <= 59) {
            fanRelay->Activity.StartM = (uint8_t)instance->cfg.Fan.ActivityPeriod.StartM;
        } else {
            fanRelay->Activity.StartM = 100;
        }
        if (instance->cfg.Fan.ActivityPeriod.EndH >= 0 && instance->cfg.Fan.ActivityPeriod.EndH <= 23) {
            fanRelay->Activity.EndH = (uint8_t)instance->cfg.Fan.ActivityPeriod.EndH;
        } else {
            fanRelay->Activity.EndH = 100; 
        }
        if (instance->cfg.Fan.ActivityPeriod.EndM >= 0 && instance->cfg.Fan.ActivityPeriod.EndM <= 59) {
            fanRelay->Activity.EndM = (uint8_t)instance->cfg.Fan.ActivityPeriod.EndM;
        } else {
            fanRelay->Activity.EndM = 100;
        }
        fanRelay->IsActive = (uint8_t)instance->pars.FanActive;
        fanRelay->IsAuto = (uint8_t)instance->cfg.Fan.IsAuto;
    }

    // Humidifier relay
    if ((req.Mask & DEV_HUMIDIFIER) && (instance->device_mask & DEV_HUMIDIFIER)) {
        humRelay = new RelayInfo();
        humRelay->DeviceID = DEV_HUMIDIFIER;
        if (instance->cfg.Humidifier.ActivityPeriod.StartH >= 0 && instance->cfg.Humidifier.ActivityPeriod.StartH <= 23) {
            humRelay->Activity.StartH = (uint8_t)instance->cfg.Humidifier.ActivityPeriod.StartH;
        } else {
            humRelay->Activity.StartH = 100;
        }
        if (instance->cfg.Humidifier.ActivityPeriod.StartM >= 0 && instance->cfg.Humidifier.ActivityPeriod.StartM <= 59) {
            humRelay->Activity.StartM = (uint8_t)instance->cfg.Humidifier.ActivityPeriod.StartM;
        } else {
            humRelay->Activity.StartM = 100;
        }
        if (instance->cfg.Humidifier.ActivityPeriod.EndH >= 0 && instance->cfg.Humidifier.ActivityPeriod.EndH <= 23) {
            humRelay->Activity.EndH = (uint8_t)instance->cfg.Humidifier.ActivityPeriod.EndH;
        } else {
            humRelay->Activity.EndH = 100; 
        }
        if (instance->cfg.Humidifier.ActivityPeriod.EndM >= 0 && instance->cfg.Humidifier.ActivityPeriod.EndM <= 59) {
            humRelay->Activity.EndM = (uint8_t)instance->cfg.Humidifier.ActivityPeriod.EndM;
        } else {
            humRelay->Activity.EndM = 100;
        }
        humRelay->IsActive = (uint8_t)instance->pars.HumidifierActive;
        humRelay->IsAuto = (uint8_t)instance->cfg.Humidifier.IsAuto;
    }

    // Create output buffer
    // Copy header
    *asw_len = sizeof(CmdResponse);
    uint8_t ofs = sizeof(CmdResponse);
    memcpy(asw_data, &respHdr, sizeof(CmdResponse));

    // Copy air sensor data
    if (respAir != nullptr) {
        memcpy(asw_data+ofs, respAir, sizeof(AirSensorInfo));
        ofs += sizeof(AirSensorInfo);
        *asw_len += sizeof(AirSensorInfo);
        delete respAir;
    }

    // Soil moisture sesnsor 1 data
    if (respSoil1 != nullptr) {
        memcpy(asw_data+ofs, respSoil1, sizeof(SoilSensorInfo));
        ofs += sizeof(SoilSensorInfo);
        *asw_len += sizeof(SoilSensorInfo);
        delete respSoil1;
    }

    // Soil moisture sesnsor 2 data
    if (respSoil2 != nullptr) {
        memcpy(asw_data+ofs, respSoil2, sizeof(SoilSensorInfo));
        ofs += sizeof(SoilSensorInfo);
        *asw_len += sizeof(SoilSensorInfo);
        delete respSoil2;
    }

    // Lamp relay data
    if (lampRelay != nullptr) {
        memcpy(asw_data+ofs, lampRelay, sizeof(RelayInfo));
        ofs += sizeof(RelayInfo);
        *asw_len += sizeof(RelayInfo);
        delete lampRelay;
    }

    // Pump relay data
    if (pumpRelay != nullptr) {
        memcpy(asw_data+ofs, pumpRelay, sizeof(PumpRelayInfo));
        ofs += sizeof(PumpRelayInfo);
        *asw_len += sizeof(PumpRelayInfo);
        delete pumpRelay;
    }

    // Fan relay data
    if (fanRelay != nullptr) {
        memcpy(asw_data+ofs, fanRelay, sizeof(RelayInfo));
        ofs += sizeof(RelayInfo);
        *asw_len += sizeof(RelayInfo);
        delete fanRelay;
    }

    // Humidifier relay data
    if (humRelay != nullptr) {
        memcpy(asw_data+ofs, humRelay, sizeof(RelayInfo));
        ofs += sizeof(RelayInfo);
        *asw_len += sizeof(RelayInfo);
        delete humRelay;
    }
}

void Controller::setStatus(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len) {
    uint8_t ofs = 0;
    CmdSetStatusRequest reqHdr;
    CmdSetStatusResponse resp;
    AirSensorInfo *respAir = nullptr;
    SoilSensorInfo *respSoil1 = nullptr;
    RelayInfo *relay = nullptr;
    PumpRelayInfo *pumpRelay = nullptr;

    if (req_data == nullptr || req_len < sizeof(CmdSetStatusRequest)) {
        resp.Code = 0x02;
        resp.FaultMask = 0xFFFF;
    } else {
        memcpy(&reqHdr, req_data, sizeof(CmdSetStatusRequest));
        ofs += sizeof(CmdSetStatusRequest);
        resp.Code = 0x00;
        resp.FaultMask = 0x0000;
    }

    if (resp.Code != 0x00) {
        *asw_len = sizeof(CmdSetStatusResponse);
        memcpy(asw_data, &resp, sizeof(CmdSetStatusResponse));
        return;
    }

    // Read devices and their settings
    uint16_t loaded = 0x0000; // loaded devices mask
    while (ofs < req_len) {
        if (ofs+sizeof(uint16_t) >= req_len) {
            resp.Code = 0x02;
            *asw_len = sizeof(CmdSetStatusResponse);
            memcpy(asw_data, &resp, sizeof(CmdSetStatusResponse));
            return;
        }

        uint16_t devId;
        memcpy(&devId, req_data+ofs, sizeof(uint16_t));
        switch (devId) {
        case SENS_AIR:
            if (ofs+sizeof(AirSensorInfo) > req_len || (loaded & devId)) {
                resp.Code = 0x02;
                resp.FaultMask |= devId;
                *asw_len = sizeof(CmdSetStatusResponse);
                memcpy(asw_data, &resp, sizeof(CmdSetStatusResponse));
                return;
            }

            respAir = new AirSensorInfo();
            memcpy(respAir, req_data+ofs, sizeof(AirSensorInfo));
            ofs += sizeof(AirSensorInfo);
            if (respAir->HumStartVent > 100 || respAir->HumEndVent > 100 || respAir->TempStartVent <= 0 || respAir->TempStartVent >= 100 || respAir->TempEndVent <= 0 || respAir->TempEndVent >= 100) {
                resp.Code = 0x02;
                resp.FaultMask |= devId;
            } else {
                instance->cfg.AirSensor.HumStartVent = respAir->HumStartVent;
                instance->cfg.AirSensor.HumEndVent = respAir->HumEndVent;
                instance->cfg.AirSensor.TempStartVent = respAir->TempStartVent;
                instance->cfg.AirSensor.TempEndVent = respAir->TempEndVent;
                instance->cfg.AirSensor.HumStartHum = respAir->HumStartHum;
                instance->cfg.AirSensor.HumEndHum = respAir->HumEndHum;
            }
            loaded |= devId;
            delete respAir;
            break;

        case SENS_SOIL1:
        case SENS_SOIL2:
            if (ofs+sizeof(SoilSensorInfo) > req_len || (loaded & devId)) {
                resp.Code = 0x02;
                resp.FaultMask |= devId;
                *asw_len = sizeof(CmdSetStatusResponse);
                memcpy(asw_data, &resp, sizeof(CmdSetStatusResponse));
                return;
            }

            respSoil1 = new SoilSensorInfo();
            memcpy(respSoil1, req_data+ofs, sizeof(SoilSensorInfo));
            ofs += sizeof(SoilSensorInfo);
            {
                SoilMoistureConfig *soil_cfg = (devId == SENS_SOIL1) ? &instance->cfg.SoilSensor1 : &instance->cfg.SoilSensor2;
                if (respSoil1->MinValue == respSoil1->MaxValue || respSoil1->EndWatering > 100 || respSoil1->StartWatering > 100 || respSoil1->StartWatering >= respSoil1->EndWatering) {
                    resp.Code = 0x02;
                    resp.FaultMask |= devId;
                } else {
                    soil_cfg->MinValue = respSoil1->MinValue;
                    soil_cfg->MaxValue = respSoil1->MaxValue;
                    soil_cfg->StartWatering = respSoil1->StartWatering;
                    soil_cfg->EndWatering = respSoil1->EndWatering;
                }
            }
            loaded |= devId;
            delete respSoil1;
            break;

        case DEV_LAMP:
        case DEV_FAN:
        case DEV_HUMIDIFIER:
            if (ofs+sizeof(RelayInfo) > req_len || (loaded & devId)) {
                resp.Code = 0x02;
                resp.FaultMask |= devId;
                *asw_len = sizeof(CmdSetStatusResponse);
                memcpy(asw_data, &resp, sizeof(CmdSetStatusResponse));
                return;
            }

            relay = new RelayInfo();
            memcpy(relay, req_data+ofs, sizeof(RelayInfo));
            ofs += sizeof(RelayInfo);
            PeriodConfig *period;
            if (devId == DEV_LAMP) {
                period = &instance->cfg.Lamp.ActivityPeriod;
                instance->cfg.Lamp.IsAuto = relay->IsAuto;
                if (!relay->IsAuto && instance->lamp != nullptr && instance->lamp->IsActivated() != relay->IsActive) {
                    if (relay->IsActive) {
                        if (!instance->lamp->Activate()) {
                            instance->pars.LampActive = true;
                        }
                    } else {
                        if (!instance->lamp->Deactivate()) {
                            instance->pars.LampActive = false;
                        }
                    }
                }
            } else if (devId == DEV_FAN) {
                period = &instance->cfg.Fan.ActivityPeriod;
                instance->cfg.Fan.IsAuto = relay->IsAuto;
                if (!relay->IsAuto && instance->fan != nullptr && instance->fan->IsActivated() != relay->IsActive) {
                    if (relay->IsActive) {
                        if (!instance->fan->Activate()) {
                            instance->pars.FanActive = true;
                        }
                    } else {
                        if (!instance->fan->Deactivate()) {
                            instance->pars.FanActive = false;
                        }
                    }
                }
            } else {
                period = &instance->cfg.Humidifier.ActivityPeriod;
                instance->cfg.Humidifier.IsAuto = relay->IsAuto;
                if (!relay->IsAuto && instance->humidifier != nullptr && instance->humidifier->IsActivated() != relay->IsActive) {
                    if (relay->IsActive) {
                        if (!instance->humidifier->Activate()) {
                            instance->pars.HumidifierActive = true;
                        }
                    } else {
                        if (!instance->humidifier->Deactivate()) {
                            instance->pars.HumidifierActive = false;
                        }
                    }
                }
            }
            if (relay->Activity.StartH > 23 || relay->Activity.EndH > 23 || relay->Activity.StartM > 59 || relay->Activity.EndM > 59) {
                period->StartH = -1;
                period->StartM = -1;
                period->EndH = -1;
                period->EndM = -1;
            } else {
                period->StartH = relay->Activity.StartH;
                period->StartM = relay->Activity.StartM;
                period->EndH = relay->Activity.EndH;
                period->EndM = relay->Activity.EndM;
            }
            loaded |= devId;
            delete relay;
            break;
        
        case DEV_PUMP:
            if (ofs+sizeof(PumpRelayInfo) > req_len || (loaded & devId)) {
                resp.Code = 0x02;
                resp.FaultMask |= devId;
                *asw_len = sizeof(CmdSetStatusResponse);
                memcpy(asw_data, &resp, sizeof(CmdSetStatusResponse));
                return;
            }

            pumpRelay = new PumpRelayInfo();
            memcpy(pumpRelay, req_data+ofs, sizeof(PumpRelayInfo));
            ofs += sizeof(PumpRelayInfo);
            if (pumpRelay->Common.Activity.StartH > 23 || pumpRelay->Common.Activity.EndH > 23 || pumpRelay->Common.Activity.StartM > 59 || pumpRelay->Common.Activity.EndM > 59) {
                instance->cfg.Pump.ActivityPeriod.StartH = -1;
                instance->cfg.Pump.ActivityPeriod.StartM = -1;
                instance->cfg.Pump.ActivityPeriod.EndH = -1;
                instance->cfg.Pump.ActivityPeriod.EndM = -1;
            } else {
                instance->cfg.Pump.ActivityPeriod.StartH = pumpRelay->Common.Activity.StartH;
                instance->cfg.Pump.ActivityPeriod.StartM = pumpRelay->Common.Activity.StartM;
                instance->cfg.Pump.ActivityPeriod.EndH = pumpRelay->Common.Activity.EndH;
                instance->cfg.Pump.ActivityPeriod.EndM = pumpRelay->Common.Activity.EndM;
            }
            instance->cfg.Pump.FreqWatering = pumpRelay->FreqWatering;
            instance->cfg.Pump.MaxWorkTime = pumpRelay->MaxWorkTime;
            instance->cfg.Pump.PauseBetweenStart = pumpRelay->PauseBetweenStart;
            instance->cfg.Pump.IsAuto = pumpRelay->Common.IsAuto;
            if (!pumpRelay->Common.IsAuto && instance->pump != nullptr && instance->pump->IsActivated() != pumpRelay->Common.IsActive) {
                if (pumpRelay->Common.IsActive) {
                    if (!instance->pump->Activate()) {
                        instance->pars.PumpActive = true;
                    }
                } else {
                    if (!instance->pump->Deactivate()) {
                        instance->pars.PumpActive = false;
                    }
                }
            }
            loaded |= devId;
            delete pumpRelay;
            break;
        } // case
    } // while

    // Save data to EEPROM
    if (reqHdr.SaveChanges) {
        if (instance->WriteConfig()) {
            resp.Code = 0x01;
        }
    }

    *asw_len = sizeof(CmdSetStatusResponse);
    memcpy(asw_data, &resp, sizeof(CmdSetStatusResponse));
}

void Controller::getMode(Controller *instance, uint8_t *asw_data, size_t *asw_len) {
    CmdMode resp;
    resp.Mode = (uint8_t)instance->IsStdMode();
    *asw_len = sizeof(CmdMode);
    memcpy(asw_data, &resp, sizeof(CmdMode));
}

void Controller::setMode(Controller *instance, const uint8_t *req_data, const size_t req_len, uint8_t *asw_data, size_t *asw_len) {
    CmdMode resp;
    resp.Mode = 0x01;
    if (req_data != nullptr && req_len == sizeof(CmdMode)) {
        memcpy(&resp, req_data, sizeof(CmdMode));
        instance->SetMode((bool)resp.Mode);
    }
    *asw_len = sizeof(CmdMode);
    memcpy(asw_data, &resp, sizeof(CmdMode));
}

uint8_t Controller::readSensors() {
    uint8_t res = 0x00;
    unsigned long now = millis();
    unsigned long lim = max(CHECK_SENSORS_FREQ, AIR_SENSOR_OPERATION_DELAY);
    if ((now - this->last_op) < lim) {
        return res;
    }
    if (this->device_mask & SENS_RTC) {
        uint32_t cur;
        uint8_t rc =this->rtc->GetTime(&cur);
        if (!rc) {
            this->pars.Now = cur;
            res |= SENS_RTC;
        }
    }
    if (this->device_mask & SENS_SOIL1) {
        uint8_t rc = this->sensor1->GetHumidity();
        if (rc <= 100) {
            this->pars.SoilSensor1 = rc;
            res |= SENS_SOIL1;
        }
    }
    if (this->device_mask & SENS_SOIL2) {
        uint8_t rc = this->sensor2->GetHumidity();
        if (rc <= 100) {
            this->pars.SoilSensor2 = rc;
            res |= SENS_SOIL2;
        }
    }
    if (this->device_mask & SENS_AIR) {
        AirData dat;
        uint8_t rc = this->air->GetData(&dat);
        if (!rc) {
            this->pars.AirSensor = dat;
            res |= SENS_AIR;
        }
    }

    this->last_op = millis();
    Serial.println(res, HEX);
    Serial.println(this->pars.Now, DEC);
    Serial.println(this->pars.AirSensor.Humidity, DEC);
    Serial.println(this->pars.AirSensor.Temperature, 2);
    Serial.println(this->pars.SoilSensor1, DEC);
    Serial.println(this->pars.SoilSensor2, DEC);
    Serial.println(this->pars.LampActive);
    Serial.println(this->pars.FanActive);
    Serial.println(this->pars.PumpActive);
    Serial.println(this->pars.HumidifierActive);
    return res;
}

inline void Controller::checkPumpRunning() {
    if (!(this->device_mask & DEV_PUMP)) {
        return;
    }
    if (this->pars.PumpActive && this->cfg.Pump.MaxWorkTime > 0) {
        unsigned long now = millis();
        if ((now - this->pump_act) >= ((unsigned long)this->cfg.Pump.MaxWorkTime * 1000UL)) {
            uint8_t rc = this->pump->Deactivate();
            if (!rc) {
                this->pars.PumpActive = false;
                this->pump_sleep = millis();
            }
        }
    }
}

void Controller::processLamp(bool is_active_rtc) {
    if (!(this->device_mask & DEV_LAMP) || !is_active_rtc || !this->cfg.Lamp.IsAuto) {
        return;
    }
    bool act = this->lamp->IsActivated();
    if (IsAlwaysActivity(this->cfg.Lamp.ActivityPeriod)) {
        if (!act) {
            this->lamp->Activate();
            this->pars.LampActive = true;
        }
        return;
    }
    if (IsTimeInRange(this->pars.Now, this->cfg.Lamp.ActivityPeriod.StartH, this->cfg.Lamp.ActivityPeriod.StartM, 0, this->cfg.Lamp.ActivityPeriod.EndH, this->cfg.Lamp.ActivityPeriod.EndM, 0)) {
        if (!act) {
            this->lamp->Activate();
            this->pars.LampActive = true;
        }
    } else {
        if (act) {
            this->lamp->Deactivate();
            this->pars.LampActive = false;
        }
    }
}

void Controller::processFan(bool is_active_rtc, bool is_active_air) {
    if (!(this->device_mask & DEV_FAN) || !this->cfg.Fan.IsAuto) {
        return;
    }

    bool act = this->fan->IsActivated();
    bool time_flg = false;
    bool air_flg = true;
    if (IsAlwaysActivity(this->cfg.Fan.ActivityPeriod) || !is_active_rtc) {
        time_flg = true;
    } else {
        time_flg = IsTimeInRange(this->pars.Now, this->cfg.Fan.ActivityPeriod.StartH, this->cfg.Fan.ActivityPeriod.StartM, 0, this->cfg.Fan.ActivityPeriod.EndH, this->cfg.Fan.ActivityPeriod.EndM, 0);
    }

    if (!is_active_air) {
        air_flg = true;
    } else {
        if (this->pars.AirSensor.Humidity >= this->cfg.AirSensor.HumStartVent || this->pars.AirSensor.Temperature >= this->cfg.AirSensor.TempStartVent) {
            air_flg = true;
        } else if (this->pars.AirSensor.Humidity <= this->cfg.AirSensor.HumEndVent && this->pars.AirSensor.Temperature <= this->cfg.AirSensor.TempEndVent) {
            air_flg = false;
        }
    }

    if (air_flg && time_flg && !act) {
        this->fan->Activate();
        this->pars.FanActive = true;
    } else if ((!air_flg || !time_flg) && act) {
        this->fan->Deactivate();
        this->pars.FanActive = false;
    }
}

void Controller::processPump(bool is_active_rtc, bool is_active_soil1, bool is_active_soil2) {
    if (!(this->device_mask & DEV_PUMP) || (!is_active_soil1 && !is_active_soil2) || !this->cfg.Pump.IsAuto) {
        return;
    }

    bool act = this->pump->IsActivated();
    bool time_flg = false;
    bool soil1_flg = false;
    bool soil2_flg = false;

    if (IsAlwaysActivity(this->cfg.Pump.ActivityPeriod) || !is_active_rtc) {
        time_flg = true;
    } else {
        time_flg = IsTimeInRange(this->pars.Now, this->cfg.Pump.ActivityPeriod.StartH, this->cfg.Pump.ActivityPeriod.StartM, 0, this->cfg.Pump.ActivityPeriod.EndH, this->cfg.Pump.ActivityPeriod.EndM, 0);
    }

    if (is_active_soil1) {
        if (this->pars.SoilSensor1 <= this->cfg.SoilSensor1.StartWatering) {
            soil1_flg = true;
        } else if (this->pars.SoilSensor1 >= this->cfg.SoilSensor1.EndWatering) {
            soil1_flg = false;
        } else {
            soil1_flg = act;
        }
    } else {
        soil1_flg = true;
    }

    if (is_active_soil2) {
        if (this->pars.SoilSensor2 <= this->cfg.SoilSensor2.StartWatering) {
            soil2_flg = true;
        } else if (this->pars.SoilSensor2 >= this->cfg.SoilSensor2.EndWatering) {
            soil2_flg = false;
        } else {
            soil2_flg = act;
        }
    } else {
        soil2_flg = true;
    }

    if (time_flg && soil1_flg && soil2_flg && !act) {
        unsigned long now = millis();
        unsigned long min_interval = (unsigned long)this->cfg.Pump.PauseBetweenStart * 1000UL;
        unsigned long freq_interval = (unsigned long)this->cfg.Pump.FreqWatering * 3600UL * 1000UL;
        if (freq_interval > min_interval) {
            min_interval = freq_interval;
        }
        if (this->pump_act == 0 || (now - this->pump_act) >= min_interval) {
            this->pump->Activate();
            this->pars.PumpActive = true;
            this->pump_act = now;
        }
    } else if ((!soil1_flg || !soil2_flg || !time_flg) && act) {
        this->pump->Deactivate();
        this->pars.PumpActive = false;
        this->pump_sleep = millis();
    }
}

void Controller::processHumidifier(bool is_active_rtc, bool is_active_air) {
    if (!(this->device_mask & DEV_HUMIDIFIER) || !this->cfg.Humidifier.IsAuto) {
        return;
    }

    bool act = this->humidifier->IsActivated();
    bool time_flg = false;
    bool air_flg = true;
    if (IsAlwaysActivity(this->cfg.Humidifier.ActivityPeriod) || !is_active_rtc) {
        time_flg = true;
    } else {
        time_flg = IsTimeInRange(this->pars.Now, this->cfg.Humidifier.ActivityPeriod.StartH, this->cfg.Humidifier.ActivityPeriod.StartM, 0, this->cfg.Humidifier.ActivityPeriod.EndH, this->cfg.Humidifier.ActivityPeriod.EndM, 0);
    }

    if (!is_active_air) {
        air_flg = true;
    } else {
        if (this->pars.AirSensor.Humidity <= this->cfg.AirSensor.HumStartHum) {
            air_flg = true;
        } else if (this->pars.AirSensor.Humidity >= this->cfg.AirSensor.HumEndHum) {
            air_flg = false;
        }
    }

    if (air_flg && time_flg && !act) {
        this->humidifier->Activate();
        this->pars.HumidifierActive = true;
    } else if ((!air_flg || !time_flg) && act) {
        this->humidifier->Deactivate();
        this->pars.HumidifierActive = false;
    }
}

void Controller::checkBTButton() {
    if (this->bt->IsActivated()) {
        return;
    }

    bool st = true;
    if (this->device_mask & DEV_BT_BUTTON) {
        st = this->btn->IsPressed();
    }

    if (st) {
        this->bt->Activate(true);
    }
}

Controller::Controller() {
    Serial.println("S");
    this->device_mask = 0x0000;
    this->pump_act = 0;
    this->pump_sleep = millis();
    this->rtc = new RTC();
    if (this->rtc->IsInited()) {
        this->device_mask |= SENS_RTC;
    }
    this->storage = new Storage();
    if (this->storage->IsInited()) {
        this->device_mask |= SENS_STORAGE;
    }

#ifdef AIR_SENSOR_PIN 
    this->air = new Air(AIR_SENSOR_PIN);
    if (this->air->IsInited()) {
        this->device_mask |= SENS_AIR;
    }
#else
    this->air = nullptr;
#endif

#ifdef SOIL_MOISTURE_PIN1
    this->sensor1 = new Soil(SOIL_MOISTURE_PIN1);
    if (this->sensor1->IsInited()) {
        this->device_mask |= SENS_SOIL1;
    }
#else
    this->sensor1 = nullptr;
#endif

#ifdef SOIL_MOISTURE_PIN2
    this->sensor2 = new Soil(SOIL_MOISTURE_PIN2);
    if (this->sensor2->IsInited()) {
        this->device_mask |= SENS_SOIL2;
    }
#else
    this->sensor2 = nullptr;
#endif

#ifdef LAMP_RELAY_PIN
    this->lamp = new Relay(LAMP_RELAY_PIN, LAMP_RELAY_TYPE);
    this->device_mask |= DEV_LAMP;
#else
    this->lamp = nullptr;
#endif

#ifdef FAN_RELAY_PIN
    this->fan = new Relay(FAN_RELAY_PIN, FAN_RELAY_TYPE);
    this->device_mask |= DEV_FAN;
#else
    this->fan = nullptr;
#endif

#ifdef PUMP_RELAY_PIN
    this->pump = new Relay(PUMP_RELAY_PIN, PUMP_RELAY_TYPE);
    this->device_mask |= DEV_PUMP;
#else
    this->pump = nullptr;
#endif

#ifdef HUMIDIFIER_RELAY_PIN
    this->humidifier = new Relay(HUMIDIFIER_RELAY_PIN, HUMIDIFIER_RELAY_TYPE);
    this->device_mask |= DEV_HUMIDIFIER;
#else
    this->humidifier = nullptr;
#endif

#ifdef BT_BUTTON_PIN
    this->btn = new Button(BT_BUTTON_PIN, true);
    this->device_mask |= DEV_BT_BUTTON;
#else
    this->btn = nullptr;
#endif

    this->bt = new Bluetooth();
    this->bt->IdleOfflineDelay = this->cfg.Bluetooth.OnlineDelay;
    this->bt->Context = this;
    this->bt->OnReceiveData = Controller::onReceive;

    this->ReadConfig();
    this->bt->Activate(false);
    this->bt->SetName(this->cfg.Bluetooth.Name);
    char buf[10] = {0x00};
    itoa(this->cfg.Bluetooth.Password, buf, 10);
    this->bt->SetPassword(buf);
    this->bt->ChangeMode(false);
    delay(500);
    this->last_op = 0;
    Serial.println("E");
}

Controller::~Controller() {
    if (this->rtc != nullptr) {
        delete this->rtc;
    }
    if (this->storage != nullptr) {
        delete this->storage;
    }
    if (this->air != nullptr) {
        delete this->air;
    }
    if (this->sensor1 != nullptr) {
        delete this->sensor1;
    }
    if (this->sensor2 != nullptr) {
        delete this->sensor2;
    }
    if (this->lamp != nullptr) {
        delete this->lamp;
    }
    if (this->fan != nullptr) {
        delete this->fan;
    }
    if (this->pump != nullptr) {
        delete this->pump;
    }
    if (this->humidifier != nullptr) {
        delete this->humidifier;
    }
    if (this->btn != nullptr) {
        delete this->btn;
    }
    delete this->bt;
}

bool Controller::IsStdMode() {
    return this->isStdMode;
}

uint16_t Controller::Devices() {
    return this->device_mask;
}

void Controller::ResetConfig()
{
    this->cfg = DEFAULT_CONFIGURATION;
}

uint8_t Controller::ReadConfig() {
    if (!(this->device_mask & SENS_STORAGE)) {
        this->ResetConfig();
        return 5;
    }
    uint8_t rc = this->storage->Read(this->cfg);
    if (rc != 0) {
        this->ResetConfig();
        return rc;
    }
    return 0;
}

uint8_t Controller::WriteConfig() {
    if (!(this->device_mask & SENS_STORAGE)) {
        return 5;
    }
    return this->storage->Write(this->cfg);
}

uint8_t Controller::SetTime(uint32_t datetime) {
    if (!(this->device_mask & SENS_RTC)) {
        return 1;
    }
    uint8_t rc = this->rtc->SetTime(datetime);
    if (rc == 0) {
        this->pars.Now = datetime;
    }
    return rc;
}

uint8_t Controller::GetTime(uint32_t *datetime) {
    if (!(this->device_mask & SENS_RTC)) {
        return 1;
    }
    uint8_t rc = this->rtc->GetTime(datetime);
    if (rc == 0 && datetime != nullptr) {
        this->pars.Now = *datetime;
    }
    return rc;
}

void Controller::ActivateDevices(uint16_t dev_mask) {
    if (this->device_mask & DEV_LAMP) {
        bool flg = false;
        uint8_t rc;
        if (dev_mask & DEV_LAMP) {
            flg = true;
        }
        if (flg != this->lamp->IsActivated()) {
            if (flg) {
                rc = this->lamp->Activate();
                if (!rc) {
                    this->pars.LampActive = true;
                }
            } else {
                rc = this->lamp->Deactivate();
                if (!rc) {
                    this->pars.LampActive = false;
                }
            }
        }
    }

    if (this->device_mask & DEV_FAN) {
        bool flg = false;
        uint8_t rc;
        if (dev_mask & DEV_FAN) {
            flg = true;
        }
        if (flg != this->fan->IsActivated()) {
            if (flg) {
                rc = this->fan->Activate();
                if (!rc) {
                    this->pars.FanActive = true;
                }
            } else {
                rc = this->fan->Deactivate();
                if (!rc) {
                    this->pars.FanActive = false;
                }
            }
        }
    }

    if (this->device_mask & DEV_PUMP) {
        bool flg = false;
        uint8_t rc;
        if (dev_mask & DEV_PUMP) {
            flg = true;
        }
        if (flg != this->pump->IsActivated()) {
            if (flg) {
                unsigned long now = millis();
                if ((now - this->pump_sleep) >= (this->cfg.Pump.PauseBetweenStart*1000)) {
                    rc = this->pump->Activate();
                    if (!rc) {
                        this->pars.PumpActive = true;
                        this->pump_act = millis();
                    }
                }
            } else {
                rc = this->pump->Deactivate();
                if (!rc) {
                    this->pars.PumpActive = false;
                    this->pump_sleep = millis();
                }
            }
        }
    }

    if (this->device_mask & DEV_HUMIDIFIER) {
        bool flg = false;
        uint8_t rc;
        if (dev_mask & DEV_HUMIDIFIER) {
            flg = true;
        }
        if (flg != this->humidifier->IsActivated()) {
            if (flg) {
                rc = this->humidifier->Activate();
                if (!rc) {
                    this->pars.HumidifierActive = true;
                }
            } else {
                rc = this->humidifier->Deactivate();
                if (!rc) {
                    this->pars.HumidifierActive = false;
                }
            }
        }
    }
}

uint16_t Controller::GetActiveDevices() {
    uint16_t res = 0;
    if (this->device_mask & DEV_LAMP) {
        if (this->lamp->IsActivated()) {
            res |= DEV_LAMP;
        }
    }
    if (this->device_mask & DEV_FAN) {
        if (this->fan->IsActivated()) {
            res |= DEV_FAN;
        }
    }
    if (this->device_mask & DEV_PUMP) {
        if (this->pump->IsActivated()) {
            res |= DEV_PUMP;
        }
    }
    if (this->device_mask & DEV_HUMIDIFIER) {
        if (this->humidifier->IsActivated()) {
            res |= DEV_HUMIDIFIER;
        }
    }
    return res;
}

void Controller::SetMode(bool isStdandart) {
    if (!isStdandart) {
        this->ActivateDevices(0x0000);
    }
    this->isStdMode = isStdandart;
}

void Controller::Process() {
    this->checkPumpRunning();
    uint8_t sens = this->readSensors();
    if (sens == 0) {
        return;
    }
    uint8_t devs = 0;
    if ((sens & SENS_RTC) && (this->device_mask & SENS_RTC)) {
        devs |= SENS_RTC;
    }
    if ((sens & SENS_AIR) && (this->device_mask & SENS_AIR)) {
        devs |= SENS_AIR;
    }
    if ((sens & SENS_SOIL1) && (this->device_mask & SENS_SOIL1)) {
        devs |= SENS_SOIL1;
    }
    if ((sens & SENS_SOIL2) && (this->device_mask & SENS_SOIL2)) {
        devs |= SENS_SOIL2;
    }

    if (this->isStdMode) {
        this->processLamp(devs & SENS_RTC);
        this->processFan(devs & SENS_RTC, devs & SENS_AIR);
        this->processPump(devs & SENS_RTC, devs & SENS_SOIL1, devs & SENS_SOIL2);
        this->processHumidifier(devs & SENS_RTC, devs & SENS_AIR);
    }

    this->checkBTButton();
    this->bt->ProcessCommands();
}

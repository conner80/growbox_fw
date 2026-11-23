/*!
* @file bluetooth.cpp
* @brief Implementation for bluetooth transport class for chip HC-05 
* @authors John Doe
* @date 2026-11-16
* @version 1.0.0
*/
#include <string.h>
#include "bluetooth.h"
#include "common/commands.h"

/*!
* @brief OK response from device by AT-command
*/
const char OK_RESP[] = "OK\r\n";

/*!
* @brief Bluetooth module change device status (see #EnumStatus) delay (in milliseconds)
*/
const unsigned long BT_CHANGE_STATUS_DELAY = 1000;
/*!
* @brief Bluetooth AT-commands processing delay (in milliseconds)
*/
const unsigned long BT_AT_PROCESSING_DELAY = 400; 
/*!
* @brief Bluetooth initialization delay (in milliseconds)
*/
const unsigned long BT_INIT_DELAY = 500;

uint8_t Bluetooth::sendATCommand(const char *cmd, char *asw, size_t len) {
    if (this->port == nullptr || this->Status() != EnumStatus::Config) {
        return 1;
    }
    if (cmd == nullptr || asw == nullptr || cmd == asw || len == 0) {
        return 2;
    }
    if (strstr(cmd, "AT") != cmd) {
        return 2;
    }
    this->port->println(cmd);
    delay(BT_AT_PROCESSING_DELAY);
    size_t idx = 0;
    memset(asw, 0x00, len);
    while (this->port->available()) {
        char c = this->port->read();
        if (idx < (len - 1)) {
            asw[idx] = c;
            idx++;
        }
    }
    asw[idx] = '\0';
    return 0;
}

void Bluetooth::onReceive(const uint8_t cmd) {
    if (this->OnReceiveData == nullptr) {
        return;
    }

    uint8_t asw_buf[255] = {0x00};
    size_t asw_len = 0;
    this->OnReceiveData(this->Context, cmd, this->req_buf, this->req_len, asw_buf, &asw_len);
    if (asw_len > 0 && asw_len <= 255) {
        memcpy(this->req_buf, asw_buf, asw_len);
        this->req_len = asw_len;
        this->SendAnswer(cmd, asw_buf, asw_len);
    } else {
        memset(this->req_buf, 0x00, sizeof(this->req_buf));
        this->req_len = 0;
        this->SendAnswer(cmd, nullptr, 0);
    }
}

void Bluetooth::sendACK() {
    uint8_t packet = ACK_PACKET;
    this->port->write((uint8_t*)&packet, sizeof(packet));
}

void Bluetooth::sendNACK() {
    uint8_t packet = NACK_PACKET;
    this->port->write((uint8_t*)&packet, sizeof(packet));
}

void Bluetooth::reqBTParams() {
    if (!this->Activate(false)) {
        return;
    }
    memset(this->Name, 0x00, sizeof(this->Name));
    memset(this->Address, 0x00, sizeof(this->Address));
    memset(this->Version, 0x00, sizeof(this->Version));
    this->Password = 0;
    char buf[35] = {0x00};
    if (this->GetAddr(buf)) {
        strcpy(this->Address, buf);
    }
    memset(buf, 0x00, sizeof(buf));
    if (this->GetName(buf)) {
        strcpy(this->Name, buf);
    }
    memset(buf, 0x00, sizeof(buf));
    if (this->GetVersion(buf)) {
        strcpy(this->Version, buf);
    }
    memset(buf, 0x00, sizeof(buf));
    if (this->GetPassword(buf)) {
        this->Password = (uint16_t)strtoul(buf, NULL, 10);
    }
}

Bluetooth::Bluetooth()
{
    this->port = new SoftwareSerial(BT_TX_PIN, BT_RX_PIN);
    this->pwr = new Relay(BT_RELAY_PIN, BT_RELAY_TYPE);
    pinMode(BT_MODE_PIN, OUTPUT); // device status/mode
    pinMode(BT_STATE_PIN, INPUT); // connection status

    digitalWrite(BT_MODE_PIN, LOW);
    this->pwr->Deactivate();
    delay(BT_INIT_DELAY);
    this->isInited = true;
    this->reqBTParams();
    this->pwr->Deactivate();
    if (this->Password == 0) {
        this->isInited = false;
    } else {
        this->isInited = true;
    }
}

Bluetooth::~Bluetooth() {
    if (this->port != nullptr) {
        SoftwareSerial* sPort = static_cast<SoftwareSerial*>(this->port);
        sPort->~SoftwareSerial();
        free(sPort);
    }
    if (this->pwr != nullptr) {
        this->pwr->Deactivate();
        delete this->pwr;
    }
}

bool Bluetooth::IsActivated() {
    if (this->pwr == nullptr) {
        return false;
    }
    return this->pwr->IsActivated();
}

bool Bluetooth::IsInited() {
    return this->isInited;
}

bool Bluetooth::IsConnected() {
    return ((digitalRead(BT_STATE_PIN) == HIGH) && this->IsActivated());
}

EnumStatus Bluetooth::Status() {
    return this->status;
}

uint8_t Bluetooth::Activate(bool isTransport) {
    if (this->pwr == nullptr || this->port == nullptr) {
        return 2;
    }
    if (this->pwr->IsActivated()) {
        return 0;
    }

    if (isTransport) {
        digitalWrite(BT_MODE_PIN, LOW);
    } else {
        digitalWrite(BT_MODE_PIN, HIGH);
    }
    delay(100);
    uint8_t rc = this->pwr->Activate();
    if (rc != 0) {
        return rc;
    }
    delay(BT_CHANGE_STATUS_DELAY);
    if (isTransport) {
        this->status = EnumStatus::Transport;
        this->port->begin(9600);
    } else {
        this->status = EnumStatus::Config;
        this->port->begin(38400);
    }
    delay(100);
    this->last_op = millis();
    return 0;
}

uint8_t Bluetooth::Deactivate() {
    if (this->pwr == nullptr || this->port == nullptr) {
        return 2;
    }
    if (!this->pwr->IsActivated()) {
        return 0;
    }
    uint8_t rc = this->pwr->Deactivate();
    if (rc != 0) {
        return rc;
    }
    this->port->end();
    this->status = EnumStatus::Disabled;
    return 0;
}

uint8_t Bluetooth::ChangeMode(bool isConfig) {
    if (this->port == nullptr || !this->IsActivated()) {
        return 1;
    }
    this->port->end();
    if (isConfig) {
        digitalWrite(BT_MODE_PIN, HIGH);
        this->port->begin(38400);
        this->status = EnumStatus::Config;
    } else {
        digitalWrite(BT_MODE_PIN, LOW);
        this->port->begin(9600);
        this->status = EnumStatus::Transport;
    }
    delay(100);
    return 0;
}

uint8_t Bluetooth::SendATPing() {
    char data[50]; // answer data buffer
    uint8_t rc = this->sendATCommand("AT", data, sizeof(data));
    if (rc != 0) {
        return rc;
    }
    if (data != strstr(data, OK_RESP)) {
        return 2;
    }
    return 0;
}

uint8_t Bluetooth::GetAddr(char *addr) {
    char data[50]; // answer data buffer
    uint8_t rc = this->sendATCommand("AT+ADDR?", data, sizeof(data));
    if (rc != 0) {
        return rc;
    }
    if (strstr(data, OK_RESP) == NULL) {
        return 2;
    }
    if (sscanf(data, "+ADDR:%[^\\r]", addr) == 1) {
        return 0;
    }
    return 2;
}

uint8_t Bluetooth::GetVersion(char *ver) {
    char data[50]; // answer data buffer
    uint8_t rc = this->sendATCommand("AT+VERSION?", data, sizeof(data));
    if (rc != 0) {
        return rc;
    }
    if (strstr(data, OK_RESP) == NULL) {
        return 2;
    }
    if (sscanf(data, "+VERSION:%[^\\r]", ver) == 1) {
        return 0;
    }
    return 2;
}

uint8_t Bluetooth::ResetToDefault() {
    char data[50]; // answer data buffer
    uint8_t rc = this->sendATCommand("AT+ORGL", data, sizeof(data));
    if (rc != 0) {
        return rc;
    }
    if (strstr(data, OK_RESP) != data) {
        return 2;
    }
    return 0;
}

uint8_t Bluetooth::GetName(char *name) {
    char data[50]; // answer data buffer
    uint8_t rc = this->sendATCommand("AT+NAME?", data, sizeof(data));
    if (rc != 0) {
        return rc;
    }
    if (strstr(data, OK_RESP) == NULL) {
        return 2;
    }
    if (sscanf(data, "+NAME:%[^\\r]", name) == 1) {
        return 0;
    }
    return 2;
}

uint8_t Bluetooth::SetName(const char *name) {
    char buf[50] = {0x00};
    char data[50]; // answer data buffer
    snprintf(buf, sizeof(buf), "AT+NAME=%s", name);
    uint8_t rc = this->sendATCommand(buf, data, sizeof(data));
    if (rc != 0) {
        return rc;
    }
    if (strstr(data, OK_RESP) != data) {
        return 2;
    }
    strcpy(this->Name, name);
    return 0;
}

uint8_t Bluetooth::GetPassword(char *pass) {
    char data[50]; // answer data buffer
    uint8_t rc = this->sendATCommand("AT+PSWD?", data, sizeof(data));
    if (rc != 0) {
        return rc;
    }
    if (strstr(data, OK_RESP) == NULL) {
        return 2;
    }
    if (sscanf(data, "+PSWD:%[^\\r]", pass) == 1) {
        return 0;
    }
    return 2;
}

uint8_t Bluetooth::SetPassword(const char *pass) {
    char buf[50] = {0x00};
    char data[50]; // answer data buffer
    snprintf(buf, sizeof(buf), "AT+PSWD=%s", pass);
    uint8_t rc = this->sendATCommand(buf, data, sizeof(data));
    if (rc != 0) {
        return rc;
    }
    if (strstr(data, OK_RESP) != data) {
        return 2;
    }
    this->Password = (uint16_t)strtoul(pass, NULL, 10);
    return 0;
}

uint8_t Bluetooth::SendAnswer(const uint8_t cmd, const uint8_t *asw, const size_t asw_len) {
    if (cmd < CMD_INFO || cmd > CMD_SET_MODE) {
        return 1;
    }
    if (asw_len > 255) {
        return 2;
    }
    switch (cmd) {
    case CMD_INFO:
        if (asw_len != sizeof(CmdInfoReponse)) {
            return 2;
        }
        break;
    case CMD_GET_BT_INFO:
        if (asw_len != sizeof(CmdBTInfoResponse)) {
            return 2;
        }
        break;
    case CMD_SET_BT_NAME:
    case CMD_SET_BT_PASS:
    case CMD_SET_TIME:
        if (asw_len != sizeof(CmdResponse)) {
            return 2;
        }
        break;
    case CMD_GET_TIME:
        if (asw_len != sizeof(CmdGetTimeResponse)) {
            return 2;
        }
        break;
    case CMD_GET_STATUS:
        if (asw_len < sizeof(CmdResponse)) {
            return 2;
        }
        break;
    case CMD_SET_STATUS:
        if (asw_len < sizeof(CmdResponse) || asw_len > sizeof(CmdSetStatusResponse)){
            return 2;
        }
        break;
    default:
        if (asw_len != sizeof(CmdMode)) {
            return 2;
        }
        break;
    } // case
    if (this->status != EnumStatus::Transport) {
        return 3;
    }
    if (this->port == nullptr || !this->isInited || !this->IsActivated() || !this->IsConnected()) {
        return 4;
    }
    PacketHeader hdr;
    hdr.Sign = RESPONSE_PACKET;
    hdr.Cmd = cmd;
    hdr.Len = asw_len;
    this->port->write((uint8_t*)&hdr, sizeof(hdr));
    if (asw != nullptr && asw_len != 0) {
        this->port->write(asw, asw_len);
    }
    this->stage = EnumReceiveStage::ACK_DATA;
    return 0;
}

void Bluetooth::ProcessCommands() {
    if (!this->isInited || this->port == nullptr || !this->IsActivated()) {
        return;
    }
    if (this->IsConnected()) {
        this->last_op = millis();
    } else {
        if (this->IdleOfflineDelay != 0 && ((millis() - this->last_op) >= (this->IdleOfflineDelay*1000))) {
            this->Deactivate();
        }
        return;
    }
    while (this->port->available()) {
        uint8_t b = this->port->read();

        switch (this->stage) {
        case EnumReceiveStage::ACK_DATA:
            if (b == ACK_PACKET) {
                this->stage = EnumReceiveStage::WAIT_BEGIN;
            } else if (b == NACK_PACKET) {
                if (this->SendAnswer(this->cmd, this->req_buf, this->req_len)) {
                    this->stage = EnumReceiveStage::WAIT_BEGIN;
                }
            } else if (b == REQUEST_PACKET) {
                this->cmd = 0x00;
                this->req_len = 0;
                memset(this->req_buf, 0x00, sizeof(this->req_buf));
                this->stage = EnumReceiveStage::WAIT_COMMAND;
            }
            break;
        case EnumReceiveStage::WAIT_BEGIN:
            if (b == REQUEST_PACKET) {
                this->cmd = 0x00;
                this->req_len = 0;
                memset(this->req_buf, 0x00, sizeof(this->req_buf));
                this->stage = EnumReceiveStage::WAIT_COMMAND;
            }
            break;
        case EnumReceiveStage::WAIT_COMMAND:
            if (b < CMD_INFO || b > CMD_SET_MODE) {
                this->sendNACK();
                this->stage = EnumReceiveStage::WAIT_BEGIN;
            } else {
                this->cmd = b;
                this->stage = EnumReceiveStage::WAIT_LENGTH;
            }
            break;
        case EnumReceiveStage::WAIT_LENGTH:
            switch (this->cmd) {
            case CMD_INFO:
            case CMD_GET_BT_INFO:
            case CMD_GET_TIME:
            case CMD_GET_MODE:
                if (b != 0) {
                    this->sendNACK();
                    this->stage = EnumReceiveStage::WAIT_BEGIN;
                }
                break;
            case CMD_SET_BT_NAME:
                if (b != 32 && b != 33) {
                    this->sendNACK();
                    this->stage = EnumReceiveStage::WAIT_BEGIN;
                }
                break;
            case CMD_SET_BT_PASS:
                if (b != 4 && b != 5) {
                    this->sendNACK();
                    this->stage = EnumReceiveStage::WAIT_BEGIN;
                }
                break;
            case CMD_SET_TIME:
                if (b != 4) {
                    this->sendNACK();
                    this->stage = EnumReceiveStage::WAIT_BEGIN;
                }
                break;
            case CMD_GET_STATUS:
                if (b != 2) {
                    this->sendNACK();
                    this->stage = EnumReceiveStage::WAIT_BEGIN;
                }
                break;
            case CMD_SET_STATUS:
                if (b < 3) {
                    this->sendNACK();
                    this->stage = EnumReceiveStage::WAIT_BEGIN;
                }
                break;
            default:
                if (b != 1) {
                    this->sendNACK();
                    this->stage = EnumReceiveStage::WAIT_BEGIN;
                }
                break;
            } // case

            this->req_len = b;
            this->rcv_len = 0;
            if (this->cmd == CMD_INFO || this->cmd == CMD_GET_BT_INFO || this->cmd == CMD_GET_TIME || this->cmd == CMD_GET_MODE) {
                this->sendACK();
                this->stage = EnumReceiveStage::ACK_DATA;
                this->onReceive(this->cmd);
            }

            break;
        case EnumReceiveStage::READ_DATA:
            this->req_buf[this->rcv_len] = b;
            this->rcv_len++;
            if (this->rcv_len >= this->req_len) {
                this->sendACK();
                this->stage = EnumReceiveStage::ACK_DATA;
                this->onReceive(this->cmd);
            }
            break;
        } // case
    } // while
}

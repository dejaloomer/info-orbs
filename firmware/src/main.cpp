#include "GlobalResources.h"
#include "MainHelper.h"
#include "clockwidget/ClockWidget.h"
#include "mqttwidget/MQTTWidget.h"
#include "parqetwidget/ParqetWidget.h"
#include "stockwidget/StockWidget.h"
#include "weatherwidget/WeatherWidget.h"
#include "webdatawidget/WebDataWidget.h"
#include "wifiwidget/WifiWidget.h"
#include <ArduinoLog.h>
#include "esphome.h"

using namespace esphome;
using std::isnan;
using std::min;
using std::max;
logger::Logger *logger_logger_id;
//web_server_base::WebServerBase *web_server_base_webserverbase_id;
//captive_portal::CaptivePortal *captive_portal_captiveportal_id;
//PWC wifi::WiFiComponent *wifi_wificomponent_id;
//PWC mdns::MDNSComponent *mdns_mdnscomponent_id;
//esphome::ESPHomeOTAComponent *esphome_esphomeotacomponent_id;
//safe_mode::SafeModeComponent *safe_mode_safemodecomponent_id;
api::APIServer *api_apiserver_id;
using namespace api;
//preferences::IntervalSyncer *preferences_intervalsyncer_id;
bluetooth_proxy::BluetoothProxy *bluetooth_proxy_bluetoothproxy_id;
esp32_ble_tracker::ESP32BLETracker *esp32_ble_tracker_esp32bletracker_id;
esp32_ble::ESP32BLE *esp32_ble_esp32ble_id;
bluetooth_proxy::BluetoothConnection *bluetooth_proxy_bluetoothconnection_id;
bluetooth_proxy::BluetoothConnection *bluetooth_proxy_bluetoothconnection_id_2;
bluetooth_proxy::BluetoothConnection *bluetooth_proxy_bluetoothconnection_id_3;
esp32_ble_tracker::ESPBTAdvertiseTrigger *esp32_ble_tracker_espbtadvertisetrigger_id;
Automation<const esp32_ble_tracker::ESPBTDevice &> *automation_id;
LambdaAction<const esp32_ble_tracker::ESPBTDevice &> *lambdaaction_id;

#define yield() esphome::yield()
#define millis() esphome::millis()
#define micros() esphome::micros()
#define delay(x) esphome::delay(x)
#define delayMicroseconds(x) esphome::delayMicroseconds(x)

TFT_eSPI tft = TFT_eSPI();

GlobalTime *globalTime{nullptr};
WifiWidget *wifiWidget{nullptr};
ScreenManager *sm{nullptr};
ConfigManager *config{nullptr};
OrbsWiFiManager *wifiManager{nullptr};
WidgetSet *widgetSet{nullptr};

void addWidgets() {
    // Always add clock
    widgetSet->add(new ClockWidget(*sm, *config));
#ifdef INCLUDE_WEATHER
    widgetSet->add(new WeatherWidget(*sm, *config));
#endif
#ifdef INCLUDE_STOCK
    widgetSet->add(new StockWidget(*sm, *config));
#endif
#ifdef INCLUDE_PARQET
    widgetSet->add(new ParqetWidget(*sm, *config));
#endif
#ifdef INCLUDE_WEBDATA
    #ifdef WEB_DATA_WIDGET_URL
    widgetSet->add(new WebDataWidget(*sm, *config, WEB_DATA_WIDGET_URL));
    #endif
    #ifdef WEB_DATA_STOCK_WIDGET_URL
    widgetSet->add(new WebDataWidget(*sm, *config, WEB_DATA_STOCK_WIDGET_URL));
    #endif
#endif
#ifdef INCLUDE_MQTT
    widgetSet->add(new MQTTWidget(*sm, *config));
#endif
}
void esphomeSetup(){
    App.pre_setup("higuy", "", "", "", __DATE__ ", " __TIME__, false);
  // logger:
  //   level: DEBUG
  //   id: logger_logger_id
  //   baud_rate: 115200
  //   tx_buffer_size: 512
  //   deassert_rts_dtr: false
  //   hardware_uart: UART0
  //   logs: {}
  logger_logger_id = new logger::Logger(115200, 512);
  logger_logger_id->set_uart_selection(logger::UART_SELECTION_UART0);
  logger_logger_id->pre_setup();
  logger_logger_id->set_component_source("logger");
  App.register_component(logger_logger_id);
  // web_server_base:
  //   id: web_server_base_webserverbase_id
  //PWC web_server_base_webserverbase_id = new web_server_base::WebServerBase();
  //PWC web_server_base_webserverbase_id->set_component_source("web_server_base");
  //PWC App.register_component(web_server_base_webserverbase_id);
  // captive_portal:
  //   id: captive_portal_captiveportal_id
  //   web_server_base_id: web_server_base_webserverbase_id
  //PWC captive_portal_captiveportal_id = new captive_portal::CaptivePortal(web_server_base_webserverbase_id);
  //PWC captive_portal_captiveportal_id->set_component_source("captive_portal");
  //PWC App.register_component(captive_portal_captiveportal_id);
  // wifi:
  //   ap:
  //     ssid: Pwcproxy Fallback Hotspot
  //     password: QICxmY5ZCtEB
  //     id: wifi_wifiap_id
  //     ap_timeout: 1min
  //   id: wifi_wificomponent_id
  //   domain: .local
  //   reboot_timeout: 15min
  //   power_save_mode: LIGHT
  //   fast_connect: false
  //   passive_scan: false
  //   enable_on_boot: true
  //   networks:
  //   - ssid: RFK
  //     password: goawaydude
  //     id: wifi_wifiap_id_2
  //     priority: 0.0
  //   use_address: guybrodude.local
  #if PWCMEM
  wifi_wificomponent_id = new wifi::WiFiComponent();
  wifi_wificomponent_id->set_use_address("matrix.local");
  {
  wifi::WiFiAP wifi_wifiap_id_2 = wifi::WiFiAP();
  wifi_wifiap_id_2.set_ssid("RFK");
  wifi_wifiap_id_2.set_password("goawaydude");
  wifi_wifiap_id_2.set_priority(0.0f);
  wifi_wificomponent_id->add_sta(wifi_wifiap_id_2);
  }
  #if PWC
  {
  wifi::WiFiAP wifi_wifiap_id = wifi::WiFiAP();
  wifi_wifiap_id.set_ssid("Pwcproxy Fallback Hotspot");
  wifi_wifiap_id.set_password("QICxmY5ZCtEB");
  wifi_wificomponent_id->set_ap(wifi_wifiap_id);
  }
  #endif
  wifi_wificomponent_id->set_ap_timeout(60000);
  wifi_wificomponent_id->set_reboot_timeout(900000);
  wifi_wificomponent_id->set_power_save_mode(wifi::WIFI_POWER_SAVE_LIGHT);
  wifi_wificomponent_id->set_fast_connect(false);
  wifi_wificomponent_id->set_passive_scan(false);
  wifi_wificomponent_id->set_enable_on_boot(true);
  wifi_wificomponent_id->set_component_source("wifi");
  App.register_component(wifi_wificomponent_id);
  #endif
  // mdns:
  //   id: mdns_mdnscomponent_id
  //   disabled: false
  //   services: []
  #if PWCMEM
  mdns_mdnscomponent_id = new mdns::MDNSComponent();
  mdns_mdnscomponent_id->set_component_source("mdns");
  App.register_component(mdns_mdnscomponent_id);
  #endif
  // ota:
  // ota.esphome:
  //   platform: esphome
  //   password: mysecret
  //   id: esphome_esphomeotacomponent_id
  //   version: 2
  //   port: 3232
  #if PWC
  esphome_esphomeotacomponent_id = new esphome::ESPHomeOTAComponent();
  esphome_esphomeotacomponent_id->set_port(3232);
  esphome_esphomeotacomponent_id->set_auth_password("mysecret");
  esphome_esphomeotacomponent_id->set_component_source("esphome.ota");
  App.register_component(esphome_esphomeotacomponent_id);
  #endif
  // safe_mode:
  //   id: safe_mode_safemodecomponent_id
  //   boot_is_good_after: 1min
  //   disabled: false
  //   num_attempts: 10
  //   reboot_timeout: 5min
  #if PWC
  safe_mode_safemodecomponent_id = new safe_mode::SafeModeComponent();
  safe_mode_safemodecomponent_id->set_component_source("safe_mode");
  App.register_component(safe_mode_safemodecomponent_id);
  if (safe_mode_safemodecomponent_id->should_enter_safe_mode(10, 300000, 60000)) return;
  #endif
  // api:
  //   password: mysecret
  //   id: api_apiserver_id
  //   port: 6053
  //   reboot_timeout: 15min
  #if 1
  api_apiserver_id = new api::APIServer();
  api_apiserver_id->set_component_source("api");
  App.register_component(api_apiserver_id);
  api_apiserver_id->set_port(6053);
  api_apiserver_id->set_password("mysecret");
  api_apiserver_id->set_reboot_timeout(900000);
  #endif
  // esp32:
  //   board: esp32doit-devkit-v1
  //   framework:
  //     version: 2.0.5
  //     advanced:
  //       ignore_efuse_custom_mac: false
  //     source: ~3.20005.0
  //     platform_version: platformio/espressif32@5.4.0
  //     type: arduino
  //   flash_size: 4MB
  //   variant: ESP32
  // preferences:
  //   id: preferences_intervalsyncer_id
  //   flash_write_interval: 60s
  #if PWC
  preferences_intervalsyncer_id = new preferences::IntervalSyncer();
  preferences_intervalsyncer_id->set_write_interval(60000);
  preferences_intervalsyncer_id->set_component_source("preferences");
  App.register_component(preferences_intervalsyncer_id);
  #endif
  // bluetooth_proxy:
  //   active: true
  //   id: bluetooth_proxy_bluetoothproxy_id
  //   esp32_ble_id: esp32_ble_tracker_esp32bletracker_id
  //   connections:
  //   - esp32_ble_id: esp32_ble_tracker_esp32bletracker_id
  //     id: bluetooth_proxy_bluetoothconnection_id
  //   - esp32_ble_id: esp32_ble_tracker_esp32bletracker_id
  //     id: bluetooth_proxy_bluetoothconnection_id_2
  //   - esp32_ble_id: esp32_ble_tracker_esp32bletracker_id
  //     id: bluetooth_proxy_bluetoothconnection_id_3
  bluetooth_proxy_bluetoothproxy_id = new bluetooth_proxy::BluetoothProxy();
  bluetooth_proxy_bluetoothproxy_id->set_component_source("bluetooth_proxy");
  App.register_component(bluetooth_proxy_bluetoothproxy_id);
  bluetooth_proxy_bluetoothproxy_id->set_active(true);
  // esp32_ble_tracker:
  //   on_ble_advertise:
  //   - then:
  //     - lambda: !lambda |-
  //         ESP_LOGD("ble_tracker", "Device detected: %s, RSSI: %d",
  //                  x.address_str().c_str(), x.get_rssi());
  //       type_id: lambdaaction_id
  //     automation_id: automation_id
  //     trigger_id: esp32_ble_tracker_espbtadvertisetrigger_id
  //   id: esp32_ble_tracker_esp32bletracker_id
  //   ble_id: esp32_ble_esp32ble_id
  //   scan_parameters:
  //     duration: 5min
  //     interval: 320ms
  //     window: 30ms
  //     active: true
  //     continuous: true
  esp32_ble_tracker_esp32bletracker_id = new esp32_ble_tracker::ESP32BLETracker();
  esp32_ble_tracker_esp32bletracker_id->set_component_source("esp32_ble_tracker");
  App.register_component(esp32_ble_tracker_esp32bletracker_id);
  // socket:
  //   implementation: bsd_sockets
  // md5:
  // network:
  //   enable_ipv6: false
  //   min_ipv6_addr_count: 0
  // esp32_ble:
  //   id: esp32_ble_esp32ble_id
  //   io_capability: none
  //   enable_on_boot: true
  //   advertising_cycle_time: 10s
  esp32_ble_esp32ble_id = new esp32_ble::ESP32BLE();
  esp32_ble_esp32ble_id->set_enable_on_boot(true);
  esp32_ble_esp32ble_id->set_io_capability(esp32_ble::IO_CAP_NONE);
  esp32_ble_esp32ble_id->set_advertising_cycle_time(10000);
  esp32_ble_esp32ble_id->set_component_source("esp32_ble");
  App.register_component(esp32_ble_esp32ble_id);
  esp32_ble_tracker_esp32bletracker_id->register_listener(bluetooth_proxy_bluetoothproxy_id);
  bluetooth_proxy_bluetoothconnection_id = new bluetooth_proxy::BluetoothConnection();
  bluetooth_proxy_bluetoothconnection_id->set_component_source("bluetooth_proxy");
  App.register_component(bluetooth_proxy_bluetoothconnection_id);
  bluetooth_proxy_bluetoothproxy_id->register_connection(bluetooth_proxy_bluetoothconnection_id);
  esp32_ble_tracker_esp32bletracker_id->register_client(bluetooth_proxy_bluetoothconnection_id);
  bluetooth_proxy_bluetoothconnection_id_2 = new bluetooth_proxy::BluetoothConnection();
  bluetooth_proxy_bluetoothconnection_id_2->set_component_source("bluetooth_proxy");
  App.register_component(bluetooth_proxy_bluetoothconnection_id_2);
  bluetooth_proxy_bluetoothproxy_id->register_connection(bluetooth_proxy_bluetoothconnection_id_2);
  esp32_ble_tracker_esp32bletracker_id->register_client(bluetooth_proxy_bluetoothconnection_id_2);
  bluetooth_proxy_bluetoothconnection_id_3 = new bluetooth_proxy::BluetoothConnection();
  bluetooth_proxy_bluetoothconnection_id_3->set_component_source("bluetooth_proxy");
  App.register_component(bluetooth_proxy_bluetoothconnection_id_3);
  bluetooth_proxy_bluetoothproxy_id->register_connection(bluetooth_proxy_bluetoothconnection_id_3);
  esp32_ble_tracker_esp32bletracker_id->register_client(bluetooth_proxy_bluetoothconnection_id_3);
  esp32_ble_esp32ble_id->register_gap_event_handler(esp32_ble_tracker_esp32bletracker_id);
  esp32_ble_esp32ble_id->register_gattc_event_handler(esp32_ble_tracker_esp32bletracker_id);
  esp32_ble_esp32ble_id->register_ble_status_event_handler(esp32_ble_tracker_esp32bletracker_id);
  esp32_ble_tracker_esp32bletracker_id->set_parent(esp32_ble_esp32ble_id);
  esp32_ble_tracker_esp32bletracker_id->set_scan_duration(300);
  esp32_ble_tracker_esp32bletracker_id->set_scan_interval(512);
  esp32_ble_tracker_esp32bletracker_id->set_scan_window(48);
  esp32_ble_tracker_esp32bletracker_id->set_scan_active(true);
  esp32_ble_tracker_esp32bletracker_id->set_scan_continuous(true);
 #if 1
  esp32_ble_tracker_espbtadvertisetrigger_id = new esp32_ble_tracker::ESPBTAdvertiseTrigger(esp32_ble_tracker_esp32bletracker_id);
  automation_id = new Automation<const esp32_ble_tracker::ESPBTDevice &>(esp32_ble_tracker_espbtadvertisetrigger_id);
  lambdaaction_id = new LambdaAction<const esp32_ble_tracker::ESPBTDevice &>([=](const esp32_ble_tracker::ESPBTDevice & x) -> void {
      #line 39 "esp32_s3_bluetooth_proxy.yaml"
      ESP_LOGD("ble_tracker", "Device detected: %s, RSSI: %d",
               x.address_str().c_str(), x.get_rssi());
  });
  automation_id->add_actions({lambdaaction_id});
 #endif
  App.setup();
}

void setup() {
    // Initialize global resources
    initializeGlobalResources();

#ifdef SERIAL_INTERFACE_INIT_DELAY
    // Add a delay to allow the serial interface to initialize
    delay(SERIAL_INTERFACE_INIT_DELAY);
#endif

    Serial.begin(115200);

    // Clear the serial buffer of any garbage
    while (Serial.available() > 0) {
        Serial.read();
    }

#ifdef LOG_TIMESTAMP
    Log.setPrefix(MainHelper::printPrefix);
#endif
    Log.begin(LOG_LEVEL, &Serial);
    Log.noticeln("🚀 Starting up...");
    Log.noticeln("PCB Version: %s", PCB_VERSION);

    ShowMemoryUsage::printSerial(true, true);
    wifiManager = new OrbsWiFiManager();
    config = new ConfigManager(*wifiManager);
    sm = new ScreenManager(tft);
    widgetSet = new WidgetSet(sm);

    // Pass references to MainHelper
    MainHelper::init(wifiManager, config, sm, widgetSet);
    MainHelper::setupLittleFS();
    MainHelper::setupConfig();
    MainHelper::setupButtons();
    MainHelper::showWelcome();

    pinMode(BUSY_PIN, OUTPUT);
    Log.noticeln("Connecting to WiFi");
    wifiWidget = new WifiWidget(*sm, *config, *wifiManager);
    wifiWidget->setup();

    globalTime = GlobalTime::getInstance();
    addWidgets();
    config->setupWebPortal();
    MainHelper::resetCycleTimer();
    Serial.println("Starting esphome");
        ShowMemoryUsage::printSerial(true, true);
        Serial.println("Starting netif");
//esp_netif_init();
    esphomeSetup();
    ShowMemoryUsage::printSerial(true, true);


}

void loop() {
      App.loop();
#if 1
    MainHelper::watchdogReset();
    if (wifiWidget->isConnected() == false) {
        wifiWidget->update();
        wifiWidget->draw();
        widgetSet->setClearScreensOnDrawCurrent(); // Clear screen after wifiWidget
        delay(100);
    } else {
        if (!widgetSet->initialUpdateDone()) {
            widgetSet->initializeAllWidgetsData();
            MainHelper::setupWebPortalEndpoints();
        }
        globalTime->updateTime();

        MainHelper::checkButtons();

        widgetSet->updateCurrent();
        MainHelper::updateBrightnessByTime(globalTime->getHour24());
        widgetSet->drawCurrent();

        MainHelper::checkCycleWidgets();
        wifiManager->process();
        TaskManager::getInstance()->processAwaitingTasks();
        TaskManager::getInstance()->processTaskResponses();
    }
#ifdef MEMORY_DEBUG_INTERVAL
    ShowMemoryUsage::printSerial();
#endif
    MainHelper::restartIfNecessary();
#endif
}

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <Arduino_NetworkConfigurator.h>
#include "configuratorAgents/agents/BLEAgent.h"
#include "configuratorAgents/agents/SerialAgent.h"
void onCloudBackwardChange();
void onCloudCloseCurtainsChange();
void onCloudCloseFrontCurtainsChange();
void onCloudCloseLeftCurtainsChange();
void onCloudCloseRearCurtainsChange();
void onCloudCloseRightCurtainsChange();
void onCloudFanStateChange();
void onCloudForwardChange();
void onCloudLeftChange();
void onCloudLightStateChange();
void onCloudManualCurtainsChange();
void onCloudManualDriveChange();
void onCloudManualFanChange();
void onCloudManualLightChange();
void onCloudOpenCurtainsChange();
void onCloudOpenFrontCurtainsChange();
void onCloudOpenLeftCurtainsChange();
void onCloudOpenRearCurtainsChange();
void onCloudOpenRightCurtainsChange();
void onCloudRightChange();

String cloudLocation;
String cloudMapsLink;
String cloudUptime;
float cloudAltitude;
float cloudHumidity;
float cloudPressure;
float cloudTemperature;
int cloudAncientHelmet;
int cloudAncientStatue;
int cloudAncientVase;
int cloudAverageBottom;
int cloudAverageLeft;
int cloudAverageRight;
int cloudAverageTop;
int cloudCandleHolder;
int cloudLight;
int cloudRain;
bool cloudAlertDetected;
bool cloudArtifactDetected;
bool cloudBackward;
bool cloudCloseCurtains;
bool cloudCloseFrontCurtains;
bool cloudCloseLeftCurtains;
bool cloudCloseRearCurtains;
bool cloudCloseRightCurtains;
bool cloudFanState;
bool cloudForward;
bool cloudLeft;
bool cloudLightState;
bool cloudManualCurtains;
bool cloudManualDrive;
bool cloudManualFan;
bool cloudManualLight;
bool cloudOpenCurtains;
bool cloudOpenFrontCurtains;
bool cloudOpenLeftCurtains;
bool cloudOpenRearCurtains;
bool cloudOpenRightCurtains;
bool cloudRight;

KVStore kvStore;
BLEAgentClass BLEAgent;
SerialAgentClass SerialAgent;
WiFiConnectionHandler ArduinoIoTPreferredConnection; 
NetworkConfiguratorClass NetworkConfigurator(ArduinoIoTPreferredConnection);

void initProperties(){
  NetworkConfigurator.addAgent(BLEAgent);
  NetworkConfigurator.addAgent(SerialAgent);
  NetworkConfigurator.setStorage(kvStore);
  // For changing the default reset pin uncomment and set your preferred pin. Use DISABLE_PIN for disabling the reset procedure.
  //NetworkConfigurator.setReconfigurePin(your_pin);
  ArduinoCloud.setConfigurator(NetworkConfigurator);

  ArduinoCloud.addProperty(cloudLocation, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudMapsLink, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudUptime, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudAltitude, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudHumidity, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudPressure, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudTemperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudAncientHelmet, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudAncientStatue, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudAncientVase, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudAverageBottom, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudAverageLeft, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudAverageRight, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudAverageTop, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudCandleHolder, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudLight, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudRain, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudAlertDetected, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudArtifactDetected, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cloudBackward, READWRITE, ON_CHANGE, onCloudBackwardChange);
  ArduinoCloud.addProperty(cloudCloseCurtains, READWRITE, ON_CHANGE, onCloudCloseCurtainsChange);
  ArduinoCloud.addProperty(cloudCloseFrontCurtains, READWRITE, ON_CHANGE, onCloudCloseFrontCurtainsChange);
  ArduinoCloud.addProperty(cloudCloseLeftCurtains, READWRITE, ON_CHANGE, onCloudCloseLeftCurtainsChange);
  ArduinoCloud.addProperty(cloudCloseRearCurtains, READWRITE, ON_CHANGE, onCloudCloseRearCurtainsChange);
  ArduinoCloud.addProperty(cloudCloseRightCurtains, READWRITE, ON_CHANGE, onCloudCloseRightCurtainsChange);
  ArduinoCloud.addProperty(cloudFanState, READWRITE, ON_CHANGE, onCloudFanStateChange);
  ArduinoCloud.addProperty(cloudForward, READWRITE, ON_CHANGE, onCloudForwardChange);
  ArduinoCloud.addProperty(cloudLeft, READWRITE, ON_CHANGE, onCloudLeftChange);
  ArduinoCloud.addProperty(cloudLightState, READWRITE, ON_CHANGE, onCloudLightStateChange);
  ArduinoCloud.addProperty(cloudManualCurtains, READWRITE, ON_CHANGE, onCloudManualCurtainsChange);
  ArduinoCloud.addProperty(cloudManualDrive, READWRITE, ON_CHANGE, onCloudManualDriveChange);
  ArduinoCloud.addProperty(cloudManualFan, READWRITE, ON_CHANGE, onCloudManualFanChange);
  ArduinoCloud.addProperty(cloudManualLight, READWRITE, ON_CHANGE, onCloudManualLightChange);
  ArduinoCloud.addProperty(cloudOpenCurtains, READWRITE, ON_CHANGE, onCloudOpenCurtainsChange);
  ArduinoCloud.addProperty(cloudOpenFrontCurtains, READWRITE, ON_CHANGE, onCloudOpenFrontCurtainsChange);
  ArduinoCloud.addProperty(cloudOpenLeftCurtains, READWRITE, ON_CHANGE, onCloudOpenLeftCurtainsChange);
  ArduinoCloud.addProperty(cloudOpenRearCurtains, READWRITE, ON_CHANGE, onCloudOpenRearCurtainsChange);
  ArduinoCloud.addProperty(cloudOpenRightCurtains, READWRITE, ON_CHANGE, onCloudOpenRightCurtainsChange);
  ArduinoCloud.addProperty(cloudRight, READWRITE, ON_CHANGE, onCloudRightChange);

}

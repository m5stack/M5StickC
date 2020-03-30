## BeetleC

### method description

- ```carLRcontrol(int8_t left, int8_t right)```: this function will send two value to the BeetleC base.
- ```esp_err_t control(httpd_req_t *req)```: callback function of http server, receive http requeset to control the beetleC base.
- ```esp_err_t test_handler(httpd_req_t *req)```: send back a control page when some one access ```192.168.4.1/ctl``` which can send the control message.
- ```static esp_err_t http_server_init()```: Http Server initialization. callback function will be set here.
- ```static void initWifi()```: wifi initalization.
- ```void blink()```: turn on the led one by one for 3 times.

### code logic

1. initialize the wifi and http server which allow other to connect and access the control page.
2. BeetleC will wait for the control message all the time once you start it
3. control message will be decode and use it to control the BeetleC base when some one push the control button on the control page.
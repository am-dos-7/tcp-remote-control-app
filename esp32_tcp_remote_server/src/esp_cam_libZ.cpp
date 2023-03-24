#include "esp_cam_libZ.hpp"

EspCamLib::EspCamLib()
{

}

EspCamLib::~EspCamLib()
{

}

void EspCamLib::setFrameSize(taille t)
{
    switch(t){
        case taille::QVGA:
            camera->set_framesize(camera, FRAMESIZE_QVGA);  break;

        case taille::VGA:
            camera->set_framesize(camera, FRAMESIZE_VGA);   break;
        
        //default: 
    }
}

void EspCamLib::getFrame(camera_fb_t **fb)
{
    *fb = esp_camera_fb_get();
}

void EspCamLib::releaseFrameBuffer(camera_fb_t **fb)
{
    esp_camera_fb_return(*fb);
}

void EspCamLib::begin()
{
    /* Le bloc suivant a été pris de l'exemple fourni avec la biblithèque tel quel */
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000; //5000000;
    config.pixel_format = PIXFORMAT_JPEG;
    
    // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
    //                      for larger pre-allocated frame buffer.
    if(psramFound()){
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    #if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    #endif

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    //sensor_t * s = esp_camera_sensor_get();
    camera = esp_camera_sensor_get();
    // initial sensors are flipped vertically and colors are a bit saturated
    if (camera->id.PID == OV3660_PID) {
        camera->set_vflip(camera, 1); // flip it back
        camera->set_brightness(camera, 1); // up the brightness just a bit
        camera->set_saturation(camera, -2); // lower the saturation
    }
    // drop down frame size for higher initial frame rate
    camera->set_framesize(camera, FRAMESIZE_QVGA);

    #if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
    camera->set_vflip(camera, 1);
    camera->set_hmirror(camera, 1);
    #endif
}
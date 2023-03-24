/* Début: 06/05/2021 
 * Auteur: Am_Zirée
 * 
 * Je n'ajoute pas grand chose de nouveau dans cette bibliothèque
 * Je voulais juste éviter de trimballer tout le bloc qui était fourni par l'exemple
 * dans le main. J'ai donc voulu les rassembler en une classe EspCamLib
 * 
 */

#ifndef ESP_CAM_LIBZ_HPP
#define ESP_CAM_LIBZ_HPP

//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//

/* Decommenter le modèle de caméra correspondant 
 * Ce bloc #define doit forcément venir avant l'#include de 'camaera_pins.h'
 * sinon aucun modèle ne sera reconnu (préprocesseur)
 * 
 * Notes: Le format par défaut ici est le jpg comme dans l"exemple, pour faciliter le streaming
 * 
 */

//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM

#include <Arduino.h>
#include "esp_camera.h"
#include "camera_pins.h"

/* enum pour désigner les dimensions
 * Juste pour une question de lisibilité, sinon l'enum 
 * framesize_t sert déjà à cet effet.
 * A noter également que je me limite ici à deux valeurs uniquement
 * parce les dimensions plus petites que QVGA ne m'intéressaient pas dans mon cas
 * et que celles plus grandes que VGA fournissent des frames plus volumineuses,
 * ce qui ralentit le streaming dans mon cas.
 */
typedef enum:byte{
    QVGA,  // 320*240
    VGA    // 640*480
}taille;


class EspCamLib
{
public:
    EspCamLib();
    ~EspCamLib();
    void begin();   // Fait les initialisations nécessaires
    void setFrameSize(taille);  // Définir les dimensions: QVA par défaut
    void getFrame(camera_fb_t **frame_buffer);           // Faire une prise et la sauvegarder dans un buffer passé en paramètre. (créer un pointeur dans le main et le passer par adresse)

    /* Libérer le buffer passé en paramètre pour une utilisation ultérieure. 
     * Cette fonction doit impérative être appelée avant une réutilisation du buffer, sinon cela résulte en un message
     * du genre 'failed to get frame on time'. (Après 02 prises chez moi)
     */
    void releaseFrameBuffer(camera_fb_t **frame_buffer);

    /* On peut également ajouter des fonctions de conversion entre format */

private:
    sensor_t *camera;           // Servira à la configuration des paramètres comme les dimensions de la prise
    //camera_fb_t *frame_buffer = NULL;  // Contient la prise après chaque appel de getFrame, si tout s'est bien passé
};

#endif
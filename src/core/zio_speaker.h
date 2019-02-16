
#define ZIO_SPEAKER_MIN_MS 24 
#define ZIO_SPEAKER_MAX_MS 40
#define ZIO_SPEAKER_MW 1

int zio_speaker_init(zdev_t *dev);
int zio_speaker_write(zdev_t *dev);
int zio_speaker_timer(zdev_t *dev);
int zio_speaker_term(zdev_t *dev);


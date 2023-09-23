
typedef enum {
  YORICK_SEQUENCER_MODE,
  YORICK_PLAY_MODE,
} YorickMode;

#define disable_interrupts cli()

#define enable_interrupts sei()

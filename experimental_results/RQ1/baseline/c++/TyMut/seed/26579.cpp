
typedef struct __attribute__((__packed__))
{
    /** Every frame starts with BB_FRAME_MAGIC. */
    uint32_t magic;
    unsigned char t;

    fcomplex_t bb_iq_samples[BB_FRAME_IQ_SAMPLES_COUNT];

} bb_frame_t;
int t = sizeof(bb_frame_t);


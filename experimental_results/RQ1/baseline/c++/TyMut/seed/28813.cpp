

    aligned_float *data = static_cast<aligned_float *>(aligned_alloc(64, 8192*sizeof(float)));

    float sum = 0.0f;
    for (int i=1; i<8192; i++)
      sum += data[i];


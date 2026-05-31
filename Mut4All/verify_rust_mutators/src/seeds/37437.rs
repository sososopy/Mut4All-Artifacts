# ! [feature (asm)] fn xor_buf (buf : [u32 ; 4] , buf2 : [u32 ; 4]) { let res : [u32 ; 4] ; unsafe { asm ! ("
        movaps xmm0, 1
        xorps xmm0, 2
        movaps 1, xmm0" : "=r" (res) : "a" (buf) , "b" (buf) : "xmm0" : "intel") } ; }
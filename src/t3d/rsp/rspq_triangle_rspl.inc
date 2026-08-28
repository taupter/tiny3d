#undef zero
#undef at
#undef v0
#undef v1
#undef a0
#undef a1
#undef a2
#undef a3
#undef t0
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef s0
#undef s1
#undef s2
#undef s3
#undef s4
#undef s5
#undef s6
#undef s7
#undef t8
#undef t9
#undef k0
#undef k1
#undef gp
#undef sp
#undef fp
#undef ra
.equ hex.$zero, 0
.equ hex.$at, 1
.equ hex.$v0, 2
.equ hex.$v1, 3
.equ hex.$a0, 4
.equ hex.$a1, 5
.equ hex.$a2, 6
.equ hex.$a3, 7
.equ hex.$t0, 8
.equ hex.$t1, 9
.equ hex.$t2, 10
.equ hex.$t3, 11
.equ hex.$t4, 12
.equ hex.$t5, 13
.equ hex.$t6, 14
.equ hex.$t7, 15
.equ hex.$s0, 16
.equ hex.$s1, 17
.equ hex.$s2, 18
.equ hex.$s3, 19
.equ hex.$s4, 20
.equ hex.$s5, 21
.equ hex.$s6, 22
.equ hex.$s7, 23
.equ hex.$t8, 24
.equ hex.$t9, 25
.equ hex.$k0, 26
.equ hex.$k1, 27
.equ hex.$gp, 28
.equ hex.$sp, 29
.equ hex.$fp, 30
.equ hex.$ra, 31
#define vco 0
#define vcc 1
#define vce 2
.set noat

T3DCmd_TriDraw:
  ## hot-path cycles: 2
  lbu $v0, %lo(FACE_CULLING + 0)                     ## L:62   |      ^ | u8<$v0> cullFlag = load(FACE_CULLING);
  srl $a2, $a1, 16                                   ## L:61   |      2 | u32<$a2> vert2 = vert1 >> 16;
RDPQ_Triangle_Send_Async:
  ## hot-path cycles: 143
  mtc2 $a0, $v01.e3                                  ## L:97   |      ^ | vall1.w = vtx1;
  mtc2 $a1, $v02.e3                                  ## L:98   |      2 | vall2.w = vtx2;
  vsubc $v27, $v00, $v30.v                           ## L:106  |      ^ | vec16<$v27> vmconst = VZERO - VSHIFT;
  vaddc $v12, $v00, $v30.e7                          ## L:120  |      3 | vec16<$v12> vk1 = VZERO + 1;
  llv $v01, 0, 0, $a0                                ## L:101  |      ^ | vall1.xy = load(vtx1, 0x00).xy;
  llv $v02, 0, 0, $a1                                ## L:102  |      4 | vall2.xy = load(vtx2, 0x00).xy;
  lhu $t4, 6($a1)                                    ## L:109  |      5 | u16<$t4> clip2 = load(vtx2, 0x06);
  llv $v03, 0, 0, $a2                                ## L:103  |      6 | vall3.xy = load(vtx3, 0x00).xy;
  lhu $t3, 6($a0)                                    ## L:108  |      7 | u16<$t3> clip1 = load(vtx1, 0x06);
  vor $v06, $v00, $v01.e1                            ## L:112  |      ^ | vec16<$v06> vy1 = vall1.y;
  lhu $t5, 6($a2)                                    ## L:110  |      8 | u16<$t5> clip3 = load(vtx3, 0x06);
  vor $v07, $v00, $v02.e1                            ## L:113  |      ^ | vec16<$v07> vy2 = vall2.y;
  lhu $s7, %lo(RDPQ_TRI_BUFF_OFFSET + 0)             ## L:145  |      9 | u16<$s7> buffOffset = load(RDPQ_TRI_BUFF_OFFSET);
  or $t3, $t3, $t4                                   ## L:122  |     10 | clip1 |= clip2;
  or $t3, $t3, $t5                                   ## L:123  |     11 | clip1 |= clip3;
  vor $v08, $v00, $v03.e1                            ## L:114  |      ^ | vec16<$v08> vy3 = vall3.y;
  mtc2 $a2, $v03.e3                                  ## L:99   |     12 | vall3.w = vtx3;
  vge $v09, $v06, $v07                               ## L:136  |      ^ | vytmp1 = max(vy1, vy2);
  vmrg $v04, $v01, $v02                              ## L:137  |     13 | valltmp1 = select(vall1, vall2);
  andi $t6, $t3, 255                                 ## L:149  |      ^ | u32<$t6> rejCodes = clip1 & 0xFF;
  addiu $at, $zero, 255                              ## L:150  |     14 | if(rejCodes != 0xFF)goto JrRa;
  vlt $v06, $v06, $v07                               ## L:138  |      ^ | vy1 = min(vy1, vy2);
  cfc2 $t0, $vcc                                     ## L:140  |     15 | did_swap_0 = get_vcc();
  vmrg $v01, $v01, $v02                              ## L:139  |      ^ | vall1 = select(vall1, vall2);
  bne $t6, $at, JrRa                                 ## L:150  |     16 | if(rejCodes != 0xFF)goto JrRa;
  vxor $v28, $v28, $v28.v                            ## L:143  |    *18 | vhmlupp ^= vhmlupp;
  vge $v10, $v06, $v08                               ## L:153  |     19 | vytmp2 = max(vy1, vy3);
  vmrg $v05, $v01, $v03                              ## L:154  |     20 | valltmp2 = select(vall1, vall3);
  srl $t2, $t3, 13                                   ## L:165  |      ^ | u32<$t2> tileIdx = clip1 >> 13;
  andi $t4, $t3, 7936                                ## L:175  |     21 | u32<$t4> clipCodes = clip1 & 0x1F00;
  vlt $v06, $v06, $v08                               ## L:155  |      ^ | vy1 = min(vy1, vy3);
  cfc2 $t1, $vcc                                     ## L:157  |     22 | did_swap_1 = get_vcc();
  vmrg $v01, $v01, $v03                              ## L:156  |      ^ | vall1 = select(vall1, vall3);
  vge $v08, $v09, $v10                               ## L:167  |     23 | vy3 = max(vytmp1, vytmp2);
  addiu $s3, $s7, %lo(CLIP_BUFFER_TMP)               ## L:176  |      ^ | u16<$s3> outPtr = buffOffset + CLIP_BUFFER_TMP;
  lw $a3, %lo(TRI_COMMAND + 0)                       ## L:159  |     24 | u32<$a3> triCmd = load(TRI_COMMAND);
  vmrg $v03, $v04, $v05                              ## L:168  |      ^ | vall3 = select(valltmp1, valltmp2);
  vlt $v07, $v09, $v10                               ## L:169  |     25 | vy2 = min(vytmp1, vytmp2);
  ssv $v06, 0, 6, $s3                                ## L:177  |      ^ | @Barrier("tri-out", before) store(vy1.x, outPtr, 6);
  vmrg $v02, $v04, $v05                              ## L:170  |     26 | vall2 = select(valltmp1, valltmp2);
  bne $t4, $zero, RDPQ_Triangle_Clip                 ## L:181  |      ^ | if(clipCodes != 0)goto RDPQ_Triangle_Clip;
  cfc2 $t5, $vcc                                     ## L:171  |    *28 | u32<$t5> did_swap_2 = get_vcc();
  mfc2 $a2, $v03.e3                                  ## L:198  |    *30 | vtx3 = vall3.w;
  vsubc $v24, $v03, $v01.v                           ## L:190  |      ^ | vec16<$v24> vhml = vall3 - vall1;
  xor $t0, $t0, $t1                                  ## L:184  |     31 | did_swap_0 ^= did_swap_1;
  xori $s7, $s7, 176                                 ## L:216  |     32 | buffOffset ^= 176;
  vsubc $v05, $v02, $v01.v                           ## L:192  |      ^ | vec16<$v05> vm = vall2 - vall1;
  mfc2 $a0, $v01.e3                                  ## L:196  |     33 | vtx1 = vall1.w;
  vsubc $v04, $v03, $v02.v                           ## L:193  |      ^ | vec16<$v04> vl = vall3 - vall2;
  vmov $v23.e2, $v02.e0                              ## L:203  |     34 | vx12F.z = vall2.x;
  xor $t0, $t0, $t5                                  ## L:185  |      ^ | did_swap_0 ^= did_swap_2;
  andi $t0, $t0, 1                                   ## L:205  |     35 | did_swap_0 &= 1;
  vmudm $v25, $v06, $v31.e1                          ## L:215  |      ^ | vec16<$v25> vfyI = vy1 >> 2;
  addiu $t4, $s3, 32                                 ## L:244  |     36 | u32<$t4> color_dmem = outPtr + 0x20;
  vsubc $v29, $v00, $v05.e1                          ## L:211  |      ^ | VTEMP = VZERO - vm.y;
  mfc2 $a1, $v02.e3                                  ## L:197  |     37 | vtx2 = vall2.w;
  vmov $v23.e0, $v01.e0                              ## L:202  |      ^ | vx12F.x = vall1.x;
  vmudn $v26, $v06, $v31.e1                          ## L:214  |     38 | vec16<$v26> vfyF = vy1 << 14;
  ssv $v08, 0, 2, $s3                                ## L:219  |      ^ | @Barrier("tri-out", before) store(vy3.x, outPtr, 2);
  lsv $v08, 8, 30, $a2                               ## L:255  |     39 | vwF.X = load(vtx3, 0x1E).x;
  vmudh $v19, $v05, $v24.e1                          ## L:226  |      ^ | vnzF = vm:sint * vhml:sint.y;
  vmadh $v19, $v24, $v29.v                           ## L:227  |     40 | vnzF = vhml:sint +* VTEMP:sint;
  slv $v05, 0, 8, $s3                                ## L:238  |      ^ | @Barrier("tri-oA", before) @Barrier("tri-out", before) store(vm.xy, outPtr, 8);
  vsar $v18, COP2_ACC_HI                             ## L:228  |     41 | vnzI = get_acc_high();
  xor $t1, $v0, $t0                                  ## L:206  |      ^ | u32<$t1> cullFlip = cull ^ did_swap_0;
  slv $v04, 0, 12, $s3                               ## L:239  |     42 | @Barrier("tri-oA", before) @Barrier("tri-out", before) store(vl.xy, outPtr, 12);
  ssv $v07, 0, 4, $s3                                ## L:218  |     43 | @Barrier("tri-out", before) store(vy2.x, outPtr, 4);
  vsar $v19, COP2_ACC_MD                             ## L:229  |      ^ | vnzF = get_acc_mid();
  vsubc $v26, $v00, $v26.v                           ## L:248  |     44 | vfyF = VZERO - vfyF;
  ldv $v24, 4, 8, $s3                                ## L:240  |      ^ | @Barrier("tri-oA", after) @Barrier("tri-oA2", before) vhml.z = load(outPtr, 8).xyzw;
  vsub $v25, $v25, $v25.v                            ## L:249  |     45 | vfyI:sint = vfyI - vfyI;
  mfc2 $t0, $v18.e0                                  ## L:258  |      ^ | s32<$t0> nzSign = vnzI.x;
  vrcph $v20.e7, $v18.e0                             ## L:270  |     46 | vslope.W = invert_half(vnz).x;
  lsv $v08, 0, 30, $a0                               ## L:253  |      ^ | vwF.x = load(vtx1, 0x1E).x;
  vrcpl $v21.e7, $v19.e0                             ## L:270  |     47 | vslope.W = invert_half(vnz).x;
  lsv $v08, 4, 30, $a1                               ## L:254  |      ^ | vwF.z = load(vtx2, 0x1E).x;
  vrcph $v20.e7, $v00.e0                             ## L:270  |     48 | vslope.W = invert_half(vnz).x;
  TAG_CullFlipA: slt $t0, $t0, $zero                                ## L:275  |      ^ | @Tag("CullFlipA") asm_op("slt", nzSign, nzSign, ZERO);
  vrcp $v21.e0, $v24.e1                              ## L:274  |     49 | vslope.x = invert_half(vhml).y;
  beq $t0, $t1, JrRa                                 ## L:276  |      ^ | if(nzSign == cullFlip)goto JrRa;
  vrcph $v20.e0, $v24.e1                             ## L:274  |    *51 | vslope.x = invert_half(vhml).y;
  TAG_CullFlipB: xori $t0, $t0, 1                                   ## L:304  |     52 | @Tag("CullFlipB") nzSign ^= 1;
  vrcp $v21.e2, $v05.e1                              ## L:291  |      ^ | vslope.z = invert_half(vm).y;
  mfc2 $t1, $v24.e0                                  ## L:350  |     53 | u32<$t1> negHX = vhml.x;
  vrcph $v20.e2, $v05.e1                             ## L:291  |      ^ | vslope.z = invert_half(vm).y;
  lsv $v15, 8, 34, $a0                               ## L:337  |     54 | vinvwF.X = load(vtx1, 0x22).x;
  vrcp $v21.e4, $v04.e1                              ## L:293  |      ^ | vslope.X = invert_half(vl).y;
  vrcph $v20.e4, $v04.e1                             ## L:293  |     55 | vslope.X = invert_half(vl).y;
  lsv $v07, 4, 22, $a1                               ## L:296  |      ^ | vwI.z = load(vtx2, 0x16).x;
  lsv $v07, 8, 22, $a2                               ## L:297  |     56 | vwI.X = load(vtx3, 0x16).x;
  vmov $v24.e7, $v19.e0                              ## L:287  |      ^ | vhml.W = vnzF.x;
  lsv $v07, 0, 22, $a0                               ## L:295  |     57 | vwI.x = load(vtx1, 0x16).x;
  vmov $v28.e7, $v18.e0                              ## L:288  |      ^ | vhmlupp.W = vnzI.x;
  vmudn $v01, $v21, $v27.e5                          ## L:317  |     58 | vec16<$v01> vslopem2F = vslopeF:ufract * vmconst:sint.Y;
  sll $t0, $t0, 7                                    ## L:305  |      ^ | nzSign <<= 7;
  vmadh $v02, $v20, $v27.e5                          ## L:318  |     59 | vec16<$v02> vslopem2I = vslopeI:sint +* vmconst:sint.Y;
  or $a3, $a3, $t0                                   ## L:323  |      ^ | triCmd |= nzSign;
  vsubc $v29, $v08, $v08.e2                          ## L:321  |     60 | VTEMP = vwF - vwF.z;
  lsv $v14, 8, 32, $a0                               ## L:363  |      ^ | vinvwI.X = load(vtx1, 0x20).x;
  andi $t5, $a3, 1024                                ## L:341  |     61 | u32<$t5> tex_dmem = triCmd & 0x400;
  vlt $v07, $v07, $v07.e2                            ## L:322  |      ^ | vwI = min(vwI, vwI.z);
  lsv $v14, 10, 32, $a1                              ## L:361  |     62 | vinvwI.Y = load(vtx2, 0x20).x;
  vmrg $v08, $v08, $v08.e2                           ## L:324  |      ^ | vwF = select(vwF, vwF.z);
  vmudl $v03, $v21, $v24.q1                          ## L:335  |     63 | asm_op("vmudl", vtmpF, vslopeF, vhml.yywwYYWW);
  luv $v09, 0, 8, $a0                                ## L:308  |      ^ | vec16<$v09> vattr1 = load_vec_u8(vtx1, 0x08);
  lsv $v14, 12, 32, $a2                              ## L:362  |     64 | vinvwI.Z = load(vtx3, 0x20).x;
  vmadm $v03, $v20, $v24.q1                          ## L:336  |      ^ | asm_op("vmadm", vtmpF, vslopeI, vhml.yywwYYWW);
  vmadn $v03, $v21, $v28.v                           ## L:344  |     65 | vtmpF = vslopeF:ufract +* vhmlupp:sint;
  luv $v10, 0, 8, $a1                                ## L:309  |      ^ | vec16<$v10> vattr2 = load_vec_u8(vtx2, 0x08);
  vmadh $v04, $v20, $v28.v                           ## L:345  |     66 | vtmpI = vslopeI:sint +* vhmlupp:sint;
  addiu $at, $zero, 207                              ## L:436  |      ^ | set_vcc(0xCF);
  vmadn $v03, $v12, $v27.e7                          ## L:351  |     67 | vtmpF = vk1:ufract +* vmconst:sint.W;
  andi $t6, $a3, 512                                 ## L:376  |      ^ | u32<$t6> z_dmem = triCmd & 0x200;
  vmadh $v04, $v12, $v27.e7                          ## L:352  |     68 | vtmpI = vk1:sint +* vmconst:sint.W;
  luv $v11, 0, 8, $a2                                ## L:307  |      ^ | vec16<$v11> vattr3 = load_vec_u8(vtx3, 0x08);
  vsubc $v29, $v08, $v08.e4                          ## L:357  |     69 | VTEMP = vwF - vwF.X;
  lsv $v15, 12, 34, $a2                              ## L:329  |      ^ | vinvwF.Z = load(vtx3, 0x22).x;
  subu $t1, $zero, $t1                               ## L:427  |     70 | negHX = ZERO - negHX;
  vlt $v07, $v07, $v07.e4                            ## L:358  |      ^ | vwI = min(vwI, vwI.X);
  srl $t5, $t5, 4                                    ## L:342  |     71 | tex_dmem >>= 4;
  vmrg $v08, $v08, $v08.e4                           ## L:359  |      ^ | vwF = select(vwF, vwF.X);
  vmudl $v29, $v03, $v01                             ## L:370  |     72 | asm_op("vmudl", VTEMP, vtmpF, vslopem2F);
  lsv $v15, 10, 34, $a1                              ## L:338  |      ^ | vinvwF.Y = load(vtx2, 0x22).x;
  vmadm $v29, $v04, $v01                             ## L:371  |     73 | asm_op("vmadm", VTEMP, vtmpI, vslopem2F);
  mfc2 $t0, $v24.e3                                  ## L:349  |      ^ | u32<$t0> negMY = vhml.w;
  vmadn $v19, $v03, $v02.v                           ## L:372  |     74 | vnzF = vtmpF:ufract +* vslopem2I:sint;
  llv $v06, 8, 12, $a2                               ## L:412  |      ^ | vtex3.XY = load(vtx3, 0x0C).xy;
  vmadh $v18, $v04, $v02.v                           ## L:373  |     75 | vnzI = vtmpI:sint +* vslopem2I:sint;
  or $a3, $a3, $t2                                   ## L:383  |      ^ | triCmd |= tileIdx;
  vmudl $v29, $v15, $v08.e0                          ## L:401  |     76 | asm_op("vmudl", VTEMP, vinvwF, vwF.x);
  llv $v04, 8, 12, $a0                               ## L:410  |      ^ | vtex1.XY = load(vtx1, 0x0C).xy;
  vmadm $v29, $v14, $v08.e0                          ## L:402  |     77 | asm_op("vmadm", VTEMP, vinvwI, vwF.x);
  ctc2 $at, $vcc                                     ## L:436  |      ^ | set_vcc(0xCF);
  vmadn $v15, $v15, $v07.e0                          ## L:403  |     78 | vinvwF = vinvwF:ufract +* vwI:sint.x;
  llv $v03, 8, 12, $a1                               ## L:411  |      ^ | vtex2.XY = load(vtx2, 0x0C).xy;
  vmudn $v29, $v19, $v24.v                           ## L:420  |     79 | VTEMP = vnzF:ufract * vhml:sint;
  subu $t0, $zero, $t0                               ## L:426  |      ^ | negMY = ZERO - negMY;
  vmadh $v29, $v18, $v24.v                           ## L:421  |     80 | VTEMP = vnzI:sint +* vhml:sint;
  addu $t5, $t5, $t4                                 ## L:378  |      ^ | tex_dmem += color_dmem;
  vsar $v21, COP2_ACC_MD                             ## L:422  |     81 | vslopeF = get_acc_mid();
  sh $a3, 0($s3)                                     ## L:385  |      ^ | @Barrier("tri-out", before) store(triCmd:u16, outPtr, 0);
  vsar $v20, COP2_ACC_HI                             ## L:423  |     82 | vslopeI = get_acc_high();
  mtc2 $t0, $v24.e3                                  ## L:428  |      ^ | vhml.w = negMY;
  vsubc $v15, $v15, $v30.e7                          ## L:433  |     83 | vinvwF -= 1;
  mtc2 $t1, $v24.e0                                  ## L:429  |      ^ | vhml.x = negHX;
  vmudl $v10, $v10, $v31.e6                          ## L:440  |     84 | vattr2 >>>= 7;
  lbu $t0, %lo(RDPQ_SYNCFULL_ONGOING + 0)            ## L:483  |      ^ | u8<$t0> syncFull = load(RDPQ_SYNCFULL_ONGOING);
  lsv $v10, 14, 4, $a1                               ## L:445  |     85 | vattr2.W = load(vtx2, 0x04).x;
  vmudl $v09, $v09, $v31.e6                          ## L:439  |      ^ | vattr1 >>>= 7;
  vmudl $v11, $v11, $v31.e6                          ## L:441  |     86 | vattr3 >>>= 7;
  andi $t3, $a3, 256                                 ## L:462  |      ^ | u32<$t3> prim_size = triCmd & 0x100;
  lsv $v11, 14, 4, $a2                               ## L:446  |     87 | vattr3.W = load(vtx3, 0x04).x;
  vmudl $v15, $v15, $v31.e0                          ## L:450  |      ^ | vinvwF >>>= 1;
  mfc0 $t1, COP0_DMA_BUSY                            ## L:464  |     88 | u32<$t1> dmaBusy = get_dma_busy();
  vmudl $v29, $v21, $v26.e4                          ## L:460  |      ^ | vec32<$v16> vedges = vslope * vfy.X;
  vmadm $v29, $v20, $v26.e4                          ## L:460  |     89 | vec32<$v16> vedges = vslope * vfy.X;
  srl $t6, $t6, 3                                    ## L:377  |      ^ | z_dmem >>= 3;
  vmadn $v17, $v21, $v25.e4                          ## L:460  |     90 | vec32<$v16> vedges = vslope * vfy.X;
  lsv $v09, 14, 4, $a0                               ## L:444  |      ^ | vattr1.W = load(vtx1, 0x04).x;
  vmadh $v16, $v20, $v25.e4                          ## L:460  |     91 | vec32<$v16> vedges = vslope * vfy.X;
  sdv $v15, 8, 16, $s3                               ## L:467  |      ^ | @Barrier("tri-oB", before) @Barrier("tri-out", before) store(vinvwF.XYZW, outPtr, 16);
  lsv $v11, 12, 20, $s3                              ## L:470  |     92 | @Barrier("tri-oB", after) @Barrier("tri-oB2", before) vattr3.Z = load(outPtr, 20).x;
  vmulf $v06, $v06, $v15.h2                          ## L:475  |      ^ | vtex3:sfract *= vinvwF.zzzzZZZZ;
  vmulf $v04, $v04, $v15.h0                          ## L:473  |     93 | vtex1:sfract *= vinvwF.xxxxXXXX;
  lsv $v09, 12, 16, $s3                              ## L:468  |      ^ | @Barrier("tri-oB", after) @Barrier("tri-oB2", before) vattr1.Z = load(outPtr, 16).x;
  vmulf $v03, $v03, $v15.h1                          ## L:474  |     94 | vtex2:sfract *= vinvwF.yyyyYYYY;
  lsv $v10, 12, 18, $s3                              ## L:469  |      ^ | @Barrier("tri-oB", after) @Barrier("tri-oB2", before) vattr2.Z = load(outPtr, 18).x;
  vmudm $v22, $v23, $v31.e1                          ## L:480  |     95 | vec16<$v22> vx12I = vx12F >> 2;
  bne $t1, $zero, LABEL_RDPQ_Triangle_Send_Async_0006## L:493  |      ^ | @Unlikely if(dmaBusy != 0)
  vmudn $v23, $v23, $v31.e1                          ## L:481  |     96 | vx12F <<= 14;
  LABEL_RDPQ_Triangle_Send_Async_0005:
  lw $a0, %lo(RDPQ_CURRENT + 0)                      ## L:522  |     97 | u32<$a0> rdramCur = load(RDPQ_CURRENT);
  ssv $v20, 8, 12, $s3                               ## L:517  |     98 | @Barrier("tri-oA2", after) @Barrier("tri-out", before) store(vslope:sint.X, outPtr, 12);
  vmrg $v09, $v09, $v04                              ## L:509  |      ^ | vattr1 = select(vattr1, vtex1);
  srl $t3, $t3, 4                                    ## L:505  |     99 | prim_size >>= 4;
  vmrg $v10, $v10, $v03                              ## L:510  |      ^ | vattr2 = select(vattr2, vtex2);
  ssv $v22, 4, 8, $s3                                ## L:518  |    100 | @Barrier("tri-oA2", after) @Barrier("tri-out", before) store(vx12I.z, outPtr, 8);
  vmrg $v11, $v11, $v06                              ## L:511  |      ^ | vattr3 = select(vattr3, vtex3);
  ssv $v21, 8, 14, $s3                               ## L:519  |    101 | @Barrier("tri-oA2", after) @Barrier("tri-out", before) store(vslope:ufract.X, outPtr, 14);
  vaddc $v17, $v17, $v23.e0                          ## L:527  |      ^ | vedges += vx12.x;
  vadd $v16, $v16, $v22.e0                           ## L:527  |    102 | vedges += vx12.x;
  bne $t0, $zero, LABEL_RDPQ_Triangle_Send_Async_0008## L:535  |      ^ | if(syncFull == 0) {
  addu $t6, $t6, $t5                                 ## L:504  |   *104 | z_dmem += tex_dmem;
  mtc0 $a0, COP0_DP_END                              ## L:536  |    105 | set_rdp_end(rdramCur);
  LABEL_RDPQ_Triangle_Send_Async_0008:
  vsubc $v29, $v00, $v00                             ## L:548  |      ^ | clear_vcc();
  vsub $v12, $v10, $v09.v                            ## L:558  |    106 | vec16<$v12> vma:sint = vattr2 - vattr1;
  addu $t3, $t3, $t6                                 ## L:612  |      ^ | prim_size += z_dmem;
  lw $a2, %lo(RDPQ_SENTINEL + 0)                     ## L:568  |    107 | u32<$a2> rdramSentinel = load(RDPQ_SENTINEL);
  vsub $v13, $v11, $v09.v                            ## L:559  |      ^ | vec16<$v13> vha:sint = vattr3 - vattr1;
  ssv $v20, 4, 28, $s3                               ## L:580  |    108 | @Barrier("tri-out", before) store(vslope:sint.z, outPtr, 28);
  vmudn $v19, $v19, $v30.e5                          ## L:566  |      ^ | vnz = vnz * 4;
  subu $t3, $t3, $s3                                 ## L:613  |    109 | prim_size -= outPtr;
  vmadh $v18, $v18, $v30.e5                          ## L:566  |      ^ | vnz = vnz * 4;
  sh $s7, %lo(RDPQ_TRI_BUFF_OFFSET)($zero)           ## L:617  |    110 | store(buffOffset, RDPQ_TRI_BUFF_OFFSET);
  vmudh $v29, $v12, $v24.e1                          ## L:573  |      ^ | VTEMP = vma:sint * vhml:sint.y;
  vmadh $v29, $v13, $v24.e3                          ## L:574  |    111 | VTEMP = vha:sint +* vhml:sint.w;
  ssv $v17, 0, 18, $s3                               ## L:586  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out", before) store(vedges:ufract.x, outPtr, 18);
  vsar $v04, COP2_ACC_MD                             ## L:575  |    112 | vdxF = get_acc_mid();
  ssv $v23, 4, 10, $s3                               ## L:544  |      ^ | @Barrier("tri-oA2", after) @Barrier("tri-out", before) store(vx12F.z, outPtr, 10);
  vsar $v03, COP2_ACC_HI                             ## L:576  |    113 | vdxI = get_acc_high();
  ssv $v20, 0, 20, $s3                               ## L:600  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out", before) store(vslope:sint.x, outPtr, 20);
  vmudh $v29, $v13, $v24.e2                          ## L:592  |    114 | VTEMP = vha:sint * vhml:sint.z;
  ssv $v17, 4, 26, $s3                               ## L:584  |      ^ | @Barrier("tri-out", before) store(vedges:ufract.z, outPtr, 26);
  vmadh $v29, $v12, $v24.e0                          ## L:593  |    115 | VTEMP = vma:sint +* vhml:sint.x;
  ssv $v21, 0, 22, $s3                               ## L:601  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out", before) store(vslope:ufract.x, outPtr, 22);
  vsar $v08, COP2_ACC_MD                             ## L:594  |    116 | vdyF = get_acc_mid();
  ssv $v16, 4, 24, $s3                               ## L:583  |      ^ | @Barrier("tri-out", before) store(vedges:sint.z, outPtr, 24);
  vsar $v07, COP2_ACC_HI                             ## L:595  |    117 | vdyI = get_acc_high();
  ssv $v21, 4, 30, $s3                               ## L:581  |      ^ | @Barrier("tri-out", before) store(vslope:ufract.z, outPtr, 30);
  vmudl $v29, $v04, $v19.e7                          ## L:606  |    118 | vdx *= vnz.W;
  mtc0 $s3, COP0_DMA_SPADDR                          ## L:603  |      ^ | @Barrier("tri-out", before) set_dma_addr_rsp(outPtr);
  vmadm $v29, $v03, $v19.e7                          ## L:606  |    119 | vdx *= vnz.W;
  mtc0 $a0, COP0_DMA_RAMADDR                         ## L:615  |      ^ | @Barrier("tri-out", before) set_dma_addr_rdram(rdramCur);
  vmadn $v04, $v04, $v18.e7                          ## L:606  |    120 | vdx *= vnz.W;
  addu $a0, $a0, $t3                                 ## L:621  |      ^ | rdramCur += prim_size;
  vmadh $v03, $v03, $v18.e7                          ## L:606  |    121 | vdx *= vnz.W;
  sltu $at, $a2, $a0                                 ## L:643  |      ^ | @Unlikely if(rdramSentinel < rdramCur)
  vmudl $v29, $v08, $v19.e7                          ## L:608  |    122 | vdy *= vnz.W;
  ssv $v16, 0, 16, $s3                               ## L:585  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out", before) store(vedges:sint.x, outPtr, 16);
  vmadm $v29, $v07, $v19.e7                          ## L:608  |    123 | vdy *= vnz.W;
  addu $s3, $s3, $t3                                 ## L:614  |      ^ | outPtr += prim_size;
  vmadn $v08, $v08, $v18.e7                          ## L:608  |    124 | vdy *= vnz.W;
  sdv $v04, 0, 24, $t4                               ## L:624  |      ^ | @Barrier("tri-out", before) store(vdx:ufract.xyzw, color_dmem, 0x18);
  vmadh $v07, $v07, $v18.e7                          ## L:608  |    125 | vdy *= vnz.W;
  sdv $v04, 8, 24, $t5                               ## L:625  |      ^ | @Barrier("tri-out", before) store(vdx:ufract.XYZW, tex_dmem, 0x18);
  vmadl $v29, $v04, $v21.e0                          ## L:630  |    126 | vec32<$v05> vde = vdx +* vslope.x;
  sdv $v03, 8, 8, $t5                                ## L:627  |      ^ | @Barrier("tri-out", before) store(vdx:sint.XYZW, tex_dmem, 0x08);
  vmadm $v29, $v03, $v21.e0                          ## L:630  |    127 | vec32<$v05> vde = vdx +* vslope.x;
  sdv $v03, 0, 8, $t4                                ## L:626  |      ^ | @Barrier("tri-out", before) store(vdx:sint.xyzw, color_dmem, 0x08);
  vmadn $v06, $v04, $v20.e0                          ## L:630  |    128 | vec32<$v05> vde = vdx +* vslope.x;
  sdv $v08, 0, 56, $t4                               ## L:634  |      ^ | @Barrier("tri-out", before) store(vdy:ufract.xyzw, color_dmem, 0x38);
  vmadh $v05, $v03, $v20.e0                          ## L:630  |    129 | vec32<$v05> vde = vdx +* vslope.x;
  bne $at, $zero, LABEL_RDPQ_Triangle_Send_Async_0009## L:643  |      ^ | @Unlikely if(rdramSentinel < rdramCur)
  sdv $v08, 8, 56, $t5                               ## L:637  |    130 | @Barrier("tri-out", before) store(vdy:ufract.XYZW, tex_dmem, 0x38);
  LABEL_RDPQ_Triangle_Send_Async_000A:
  addiu $t3, $t3, 65535                              ## L:723  |    131 | prim_size -= 1;
  vmudh $v29, $v09, $v30.e7                          ## L:674  |      ^ | VTEMP = vattr1:sint * 1;
  sw $a0, %lo(RDPQ_CURRENT)($zero)                   ## L:687  |    132 | store(rdramCur, RDPQ_CURRENT);
  vmadl $v29, $v06, $v26.e4                          ## L:675  |      ^ | vec32<$v01> vfinal = vde +* vfy.X;
  vmadm $v29, $v05, $v26.e4                          ## L:675  |    133 | vec32<$v01> vfinal = vde +* vfy.X;
  sdv $v07, 8, 40, $t5                               ## L:679  |      ^ | @Barrier("tri-out", before) store(vdy:sint.XYZW, tex_dmem, 0x28);
  vmadn $v02, $v06, $v25.e4                          ## L:675  |    134 | vec32<$v01> vfinal = vde +* vfy.X;
  sdv $v06, 8, 48, $t5                               ## L:682  |      ^ | @Barrier("tri-out", before) store(vde:ufract.XYZW, tex_dmem, 0x30);
  sdv $v06, 0, 48, $t4                               ## L:681  |    135 | @Barrier("tri-out", before) store(vde:ufract.xyzw, color_dmem, 0x30);
  vmadh $v01, $v05, $v25.e4                          ## L:675  |      ^ | vec32<$v01> vfinal = vde +* vfy.X;
  sdv $v07, 0, 40, $t4                               ## L:678  |    136 | @Barrier("tri-out", before) store(vdy:sint.xyzw, color_dmem, 0x28);
  vmov $v06.e6, $v05.e7                              ## L:708  |      ^ | vdeF.Z = vdeI.W;
  vmov $v08.e6, $v07.e7                              ## L:707  |    137 | vdyF.Z = vdyI.W;
  sdv $v05, 8, 32, $t5                               ## L:684  |      ^ | @Barrier("tri-out", before) store(vde:sint.XYZW, tex_dmem, 0x20);
  vmov $v10.e2, $v03.e7                              ## L:712  |    138 | vzout.z = vdxI.W;
  sdv $v02, 8, 16, $t5                               ## L:716  |      ^ | @Barrier("tri-out", before) store(vfinalF.XYZW, tex_dmem, 0x10);
  sdv $v02, 0, 16, $t4                               ## L:715  |    139 | @Barrier("tri-out", before) store(vfinalF.xyzw, color_dmem, 0x10);
  vmov $v10.e3, $v04.e7                              ## L:711  |      ^ | vzout.w = vdxF.W;
  sdv $v01, 8, 0, $t5                                ## L:718  |    140 | @Barrier("tri-out", before) store(vfinalI.XYZW, tex_dmem, 0x00);
  vmov $v10.e0, $v01.e7                              ## L:710  |      ^ | vzout.x = vfinalI.W;
  vmov $v10.e1, $v02.e7                              ## L:709  |    141 | vzout.y = vfinalF.W;
  sdv $v01, 0, 0, $t4                                ## L:717  |      ^ | @Barrier("tri-out", before) store(vfinalI.xyzw, color_dmem, 0x00);
  sdv $v05, 0, 32, $t4                               ## L:683  |    142 | @Barrier("tri-out", before) store(vde:sint.xyzw, color_dmem, 0x20);
  slv $v08, 12, 12, $t6                              ## L:722  |    143 | @Barrier("tri-out", before) store(vdyF.ZW, z_dmem, 0x0C);
  slv $v06, 12, 8, $t6                               ## L:721  |    144 | @Barrier("tri-out", before) store(vdeF.ZW, z_dmem, 0x08);
  sdv $v10, 0, 0, $t6                                ## L:724  |    145 | @Barrier("tri-out", before) store(vzout.xyzw, z_dmem, 0x00);
  jr $ra                                             ## L:736  |    146 | }
  mtc0 $t3, COP0_DMA_WRITE                           ## L:735  |   *148 | @Barrier("tri-out", after) set_dma_write(prim_size);
  LABEL_RDPQ_Triangle_Send_Async_0006:
  mfc0 $t1, COP0_DMA_BUSY                            ## L:496  |    149 | dmaBusy = get_dma_busy();
  bne $t1, $zero, LABEL_RDPQ_Triangle_Send_Async_0006## L:496  |  **152 | dmaBusy = get_dma_busy();
  nop                                                ## L:496  |   *154 | dmaBusy = get_dma_busy();
  LABEL_RDPQ_Triangle_Send_Async_0007:
  j LABEL_RDPQ_Triangle_Send_Async_0005              ## L:496  |    155 | dmaBusy = get_dma_busy();
  nop                                                ## L:496  |   *157 | dmaBusy = get_dma_busy();
  LABEL_RDPQ_Triangle_Send_Async_0009:
  lw $a0, %lo(RDPQ_DYNAMIC_BUFFERS + 4)              ## L:646  |    158 | rdramCur = load(RDPQ_DYNAMIC_BUFFERS, 4);
  lui $at, 0x01                                      ## L:659  |    159 | rdramSentinel = rdramCur + 65536;
  lw $t1, %lo(RDPQ_DYNAMIC_BUFFERS + 0)              ## L:647  |    160 | u32<$t1> otherBuff = load(RDPQ_DYNAMIC_BUFFERS, 0);
  or $a3, $zero, $ra                                 ## L:645  |    161 | u32<$a3> savedRA = RA;
  addu $a2, $a0, $at                                 ## L:659  |    162 | rdramSentinel = rdramCur + 65536;
  or $a1, $zero, $a0                                 ## L:660  |    163 | u32<$a1> rdramCurCopy = rdramCur;
  sw $t1, %lo(4 + RDPQ_DYNAMIC_BUFFERS)($zero)       ## L:649  |    164 | store(otherBuff, RDPQ_DYNAMIC_BUFFERS, 4);
  sb $t3, 0($t4)                                     ## L:657  |    165 | @Barrier("cb-stash") store(prim_size:u8, color_dmem, 0x00); ## Barrier: 0x1
  jal RSPQCmd_RdpSetBuffer                           ## L:661  |    166 | RSPQCmd_RdpSetBuffer(rdramCur, rdramCurCopy, rdramSentinel); ## Args: $a0, $a1, $a2
  sw $a0, %lo(0 + RDPQ_DYNAMIC_BUFFERS)($zero)       ## L:648  |   *168 | store(rdramCur, RDPQ_DYNAMIC_BUFFERS, 0);
  or $ra, $zero, $a3                                 ## L:669  |    169 | RA = savedRA;
  lbu $t3, 0($t4)                                    ## L:667  |    170 | @Barrier("cb-stash") prim_size:u8 = load(color_dmem, 0x00); ## Barrier: 0x1
  mtc0 $a0, COP0_DMA_RAMADDR                         ## L:666  |    171 | @Barrier("tri-out", before) set_dma_addr_rdram(rdramCur);
  j LABEL_RDPQ_Triangle_Send_Async_000A              ## L:669  |    172 | RA = savedRA;
  addu $a0, $a0, $t3                                 ## L:668  |   *174 | rdramCur += prim_size;
RDPQ_Triangle_Send_End:
  ## hot-path cycles: 11
  LABEL_RDPQ_Triangle_Send_End_000B:
  mfc0 $t2, COP0_DMA_BUSY                            ## L:753  |      1 | dmaBusy = get_dma_busy();
  sh $zero, %lo(RDPQ_TRI_BUFF_OFFSET)($zero)         ## L:755  |      2 | store(ZERO:u16, RDPQ_TRI_BUFF_OFFSET);
  lbu $t0, %lo(RDPQ_SYNCFULL_ONGOING + 0)            ## L:756  |      3 | syncFull = load(RDPQ_SYNCFULL_ONGOING);
  bne $t2, $zero, LABEL_RDPQ_Triangle_Send_End_000B  ## L:757  |      4 | rdramCur = load(RDPQ_CURRENT);
  lw $t1, %lo(RDPQ_CURRENT + 0)                      ## L:757  |     *6 | rdramCur = load(RDPQ_CURRENT);
  LABEL_RDPQ_Triangle_Send_End_000C:
  bne $t0, $zero, JrRa                               ## L:764  |      7 | if(syncFull != 0)goto JrRa;
  sw $t1, %lo(RDPQ_CURRENT)($zero)                   ## L:763  |   **10 | store(rdramCur, RDPQ_CURRENT);
  jr $ra                                             ## L:767  |     11 | }
  mtc0 $t1, COP0_DP_END                              ## L:766  |    *13 | set_rdp_end(rdramCur);

.set at
#define zero $0
#define v0 $2
#define v1 $3
#define a0 $4
#define a1 $5
#define a2 $6
#define a3 $7
#define t0 $8
#define t1 $9
#define t2 $10
#define t3 $11
#define t4 $12
#define t5 $13
#define t6 $14
#define t7 $15
#define s0 $16
#define s1 $17
#define s2 $18
#define s3 $19
#define s4 $20
#define s5 $21
#define s6 $22
#define s7 $23
#define t8 $24
#define t9 $25
#define k0 $26
#define k1 $27
#define gp $28
#define sp $29
#define fp $30
#define ra $31
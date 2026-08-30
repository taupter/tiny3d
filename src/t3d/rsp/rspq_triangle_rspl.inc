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
  lbu $v0, %lo(FACE_CULLING + 0)                     ## L:69   |      ^ | u8<$v0> cullFlag = load(FACE_CULLING);
  srl $a2, $a1, 16                                   ## L:68   |      2 | u32<$a2> vert2 = vert1 >> 16;
RDPQ_Triangle_Send_Async:
  ## hot-path cycles: 142
  llv $v01, 0, 0, $a0                                ## L:95   |      ^ | vall1.xy = load(vtx1, 0x00).xy;
  mtc2 $a0, $v01.e3                                  ## L:93   |      2 | vall1.w = vtx1;
  llv $v02, 0, 0, $a1                                ## L:96   |      3 | vall2.xy = load(vtx2, 0x00).xy;
  vsubc $v27, $v00, $v30.v                           ## L:101  |      ^ | vec16<$v27> vmconst = VZERO - VSHIFT;
  llv $v03, 0, 0, $a2                                ## L:98   |      4 | vall3.xy = load(vtx3, 0x00).xy;
  vaddc $v12, $v00, $v30.e7                          ## L:115  |      ^ | vec16<$v12> vk1 = VZERO + 1;
  lhu $t4, 6($a1)                                    ## L:104  |      5 | u16<$t4> clip2 = load(vtx2, 0x06);
  addiu $at, $zero, 255                              ## L:145  |      6 | if(rejCodes != 0xFF)goto JrRa;
  vor $v06, $v00, $v01.e1                            ## L:107  |      ^ | vec16<$v06> vy1 = vall1.y;
  vor $v07, $v00, $v02.e1                            ## L:108  |      7 | vec16<$v07> vy2 = vall2.y;
  lhu $t3, 6($a0)                                    ## L:103  |      ^ | u16<$t3> clip1 = load(vtx1, 0x06);
  mtc2 $a1, $v02.e3                                  ## L:94   |      8 | vall2.w = vtx2;
  vor $v08, $v00, $v03.e1                            ## L:109  |      ^ | vec16<$v08> vy3 = vall3.y;
  lhu $t5, 6($a2)                                    ## L:105  |      9 | u16<$t5> clip3 = load(vtx3, 0x06);
  lhu $s7, %lo(RDPQ_TRI_BUFF_OFFSET + 0)             ## L:140  |     10 | u16<$s7> buffOffset = load(RDPQ_TRI_BUFF_OFFSET);
  or $t3, $t3, $t4                                   ## L:117  |     11 | clip1 |= clip2;
  vge $v09, $v06, $v07                               ## L:131  |      ^ | vytmp1 = max(vy1, vy2);
  vmrg $v04, $v01, $v02                              ## L:132  |     12 | valltmp1 = select(vall1, vall2);
  or $t3, $t3, $t5                                   ## L:118  |      ^ | clip1 |= clip3;
  vlt $v06, $v06, $v07                               ## L:133  |     13 | vy1 = min(vy1, vy2);
  mtc2 $a2, $v03.e3                                  ## L:97   |      ^ | vall3.w = vtx3;
  vmrg $v01, $v01, $v02                              ## L:134  |     14 | vall1 = select(vall1, vall2);
  andi $t6, $t3, 255                                 ## L:144  |      ^ | u32<$t6> rejCodes = clip1 & 0xFF;
  vxor $v28, $v28, $v28.v                            ## L:138  |     15 | vhmlupp ^= vhmlupp;
  bne $t6, $at, JrRa                                 ## L:145  |      ^ | if(rejCodes != 0xFF)goto JrRa;
  cfc2 $t0, $vcc                                     ## L:135  |     16 | did_swap_0 = get_vcc();
  vge $v10, $v06, $v08                               ## L:148  |     17 | vytmp2 = max(vy1, vy3);
  vmrg $v05, $v01, $v03                              ## L:149  |     18 | valltmp2 = select(vall1, vall3);
  addiu $s3, $s7, %lo(CLIP_BUFFER_TMP)               ## L:171  |      ^ | u16<$s3> outPtr = buffOffset + CLIP_BUFFER_TMP;
  srl $t2, $t3, 13                                   ## L:160  |     19 | u32<$t2> tileIdx = clip1 >> 13;
  vlt $v06, $v06, $v08                               ## L:150  |      ^ | vy1 = min(vy1, vy3);
  cfc2 $t1, $vcc                                     ## L:152  |     20 | did_swap_1 = get_vcc();
  vmrg $v01, $v01, $v03                              ## L:151  |      ^ | vall1 = select(vall1, vall3);
  vge $v08, $v09, $v10                               ## L:162  |     21 | vy3 = max(vytmp1, vytmp2);
  andi $t4, $t3, 7936                                ## L:170  |      ^ | u32<$t4> clipCodes = clip1 & 0x1F00;
  lw $a3, %lo(TRI_COMMAND + 0)                       ## L:154  |     22 | u32<$a3> triCmd = load(TRI_COMMAND);
  vmrg $v03, $v04, $v05                              ## L:163  |      ^ | vall3 = select(valltmp1, valltmp2);
  vlt $v07, $v09, $v10                               ## L:164  |     23 | vy2 = min(vytmp1, vytmp2);
  ssv $v06, 0, 6, $s3                                ## L:172  |      ^ | @Barrier("tri-out") store(vy1.x, outPtr, 6); ## Barrier: 0x1
  vmrg $v02, $v04, $v05                              ## L:165  |     24 | vall2 = select(valltmp1, valltmp2);
  bne $t4, $zero, RDPQ_Triangle_Clip                 ## L:176  |      ^ | if(clipCodes != 0)goto RDPQ_Triangle_Clip;
  cfc2 $t5, $vcc                                     ## L:166  |     25 | u32<$t5> did_swap_2 = get_vcc();
  vmov $v23.e0, $v01.e0                              ## L:197  |     26 | vx12F.x = vall1.x;
  mfc2 $a2, $v03.e3                                  ## L:193  |      ^ | vtx3 = vall3.w;
  xor $t0, $t0, $t1                                  ## L:179  |     27 | did_swap_0 ^= did_swap_1;
  xor $t0, $t0, $t5                                  ## L:180  |     28 | did_swap_0 ^= did_swap_2;
  vsubc $v05, $v02, $v01.v                           ## L:187  |      ^ | vec16<$v05> vm = vall2 - vall1;
  vsubc $v24, $v03, $v01.v                           ## L:185  |     29 | vec16<$v24> vhml = vall3 - vall1;
  ssv $v07, 0, 4, $s3                                ## L:213  |      ^ | @Barrier("tri-out") store(vy2.x, outPtr, 4); ## Barrier: 0x1
  ssv $v08, 0, 2, $s3                                ## L:214  |     30 | @Barrier("tri-out") store(vy3.x, outPtr, 2); ## Barrier: 0x1
  vsubc $v04, $v03, $v02.v                           ## L:188  |      ^ | vec16<$v04> vl = vall3 - vall2;
  mfc2 $a1, $v02.e3                                  ## L:192  |     31 | vtx2 = vall2.w;
  vmudn $v26, $v06, $v31.e1                          ## L:209  |      ^ | vec16<$v26> vfyF = vy1 << 14;
  vsubc $v29, $v00, $v05.e1                          ## L:206  |     32 | VTEMP = VZERO - vm.y;
  slv $v05, 0, 8, $s3                                ## L:232  |      ^ | @Barrier("tri-oA", before) @Barrier("tri-out") store(vm.xy, outPtr, 8); ## Barrier: 0x1
  andi $t0, $t0, 1                                   ## L:200  |     33 | did_swap_0 &= 1;
  vmudm $v25, $v06, $v31.e1                          ## L:210  |      ^ | vec16<$v25> vfyI = vy1 >> 2;
  vmov $v23.e2, $v02.e0                              ## L:198  |     34 | vx12F.z = vall2.x;
  mfc2 $a0, $v01.e3                                  ## L:191  |      ^ | vtx1 = vall1.w;
  vmudh $v19, $v05, $v24.e1                          ## L:221  |     35 | vnzF = vm:sint * vhml:sint.y;
  slv $v04, 0, 12, $s3                               ## L:233  |      ^ | @Barrier("tri-oA", before) @Barrier("tri-out") store(vl.xy, outPtr, 12); ## Barrier: 0x1
  vmadh $v19, $v24, $v29.v                           ## L:222  |     36 | vnzF = vhml:sint +* VTEMP:sint;
  xor $t1, $v0, $t0                                  ## L:201  |      ^ | u32<$t1> cullFlip = cull ^ did_swap_0;
  ldv $v24, 4, 8, $s3                                ## L:234  |     37 | @Barrier("tri-oA", after) @Barrier("tri-oA2", before) vhml.z = load(outPtr, 8).xyzw;
  vsar $v18, COP2_ACC_HI                             ## L:223  |      ^ | vnzI = get_acc_high();
  vsar $v19, COP2_ACC_MD                             ## L:224  |     38 | vnzF = get_acc_mid();
  lsv $v08, 4, 30, $a1                               ## L:248  |      ^ | vwF.z = load(vtx2, 0x1E).x;
  vsubc $v26, $v00, $v26.v                           ## L:242  |     39 | vfyF = VZERO - vfyF;
  xori $s7, $s7, 176                                 ## L:211  |      ^ | buffOffset ^= 176;
  vsub $v25, $v25, $v25.v                            ## L:243  |     40 | vfyI:sint = vfyI - vfyI;
  lsv $v08, 8, 30, $a2                               ## L:249  |      ^ | vwF.X = load(vtx3, 0x1E).x;
  mfc2 $t0, $v18.e0                                  ## L:252  |     41 | s32<$t0> nzSign = vnzI.x;
  vrcph $v20.e7, $v18.e0                             ## L:264  |      ^ | vslope.W = invert_half(vnz).x;
  addiu $t4, $s3, 32                                 ## L:238  |     42 | u32<$t4> color_dmem = outPtr + 0x20;
  vrcpl $v21.e7, $v19.e0                             ## L:264  |      ^ | vslope.W = invert_half(vnz).x;
  vrcph $v20.e7, $v00.e0                             ## L:264  |     43 | vslope.W = invert_half(vnz).x;
  lsv $v08, 0, 30, $a0                               ## L:247  |      ^ | vwF.x = load(vtx1, 0x1E).x;
  vrcp $v21.e0, $v24.e1                              ## L:268  |     44 | vslope.x = invert_half(vhml).y;
  TAG_CullFlipA: slt $t0, $t0, $zero                                ## L:269  |      ^ | @Tag("CullFlipA") asm_op("slt", nzSign, nzSign, ZERO);
  beq $t0, $t1, JrRa                                 ## L:270  |     45 | if(nzSign == cullFlip)goto JrRa;
  vrcph $v20.e0, $v24.e1                             ## L:268  |     46 | vslope.x = invert_half(vhml).y;
  lsv $v15, 8, 34, $a0                               ## L:331  |     47 | vinvwF.X = load(vtx1, 0x22).x;
  vrcp $v21.e2, $v05.e1                              ## L:285  |      ^ | vslope.z = invert_half(vm).y;
  lsv $v07, 0, 22, $a0                               ## L:289  |     48 | vwI.x = load(vtx1, 0x16).x;
  vrcph $v20.e2, $v05.e1                             ## L:285  |      ^ | vslope.z = invert_half(vm).y;
  vrcp $v21.e4, $v04.e1                              ## L:287  |     49 | vslope.X = invert_half(vl).y;
  lsv $v14, 8, 32, $a0                               ## L:357  |      ^ | vinvwI.X = load(vtx1, 0x20).x;
  lsv $v07, 4, 22, $a1                               ## L:290  |     50 | vwI.z = load(vtx2, 0x16).x;
  vrcph $v20.e4, $v04.e1                             ## L:287  |      ^ | vslope.X = invert_half(vl).y;
  lsv $v07, 8, 22, $a2                               ## L:291  |     51 | vwI.X = load(vtx3, 0x16).x;
  vmov $v28.e7, $v18.e0                              ## L:282  |      ^ | vhmlupp.W = vnzI.x;
  lsv $v14, 10, 32, $a1                              ## L:355  |     52 | vinvwI.Y = load(vtx2, 0x20).x;
  vmov $v24.e7, $v19.e0                              ## L:281  |      ^ | vhml.W = vnzF.x;
  vmudn $v01, $v21, $v27.e5                          ## L:311  |     53 | vec16<$v01> vslopem2F = vslopeF:ufract * vmconst:sint.Y;
  lsv $v15, 10, 34, $a1                              ## L:332  |      ^ | vinvwF.Y = load(vtx2, 0x22).x;
  llv $v06, 8, 12, $a2                               ## L:406  |     54 | vtex3.XY = load(vtx3, 0x0C).xy;
  vmadh $v02, $v20, $v27.e5                          ## L:312  |      ^ | vec16<$v02> vslopem2I = vslopeI:sint +* vmconst:sint.Y;
  lsv $v15, 12, 34, $a2                              ## L:323  |     55 | vinvwF.Z = load(vtx3, 0x22).x;
  vsubc $v29, $v08, $v08.e2                          ## L:315  |      ^ | VTEMP = vwF - vwF.z;
  vlt $v07, $v07, $v07.e2                            ## L:316  |     56 | vwI = min(vwI, vwI.z);
  luv $v09, 0, 8, $a0                                ## L:302  |      ^ | vec16<$v09> vattr1 = load_vec_u8(vtx1, 0x08);
  lsv $v14, 12, 32, $a2                              ## L:356  |     57 | vinvwI.Z = load(vtx3, 0x20).x;
  vmrg $v08, $v08, $v08.e2                           ## L:318  |      ^ | vwF = select(vwF, vwF.z);
  TAG_CullFlipB: xori $t0, $t0, 1                                   ## L:298  |     58 | @Tag("CullFlipB") nzSign ^= 1;
  vmudl $v03, $v21, $v24.q1                          ## L:329  |      ^ | asm_op("vmudl", vtmpF, vslopeF, vhml.yywwYYWW);
  sll $t0, $t0, 7                                    ## L:299  |     59 | nzSign <<= 7;
  vmadm $v03, $v20, $v24.q1                          ## L:330  |      ^ | asm_op("vmadm", vtmpF, vslopeI, vhml.yywwYYWW);
  or $a3, $a3, $t0                                   ## L:317  |     60 | triCmd |= nzSign;
  vmadn $v03, $v21, $v28.v                           ## L:338  |      ^ | vtmpF = vslopeF:ufract +* vhmlupp:sint;
  mfc2 $t1, $v24.e0                                  ## L:344  |     61 | u32<$t1> negHX = vhml.x;
  vmadh $v04, $v20, $v28.v                           ## L:339  |      ^ | vtmpI = vslopeI:sint +* vhmlupp:sint;
  vmadn $v03, $v12, $v27.e7                          ## L:345  |     62 | vtmpF = vk1:ufract +* vmconst:sint.W;
  andi $t6, $a3, 512                                 ## L:370  |      ^ | u32<$t6> z_dmem = triCmd & 0x200;
  vmadh $v04, $v12, $v27.e7                          ## L:346  |     63 | vtmpI = vk1:sint +* vmconst:sint.W;
  andi $t5, $a3, 1024                                ## L:335  |      ^ | u32<$t5> tex_dmem = triCmd & 0x400;
  vsubc $v29, $v08, $v08.e4                          ## L:351  |     64 | VTEMP = vwF - vwF.X;
  mfc2 $t0, $v24.e3                                  ## L:343  |      ^ | u32<$t0> negMY = vhml.w;
  vlt $v07, $v07, $v07.e4                            ## L:352  |     65 | vwI = min(vwI, vwI.X);
  subu $t1, $zero, $t1                               ## L:421  |      ^ | negHX = ZERO - negHX;
  vmrg $v08, $v08, $v08.e4                           ## L:353  |     66 | vwF = select(vwF, vwF.X);
  luv $v11, 0, 8, $a2                                ## L:301  |      ^ | vec16<$v11> vattr3 = load_vec_u8(vtx3, 0x08);
  vmudl $v29, $v03, $v01                             ## L:364  |     67 | asm_op("vmudl", VTEMP, vtmpF, vslopem2F);
  addiu $at, $zero, 207                              ## L:430  |      ^ | set_vcc(0xCF);
  luv $v10, 0, 8, $a1                                ## L:303  |     68 | vec16<$v10> vattr2 = load_vec_u8(vtx2, 0x08);
  vmadm $v29, $v04, $v01                             ## L:365  |      ^ | asm_op("vmadm", VTEMP, vtmpI, vslopem2F);
  vmadn $v19, $v03, $v02.v                           ## L:366  |     69 | vnzF = vtmpF:ufract +* vslopem2I:sint;
  srl $t5, $t5, 4                                    ## L:336  |      ^ | tex_dmem >>= 4;
  or $a3, $a3, $t2                                   ## L:377  |     70 | triCmd |= tileIdx;
  vmadh $v18, $v04, $v02.v                           ## L:367  |      ^ | vnzI = vtmpI:sint +* vslopem2I:sint;
  vmudl $v29, $v15, $v08.e0                          ## L:395  |     71 | asm_op("vmudl", VTEMP, vinvwF, vwF.x);
  ctc2 $at, $vcc                                     ## L:430  |      ^ | set_vcc(0xCF);
  vmadm $v29, $v14, $v08.e0                          ## L:396  |     72 | asm_op("vmadm", VTEMP, vinvwI, vwF.x);
  llv $v03, 8, 12, $a1                               ## L:405  |      ^ | vtex2.XY = load(vtx2, 0x0C).xy;
  vmadn $v15, $v15, $v07.e0                          ## L:397  |     73 | vinvwF = vinvwF:ufract +* vwI:sint.x;
  llv $v04, 8, 12, $a0                               ## L:404  |      ^ | vtex1.XY = load(vtx1, 0x0C).xy;
  vmudn $v29, $v19, $v24.v                           ## L:414  |     74 | VTEMP = vnzF:ufract * vhml:sint;
  addu $t5, $t5, $t4                                 ## L:372  |      ^ | tex_dmem += color_dmem;
  subu $t0, $zero, $t0                               ## L:420  |     75 | negMY = ZERO - negMY;
  vmadh $v29, $v18, $v24.v                           ## L:415  |      ^ | VTEMP = vnzI:sint +* vhml:sint;
  sh $a3, 0($s3)                                     ## L:379  |     76 | @Barrier("tri-out") store(triCmd:u16, outPtr, 0); ## Barrier: 0x1
  vsar $v20, COP2_ACC_HI                             ## L:417  |      ^ | vslopeI = get_acc_high();
  vsar $v21, COP2_ACC_MD                             ## L:416  |     77 | vslopeF = get_acc_mid();
  mtc2 $t1, $v24.e0                                  ## L:423  |      ^ | vhml.x = negHX;
  mtc2 $t0, $v24.e3                                  ## L:422  |     78 | vhml.w = negMY;
  vsubc $v15, $v15, $v30.e7                          ## L:427  |      ^ | vinvwF -= 1;
  lbu $t0, %lo(RDPQ_SYNCFULL_ONGOING + 0)            ## L:477  |     79 | u8<$t0> syncFull = load(RDPQ_SYNCFULL_ONGOING);
  vmudl $v09, $v09, $v31.e6                          ## L:433  |      ^ | vattr1 >>>= 7;
  lsv $v09, 14, 4, $a0                               ## L:438  |     80 | vattr1.W = load(vtx1, 0x04).x;
  vmudl $v11, $v11, $v31.e6                          ## L:435  |      ^ | vattr3 >>>= 7;
  srl $t6, $t6, 3                                    ## L:371  |     81 | z_dmem >>= 3;
  vmudl $v10, $v10, $v31.e6                          ## L:434  |      ^ | vattr2 >>>= 7;
  vmudl $v15, $v15, $v31.e0                          ## L:444  |     82 | vinvwF >>>= 1;
  lsv $v11, 14, 4, $a2                               ## L:440  |      ^ | vattr3.W = load(vtx3, 0x04).x;
  vmudl $v29, $v21, $v26.e4                          ## L:454  |     83 | vec32<$v16> vedges = vslope * vfy.X;
  mfc0 $t1, COP0_DMA_BUSY                            ## L:458  |      ^ | u32<$t1> dmaBusy = get_dma_busy();
  vmadm $v29, $v20, $v26.e4                          ## L:454  |     84 | vec32<$v16> vedges = vslope * vfy.X;
  andi $t3, $a3, 256                                 ## L:456  |      ^ | u32<$t3> prim_size = triCmd & 0x100;
  vmadn $v17, $v21, $v25.e4                          ## L:454  |     85 | vec32<$v16> vedges = vslope * vfy.X;
  lsv $v10, 14, 4, $a1                               ## L:439  |      ^ | vattr2.W = load(vtx2, 0x04).x;
  vmadh $v16, $v20, $v25.e4                          ## L:454  |     86 | vec32<$v16> vedges = vslope * vfy.X;
  sdv $v15, 8, 16, $s3                               ## L:461  |      ^ | @Barrier("tri-oB", before) @Barrier("tri-out") store(vinvwF.XYZW, outPtr, 16); ## Barrier: 0x1
  vmulf $v03, $v03, $v15.h1                          ## L:468  |     87 | vtex2:sfract *= vinvwF.yyyyYYYY;
  lsv $v11, 12, 20, $s3                              ## L:464  |      ^ | @Barrier("tri-oB", after) @Barrier("tri-oB2", before) vattr3.Z = load(outPtr, 20).x;
  lsv $v10, 12, 18, $s3                              ## L:463  |     88 | @Barrier("tri-oB", after) @Barrier("tri-oB2", before) vattr2.Z = load(outPtr, 18).x;
  vmulf $v06, $v06, $v15.h2                          ## L:469  |      ^ | vtex3:sfract *= vinvwF.zzzzZZZZ;
  vmulf $v04, $v04, $v15.h0                          ## L:467  |     89 | vtex1:sfract *= vinvwF.xxxxXXXX;
  lsv $v09, 12, 16, $s3                              ## L:462  |      ^ | @Barrier("tri-oB", after) @Barrier("tri-oB2", before) vattr1.Z = load(outPtr, 16).x;
  vmudm $v22, $v23, $v31.e1                          ## L:474  |     90 | vec16<$v22> vx12I = vx12F >> 2;
  bne $t1, $zero, LABEL_RDPQ_Triangle_Send_Async_0006## L:479  |      ^ | @Unlikely if(dmaBusy != 0)
  vmudn $v23, $v23, $v31.e1                          ## L:475  |     91 | vx12F <<= 14;
  LABEL_RDPQ_Triangle_Send_Async_0005:
  vmrg $v11, $v11, $v06                              ## L:502  |     92 | vattr3 = select(vattr3, vtex3);
  ssv $v20, 8, 12, $s3                               ## L:508  |      ^ | @Barrier("tri-oA2", after) @Barrier("tri-out") store(vslope:sint.X, outPtr, 12); ## Barrier: 0x1
  lw $a0, %lo(RDPQ_CURRENT + 0)                      ## L:513  |     93 | u32<$a0> rdramCur = load(RDPQ_CURRENT);
  vmrg $v09, $v09, $v04                              ## L:500  |      ^ | vattr1 = select(vattr1, vtex1);
  vmrg $v10, $v10, $v03                              ## L:501  |     94 | vattr2 = select(vattr2, vtex2);
  addu $t6, $t6, $t5                                 ## L:495  |      ^ | z_dmem += tex_dmem;
  vaddc $v17, $v17, $v23.e0                          ## L:518  |     95 | vedges += vx12.x;
  srl $t3, $t3, 4                                    ## L:496  |      ^ | prim_size >>= 4;
  vadd $v16, $v16, $v22.e0                           ## L:518  |     96 | vedges += vx12.x;
  ssv $v22, 4, 8, $s3                                ## L:509  |      ^ | @Barrier("tri-oA2", after) @Barrier("tri-out") store(vx12I.z, outPtr, 8); ## Barrier: 0x1
  bne $t0, $zero, LABEL_RDPQ_Triangle_Send_Async_0008## L:526  |     97 | if(syncFull == 0) {
  ssv $v21, 8, 14, $s3                               ## L:510  |     98 | @Barrier("tri-oA2", after) @Barrier("tri-out") store(vslope:ufract.X, outPtr, 14); ## Barrier: 0x1
  mtc0 $a0, COP0_DP_END                              ## L:527  |     99 | set_rdp_end(rdramCur);
  LABEL_RDPQ_Triangle_Send_Async_0008:
  vsubc $v29, $v00, $v00                             ## L:539  |      ^ | clear_vcc();
  ssv $v23, 4, 10, $s3                               ## L:535  |    100 | @Barrier("tri-oA2", after) @Barrier("tri-out") store(vx12F.z, outPtr, 10); ## Barrier: 0x1
  vsub $v12, $v10, $v09.v                            ## L:549  |      ^ | vec16<$v12> vma:sint = vattr2 - vattr1;
  vsub $v13, $v11, $v09.v                            ## L:550  |    101 | vec16<$v13> vha:sint = vattr3 - vattr1;
  addu $t3, $t3, $t6                                 ## L:603  |      ^ | prim_size += z_dmem;
  ssv $v20, 4, 28, $s3                               ## L:571  |    102 | @Barrier("tri-out") store(vslope:sint.z, outPtr, 28); ## Barrier: 0x1
  vmudn $v19, $v19, $v30.e5                          ## L:557  |      ^ | vnz = vnz * 4;
  ssv $v21, 4, 30, $s3                               ## L:572  |    103 | @Barrier("tri-out") store(vslope:ufract.z, outPtr, 30); ## Barrier: 0x1
  vmadh $v18, $v18, $v30.e5                          ## L:557  |      ^ | vnz = vnz * 4;
  vmudh $v29, $v12, $v24.e1                          ## L:564  |    104 | VTEMP = vma:sint * vhml:sint.y;
  sh $s7, %lo(RDPQ_TRI_BUFF_OFFSET)($zero)           ## L:608  |      ^ | store(buffOffset, RDPQ_TRI_BUFF_OFFSET);
  vmadh $v29, $v13, $v24.e3                          ## L:565  |    105 | VTEMP = vha:sint +* vhml:sint.w;
  ssv $v16, 4, 24, $s3                               ## L:574  |      ^ | @Barrier("tri-out") store(vedges:sint.z, outPtr, 24); ## Barrier: 0x1
  vsar $v03, COP2_ACC_HI                             ## L:567  |    106 | vdxI = get_acc_high();
  ssv $v17, 4, 26, $s3                               ## L:575  |      ^ | @Barrier("tri-out") store(vedges:ufract.z, outPtr, 26); ## Barrier: 0x1
  vsar $v04, COP2_ACC_MD                             ## L:566  |    107 | vdxF = get_acc_mid();
  ssv $v16, 0, 16, $s3                               ## L:576  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out") store(vedges:sint.x, outPtr, 16); ## Barrier: 0x1
  ssv $v17, 0, 18, $s3                               ## L:577  |    108 | @Barrier("tri-oB2", after) @Barrier("tri-out") store(vedges:ufract.x, outPtr, 18); ## Barrier: 0x1
  vmudh $v29, $v13, $v24.e2                          ## L:583  |      ^ | VTEMP = vha:sint * vhml:sint.z;
  vmadh $v29, $v12, $v24.e0                          ## L:584  |    109 | VTEMP = vma:sint +* vhml:sint.x;
  ssv $v20, 0, 20, $s3                               ## L:591  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out") store(vslope:sint.x, outPtr, 20); ## Barrier: 0x1
  lw $a2, %lo(RDPQ_SENTINEL + 0)                     ## L:559  |    110 | u32<$a2> rdramSentinel = load(RDPQ_SENTINEL);
  vsar $v08, COP2_ACC_MD                             ## L:585  |      ^ | vdyF = get_acc_mid();
  vsar $v07, COP2_ACC_HI                             ## L:586  |    111 | vdyI = get_acc_high();
  ssv $v21, 0, 22, $s3                               ## L:592  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out") store(vslope:ufract.x, outPtr, 22); ## Barrier: 0x1
  vmudl $v29, $v04, $v19.e7                          ## L:597  |    112 | vdx *= vnz.W;
  subu $t3, $t3, $s3                                 ## L:604  |      ^ | prim_size -= outPtr;
  vmadm $v29, $v03, $v19.e7                          ## L:597  |    113 | vdx *= vnz.W;
  mtc0 $s3, COP0_DMA_SPADDR                          ## L:594  |      ^ | @Barrier("tri-out") set_dma_addr_rsp(outPtr); ## Barrier: 0x1
  mtc0 $a0, COP0_DMA_RAMADDR                         ## L:606  |    114 | @Barrier("tri-out") set_dma_addr_rdram(rdramCur); ## Barrier: 0x1
  vmadn $v04, $v04, $v18.e7                          ## L:597  |      ^ | vdx *= vnz.W;
  addu $a0, $a0, $t3                                 ## L:612  |    115 | rdramCur += prim_size;
  vmadh $v03, $v03, $v18.e7                          ## L:597  |      ^ | vdx *= vnz.W;
  vmudl $v29, $v08, $v19.e7                          ## L:599  |    116 | vdy *= vnz.W;
  sltu $at, $a2, $a0                                 ## L:634  |      ^ | @Unlikely if(rdramSentinel < rdramCur)
  vmadm $v29, $v07, $v19.e7                          ## L:599  |    117 | vdy *= vnz.W;
  addu $s3, $s3, $t3                                 ## L:605  |      ^ | outPtr += prim_size;
  sdv $v04, 0, 24, $t4                               ## L:615  |    118 | @Barrier("tri-out") store(vdx:ufract.xyzw, color_dmem, 0x18); ## Barrier: 0x1
  vmadn $v08, $v08, $v18.e7                          ## L:599  |      ^ | vdy *= vnz.W;
  vmadh $v07, $v07, $v18.e7                          ## L:599  |    119 | vdy *= vnz.W;
  sdv $v04, 8, 24, $t5                               ## L:616  |      ^ | @Barrier("tri-out") store(vdx:ufract.XYZW, tex_dmem, 0x18); ## Barrier: 0x1
  vmadl $v29, $v04, $v21.e0                          ## L:621  |    120 | vec32<$v05> vde = vdx +* vslope.x;
  sdv $v03, 0, 8, $t4                                ## L:617  |      ^ | @Barrier("tri-out") store(vdx:sint.xyzw, color_dmem, 0x08); ## Barrier: 0x1
  vmadm $v29, $v03, $v21.e0                          ## L:621  |    121 | vec32<$v05> vde = vdx +* vslope.x;
  sdv $v03, 8, 8, $t5                                ## L:618  |      ^ | @Barrier("tri-out") store(vdx:sint.XYZW, tex_dmem, 0x08); ## Barrier: 0x1
  vmadn $v06, $v04, $v20.e0                          ## L:621  |    122 | vec32<$v05> vde = vdx +* vslope.x;
  sdv $v08, 0, 56, $t4                               ## L:625  |      ^ | @Barrier("tri-out") store(vdy:ufract.xyzw, color_dmem, 0x38); ## Barrier: 0x1
  vmadh $v05, $v03, $v20.e0                          ## L:621  |    123 | vec32<$v05> vde = vdx +* vslope.x;
  bne $at, $zero, LABEL_RDPQ_Triangle_Send_Async_0009## L:634  |      ^ | @Unlikely if(rdramSentinel < rdramCur)
  sdv $v08, 8, 56, $t5                               ## L:628  |    124 | @Barrier("tri-out") store(vdy:ufract.XYZW, tex_dmem, 0x38); ## Barrier: 0x1
  LABEL_RDPQ_Triangle_Send_Async_000A:
  vmudh $v29, $v09, $v30.e7                          ## L:665  |    125 | VTEMP = vattr1:sint * 1;
  sdv $v07, 0, 40, $t4                               ## L:669  |      ^ | @Barrier("tri-out") store(vdy:sint.xyzw, color_dmem, 0x28); ## Barrier: 0x1
  sdv $v07, 8, 40, $t5                               ## L:670  |    126 | @Barrier("tri-out") store(vdy:sint.XYZW, tex_dmem, 0x28); ## Barrier: 0x1
  vmadl $v29, $v06, $v26.e4                          ## L:666  |      ^ | vec32<$v01> vfinal = vde +* vfy.X;
  vmadm $v29, $v05, $v26.e4                          ## L:666  |    127 | vec32<$v01> vfinal = vde +* vfy.X;
  sdv $v06, 0, 48, $t4                               ## L:672  |      ^ | @Barrier("tri-out") store(vde:ufract.xyzw, color_dmem, 0x30); ## Barrier: 0x1
  vmadn $v02, $v06, $v25.e4                          ## L:666  |    128 | vec32<$v01> vfinal = vde +* vfy.X;
  sdv $v06, 8, 48, $t5                               ## L:673  |      ^ | @Barrier("tri-out") store(vde:ufract.XYZW, tex_dmem, 0x30); ## Barrier: 0x1
  sdv $v05, 0, 32, $t4                               ## L:674  |    129 | @Barrier("tri-out") store(vde:sint.xyzw, color_dmem, 0x20); ## Barrier: 0x1
  vmadh $v01, $v05, $v25.e4                          ## L:666  |      ^ | vec32<$v01> vfinal = vde +* vfy.X;
  sw $a0, %lo(RDPQ_CURRENT)($zero)                   ## L:678  |    130 | store(rdramCur, RDPQ_CURRENT);
  vmov $v10.e3, $v04.e7                              ## L:702  |      ^ | vzout.w = vdxF.W;
  vmov $v06.e6, $v05.e7                              ## L:699  |    131 | vdeF.Z = vdeI.W;
  sdv $v05, 8, 32, $t5                               ## L:675  |      ^ | @Barrier("tri-out") store(vde:sint.XYZW, tex_dmem, 0x20); ## Barrier: 0x1
  sdv $v02, 0, 16, $t4                               ## L:706  |    132 | @Barrier("tri-out") store(vfinalF.xyzw, color_dmem, 0x10); ## Barrier: 0x1
  vmov $v08.e6, $v07.e7                              ## L:698  |      ^ | vdyF.Z = vdyI.W;
  sdv $v02, 8, 16, $t5                               ## L:707  |    133 | @Barrier("tri-out") store(vfinalF.XYZW, tex_dmem, 0x10); ## Barrier: 0x1
  vmov $v10.e2, $v03.e7                              ## L:703  |      ^ | vzout.z = vdxI.W;
  sdv $v01, 0, 0, $t4                                ## L:708  |    134 | @Barrier("tri-out") store(vfinalI.xyzw, color_dmem, 0x00); ## Barrier: 0x1
  vmov $v10.e1, $v02.e7                              ## L:700  |      ^ | vzout.y = vfinalF.W;
  sdv $v01, 8, 0, $t5                                ## L:709  |    135 | @Barrier("tri-out") store(vfinalI.XYZW, tex_dmem, 0x00); ## Barrier: 0x1
  vmov $v10.e0, $v01.e7                              ## L:701  |      ^ | vzout.x = vfinalI.W;
  slv $v06, 12, 8, $t6                               ## L:712  |    136 | @Barrier("tri-out") store(vdeF.ZW, z_dmem, 0x08); ## Barrier: 0x1
  slv $v08, 12, 12, $t6                              ## L:713  |    137 | @Barrier("tri-out") store(vdyF.ZW, z_dmem, 0x0C); ## Barrier: 0x1
  addiu $t3, $t3, 65535                              ## L:714  |    138 | prim_size -= 1;
  sdv $v10, 0, 0, $t6                                ## L:715  |    139 | @Barrier("tri-out") store(vzout.xyzw, z_dmem, 0x00); ## Barrier: 0x1
  jr $ra                                             ## L:727  |    140 | }
  mtc0 $t3, COP0_DMA_WRITE                           ## L:726  |   *142 | @Barrier("tri-out", after) set_dma_write(prim_size);
  LABEL_RDPQ_Triangle_Send_Async_0006:
  mfc0 $t1, COP0_DMA_BUSY                            ## L:487  |      1 | dmaBusy = get_dma_busy();
  bne $t1, $zero, LABEL_RDPQ_Triangle_Send_Async_0006## L:487  |    **4 | dmaBusy = get_dma_busy();
  nop                                                ## L:431  |     *6 | 
  LABEL_RDPQ_Triangle_Send_Async_0007:
  j LABEL_RDPQ_Triangle_Send_Async_0005              ## L:487  |      7 | dmaBusy = get_dma_busy();
  nop                                                ## L:431  |     *9 | 
  LABEL_RDPQ_Triangle_Send_Async_0009:
  lw $a0, %lo(RDPQ_DYNAMIC_BUFFERS + 4)              ## L:637  |     10 | rdramCur = load(RDPQ_DYNAMIC_BUFFERS, 4);
  or $a3, $zero, $ra                                 ## L:636  |     11 | u32<$a3> savedRA = RA;
  lw $t1, %lo(RDPQ_DYNAMIC_BUFFERS + 0)              ## L:638  |     12 | u32<$t1> otherBuff = load(RDPQ_DYNAMIC_BUFFERS, 0);
  sb $t3, 0($t4)                                     ## L:648  |     13 | @Barrier("cb-stash") store(prim_size:u8, color_dmem, 0x00); ## Barrier: 0x2
  or $a1, $zero, $a0                                 ## L:651  |     14 | u32<$a1> rdramCurCopy = rdramCur;
  lui $at, 0x01                                      ## L:650  |     15 | rdramSentinel = rdramCur + 65536;
  sw $a0, %lo(0 + RDPQ_DYNAMIC_BUFFERS)($zero)       ## L:639  |     16 | store(rdramCur, RDPQ_DYNAMIC_BUFFERS, 0);
  addu $a2, $a0, $at                                 ## L:650  |     17 | rdramSentinel = rdramCur + 65536;
  jal RSPQCmd_RdpSetBuffer                           ## L:652  |     18 | RSPQCmd_RdpSetBuffer(rdramCur, rdramCurCopy, rdramSentinel); ## Args: $a0, $a1, $a2
  sw $t1, %lo(4 + RDPQ_DYNAMIC_BUFFERS)($zero)       ## L:640  |    *20 | store(otherBuff, RDPQ_DYNAMIC_BUFFERS, 4);
  or $ra, $zero, $a3                                 ## L:660  |     21 | RA = savedRA;
  lbu $t3, 0($t4)                                    ## L:658  |     22 | @Barrier("cb-stash") prim_size:u8 = load(color_dmem, 0x00); ## Barrier: 0x2
  mtc0 $a0, COP0_DMA_RAMADDR                         ## L:657  |     23 | @Barrier("tri-out") set_dma_addr_rdram(rdramCur); ## Barrier: 0x1
  j LABEL_RDPQ_Triangle_Send_Async_000A              ## L:660  |     24 | RA = savedRA;
  addu $a0, $a0, $t3                                 ## L:659  |    *26 | rdramCur += prim_size;
RDPQ_Triangle_Send_End:
  ## hot-path cycles: 11
  LABEL_RDPQ_Triangle_Send_End_000B:
  mfc0 $t2, COP0_DMA_BUSY                            ## L:760  |      1 | dmaBusy = get_dma_busy();
  lw $t1, %lo(RDPQ_CURRENT + 0)                      ## L:764  |      2 | rdramCur = load(RDPQ_CURRENT);
  lbu $t0, %lo(RDPQ_SYNCFULL_ONGOING + 0)            ## L:763  |      3 | syncFull = load(RDPQ_SYNCFULL_ONGOING);
  bne $t2, $zero, LABEL_RDPQ_Triangle_Send_End_000B  ## L:764  |      4 | rdramCur = load(RDPQ_CURRENT);
  sh $zero, %lo(RDPQ_TRI_BUFF_OFFSET)($zero)         ## L:762  |     *6 | store(ZERO:u16, RDPQ_TRI_BUFF_OFFSET);
  LABEL_RDPQ_Triangle_Send_End_000C:
  bne $t0, $zero, JrRa                               ## L:771  |      7 | if(syncFull != 0)goto JrRa;
  sw $t1, %lo(RDPQ_CURRENT)($zero)                   ## L:770  |      8 | store(rdramCur, RDPQ_CURRENT);
  jr $ra                                             ## L:774  |      9 | }
  mtc0 $t1, COP0_DP_END                              ## L:773  |    *11 | set_rdp_end(rdramCur);

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
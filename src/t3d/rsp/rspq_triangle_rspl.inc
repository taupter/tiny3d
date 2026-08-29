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
  ## hot-path cycles: 143
  mtc2 $a1, $v02.e3                                  ## L:105  |      ^ | vall2.w = vtx2;
  mtc2 $a0, $v01.e3                                  ## L:104  |      2 | vall1.w = vtx1;
  llv $v01, 0, 0, $a0                                ## L:106  |      3 | vall1.xy = load(vtx1, 0x00).xy;
  llv $v02, 0, 0, $a1                                ## L:107  |      4 | vall2.xy = load(vtx2, 0x00).xy;
  AAA:
  lhu $t3, 6($a0)                                    ## L:118  |      5 | u16<$t3> clip1 = load(vtx1, 0x06);
  vsubc $v27, $v00, $v30.v                           ## L:116  |      ^ | vec16<$v27> vmconst = VZERO - VSHIFT;
  vaddc $v12, $v00, $v30.e7                          ## L:130  |      6 | vec16<$v12> vk1 = VZERO + 1;
  llv $v03, 0, 0, $a2                                ## L:113  |      ^ | vall3.xy = load(vtx3, 0x00).xy;
  vor $v06, $v00, $v01.e1                            ## L:122  |      7 | vec16<$v06> vy1 = vall1.y;
  lhu $t4, 6($a1)                                    ## L:119  |      ^ | u16<$t4> clip2 = load(vtx2, 0x06);
  lhu $t5, 6($a2)                                    ## L:120  |      8 | u16<$t5> clip3 = load(vtx3, 0x06);
  vor $v07, $v00, $v02.e1                            ## L:123  |      ^ | vec16<$v07> vy2 = vall2.y;
  addiu $at, $zero, 255                              ## L:160  |      9 | if(rejCodes != 0xFF)goto JrRa;
  lhu $s7, %lo(RDPQ_TRI_BUFF_OFFSET + 0)             ## L:155  |     10 | u16<$s7> buffOffset = load(RDPQ_TRI_BUFF_OFFSET);
  vor $v08, $v00, $v03.e1                            ## L:124  |      ^ | vec16<$v08> vy3 = vall3.y;
  mtc2 $a2, $v03.e3                                  ## L:110  |     11 | vall3.w = vtx3;
  or $t3, $t3, $t4                                   ## L:132  |     12 | clip1 |= clip2;
  vge $v09, $v06, $v07                               ## L:146  |      ^ | vytmp1 = max(vy1, vy2);
  vmrg $v04, $v01, $v02                              ## L:147  |     13 | valltmp1 = select(vall1, vall2);
  or $t3, $t3, $t5                                   ## L:133  |      ^ | clip1 |= clip3;
  vlt $v06, $v06, $v07                               ## L:148  |     14 | vy1 = min(vy1, vy2);
  andi $t6, $t3, 255                                 ## L:159  |      ^ | u32<$t6> rejCodes = clip1 & 0xFF;
  cfc2 $t0, $vcc                                     ## L:150  |     15 | did_swap_0 = get_vcc();
  vmrg $v01, $v01, $v02                              ## L:149  |      ^ | vall1 = select(vall1, vall2);
  bne $t6, $at, JrRa                                 ## L:160  |     16 | if(rejCodes != 0xFF)goto JrRa;
  vxor $v28, $v28, $v28.v                            ## L:153  |     17 | vhmlupp ^= vhmlupp;
  vge $v10, $v06, $v08                               ## L:163  |     18 | vytmp2 = max(vy1, vy3);
  andi $t4, $t3, 7936                                ## L:185  |      ^ | u32<$t4> clipCodes = clip1 & 0x1F00;
  vmrg $v05, $v01, $v03                              ## L:164  |     19 | valltmp2 = select(vall1, vall3);
  vlt $v06, $v06, $v08                               ## L:165  |     20 | vy1 = min(vy1, vy3);
  lw $a3, %lo(TRI_COMMAND + 0)                       ## L:169  |      ^ | u32<$a3> triCmd = load(TRI_COMMAND);
  cfc2 $t1, $vcc                                     ## L:167  |     21 | did_swap_1 = get_vcc();
  vmrg $v01, $v01, $v03                              ## L:166  |      ^ | vall1 = select(vall1, vall3);
  vge $v08, $v09, $v10                               ## L:177  |     22 | vy3 = max(vytmp1, vytmp2);
  vmrg $v03, $v04, $v05                              ## L:178  |     23 | vall3 = select(valltmp1, valltmp2);
  addiu $s3, $s7, %lo(CLIP_BUFFER_TMP)               ## L:186  |      ^ | u16<$s3> outPtr = buffOffset + CLIP_BUFFER_TMP;
  vlt $v07, $v09, $v10                               ## L:179  |     24 | vy2 = min(vytmp1, vytmp2);
  ssv $v06, 0, 6, $s3                                ## L:187  |      ^ | @Barrier("tri-out") store(vy1.x, outPtr, 6); ## Barrier: 0x1
  cfc2 $t5, $vcc                                     ## L:181  |     25 | u32<$t5> did_swap_2 = get_vcc();
  vmrg $v02, $v04, $v05                              ## L:180  |      ^ | vall2 = select(valltmp1, valltmp2);
  bne $t4, $zero, RDPQ_Triangle_Clip                 ## L:191  |     26 | if(clipCodes != 0)goto RDPQ_Triangle_Clip;
  srl $t2, $t3, 13                                   ## L:175  |     27 | u32<$t2> tileIdx = clip1 >> 13;
  vsubc $v24, $v03, $v01.v                           ## L:200  |     28 | vec16<$v24> vhml = vall3 - vall1;
  ssv $v07, 0, 4, $s3                                ## L:228  |      ^ | @Barrier("tri-out") store(vy2.x, outPtr, 4); ## Barrier: 0x1
  vsubc $v05, $v02, $v01.v                           ## L:202  |     29 | vec16<$v05> vm = vall2 - vall1;
  mfc2 $a0, $v01.e3                                  ## L:206  |      ^ | vtx1 = vall1.w;
  vsubc $v04, $v03, $v02.v                           ## L:203  |     30 | vec16<$v04> vl = vall3 - vall2;
  mfc2 $a1, $v02.e3                                  ## L:207  |      ^ | vtx2 = vall2.w;
  xor $t0, $t0, $t1                                  ## L:194  |     31 | did_swap_0 ^= did_swap_1;
  vmudm $v25, $v06, $v31.e1                          ## L:225  |      ^ | vec16<$v25> vfyI = vy1 >> 2;
  vmov $v23.e0, $v01.e0                              ## L:212  |     32 | vx12F.x = vall1.x;
  xor $t0, $t0, $t5                                  ## L:195  |      ^ | did_swap_0 ^= did_swap_2;
  vsubc $v29, $v00, $v05.e1                          ## L:221  |     33 | VTEMP = VZERO - vm.y;
  andi $t0, $t0, 1                                   ## L:215  |      ^ | did_swap_0 &= 1;
  vmov $v23.e2, $v02.e0                              ## L:213  |     34 | vx12F.z = vall2.x;
  ssv $v08, 0, 2, $s3                                ## L:229  |      ^ | @Barrier("tri-out") store(vy3.x, outPtr, 2); ## Barrier: 0x1
  vmudn $v26, $v06, $v31.e1                          ## L:224  |     35 | vec16<$v26> vfyF = vy1 << 14;
  slv $v05, 0, 8, $s3                                ## L:248  |      ^ | @Barrier("tri-oA", before) @Barrier("tri-out") store(vm.xy, outPtr, 8); ## Barrier: 0x1
  xor $t1, $v0, $t0                                  ## L:216  |     36 | u32<$t1> cullFlip = cull ^ did_swap_0;
  vmudh $v19, $v05, $v24.e1                          ## L:236  |      ^ | vnzF = vm:sint * vhml:sint.y;
  vmadh $v19, $v24, $v29.v                           ## L:237  |     37 | vnzF = vhml:sint +* VTEMP:sint;
  mfc2 $a2, $v03.e3                                  ## L:208  |      ^ | vtx3 = vall3.w;
  vsar $v18, COP2_ACC_HI                             ## L:238  |     38 | vnzI = get_acc_high();
  addiu $t4, $s3, 32                                 ## L:254  |      ^ | u32<$t4> color_dmem = outPtr + 0x20;
  xori $s7, $s7, 176                                 ## L:226  |     39 | buffOffset ^= 176;
  vsar $v19, COP2_ACC_MD                             ## L:239  |      ^ | vnzF = get_acc_mid();
  slv $v04, 0, 12, $s3                               ## L:249  |     40 | @Barrier("tri-oA", before) @Barrier("tri-out") store(vl.xy, outPtr, 12); ## Barrier: 0x1
  vsubc $v26, $v00, $v26.v                           ## L:258  |      ^ | vfyF = VZERO - vfyF;
  ldv $v24, 4, 8, $s3                                ## L:250  |     41 | @Barrier("tri-oA", after) @Barrier("tri-oA2", before) vhml.z = load(outPtr, 8).xyzw;
  vsub $v25, $v25, $v25.v                            ## L:259  |      ^ | vfyI:sint = vfyI - vfyI;
  vrcph $v20.e7, $v18.e0                             ## L:280  |     42 | vslope.W = invert_half(vnz).x;
  mfc2 $t0, $v18.e0                                  ## L:268  |      ^ | s32<$t0> nzSign = vnzI.x;
  lsv $v08, 0, 30, $a0                               ## L:263  |     43 | vwF.x = load(vtx1, 0x1E).x;
  vrcpl $v21.e7, $v19.e0                             ## L:280  |      ^ | vslope.W = invert_half(vnz).x;
  lsv $v08, 4, 30, $a1                               ## L:264  |     44 | vwF.z = load(vtx2, 0x1E).x;
  vrcph $v20.e7, $v00.e0                             ## L:280  |      ^ | vslope.W = invert_half(vnz).x;
  TAG_CullFlipA: slt $t0, $t0, $zero                                ## L:285  |     45 | @Tag("CullFlipA") asm_op("slt", nzSign, nzSign, ZERO);
  vrcp $v21.e0, $v24.e1                              ## L:284  |      ^ | vslope.x = invert_half(vhml).y;
  vrcph $v20.e0, $v24.e1                             ## L:284  |     46 | vslope.x = invert_half(vhml).y;
  beq $t0, $t1, JrRa                                 ## L:286  |      ^ | if(nzSign == cullFlip)goto JrRa;
  lsv $v08, 8, 30, $a2                               ## L:265  |     47 | vwF.X = load(vtx3, 0x1E).x;
  TAG_CullFlipB: xori $t0, $t0, 1                                   ## L:314  |     48 | @Tag("CullFlipB") nzSign ^= 1;
  vrcp $v21.e2, $v05.e1                              ## L:301  |      ^ | vslope.z = invert_half(vm).y;
  vrcph $v20.e2, $v05.e1                             ## L:301  |     49 | vslope.z = invert_half(vm).y;
  lsv $v07, 8, 22, $a2                               ## L:307  |      ^ | vwI.X = load(vtx3, 0x16).x;
  lsv $v15, 10, 34, $a1                              ## L:348  |     50 | vinvwF.Y = load(vtx2, 0x22).x;
  vrcp $v21.e4, $v04.e1                              ## L:303  |      ^ | vslope.X = invert_half(vl).y;
  lsv $v07, 4, 22, $a1                               ## L:306  |     51 | vwI.z = load(vtx2, 0x16).x;
  vrcph $v20.e4, $v04.e1                             ## L:303  |      ^ | vslope.X = invert_half(vl).y;
  lsv $v07, 0, 22, $a0                               ## L:305  |     52 | vwI.x = load(vtx1, 0x16).x;
  vmov $v28.e7, $v18.e0                              ## L:298  |      ^ | vhmlupp.W = vnzI.x;
  vmov $v24.e7, $v19.e0                              ## L:297  |     53 | vhml.W = vnzF.x;
  sll $t0, $t0, 7                                    ## L:315  |      ^ | nzSign <<= 7;
  vmudn $v01, $v21, $v27.e5                          ## L:327  |     54 | vec16<$v01> vslopem2F = vslopeF:ufract * vmconst:sint.Y;
  or $a3, $a3, $t0                                   ## L:333  |      ^ | triCmd |= nzSign;
  vmadh $v02, $v20, $v27.e5                          ## L:328  |     55 | vec16<$v02> vslopem2I = vslopeI:sint +* vmconst:sint.Y;
  luv $v10, 0, 8, $a1                                ## L:319  |      ^ | vec16<$v10> vattr2 = load_vec_u8(vtx2, 0x08);
  vsubc $v29, $v08, $v08.e2                          ## L:331  |     56 | VTEMP = vwF - vwF.z;
  lsv $v14, 8, 32, $a0                               ## L:373  |      ^ | vinvwI.X = load(vtx1, 0x20).x;
  vlt $v07, $v07, $v07.e2                            ## L:332  |     57 | vwI = min(vwI, vwI.z);
  lsv $v15, 12, 34, $a2                              ## L:339  |      ^ | vinvwF.Z = load(vtx3, 0x22).x;
  vmrg $v08, $v08, $v08.e2                           ## L:334  |     58 | vwF = select(vwF, vwF.z);
  lsv $v14, 10, 32, $a1                              ## L:371  |      ^ | vinvwI.Y = load(vtx2, 0x20).x;
  luv $v11, 0, 8, $a2                                ## L:317  |     59 | vec16<$v11> vattr3 = load_vec_u8(vtx3, 0x08);
  vmudl $v03, $v21, $v24.q1                          ## L:345  |      ^ | asm_op("vmudl", vtmpF, vslopeF, vhml.yywwYYWW);
  vmadm $v03, $v20, $v24.q1                          ## L:346  |     60 | asm_op("vmadm", vtmpF, vslopeI, vhml.yywwYYWW);
  lsv $v15, 8, 34, $a0                               ## L:347  |      ^ | vinvwF.X = load(vtx1, 0x22).x;
  luv $v09, 0, 8, $a0                                ## L:318  |     61 | vec16<$v09> vattr1 = load_vec_u8(vtx1, 0x08);
  vmadn $v03, $v21, $v28.v                           ## L:354  |      ^ | vtmpF = vslopeF:ufract +* vhmlupp:sint;
  andi $t5, $a3, 1024                                ## L:351  |     62 | u32<$t5> tex_dmem = triCmd & 0x400;
  vmadh $v04, $v20, $v28.v                           ## L:355  |      ^ | vtmpI = vslopeI:sint +* vhmlupp:sint;
  andi $t6, $a3, 512                                 ## L:386  |     63 | u32<$t6> z_dmem = triCmd & 0x200;
  vmadn $v03, $v12, $v27.e7                          ## L:361  |      ^ | vtmpF = vk1:ufract +* vmconst:sint.W;
  vmadh $v04, $v12, $v27.e7                          ## L:362  |     64 | vtmpI = vk1:sint +* vmconst:sint.W;
  mfc2 $t1, $v24.e0                                  ## L:360  |      ^ | u32<$t1> negHX = vhml.x;
  vsubc $v29, $v08, $v08.e4                          ## L:367  |     65 | VTEMP = vwF - vwF.X;
  mfc2 $t0, $v24.e3                                  ## L:359  |      ^ | u32<$t0> negMY = vhml.w;
  vlt $v07, $v07, $v07.e4                            ## L:368  |     66 | vwI = min(vwI, vwI.X);
  addiu $at, $zero, 207                              ## L:446  |      ^ | set_vcc(0xCF);
  vmrg $v08, $v08, $v08.e4                           ## L:369  |     67 | vwF = select(vwF, vwF.X);
  lsv $v14, 12, 32, $a2                              ## L:372  |      ^ | vinvwI.Z = load(vtx3, 0x20).x;
  vmudl $v29, $v03, $v01                             ## L:380  |     68 | asm_op("vmudl", VTEMP, vtmpF, vslopem2F);
  subu $t0, $zero, $t0                               ## L:436  |      ^ | negMY = ZERO - negMY;
  vmadm $v29, $v04, $v01                             ## L:381  |     69 | asm_op("vmadm", VTEMP, vtmpI, vslopem2F);
  subu $t1, $zero, $t1                               ## L:437  |      ^ | negHX = ZERO - negHX;
  vmadn $v19, $v03, $v02.v                           ## L:382  |     70 | vnzF = vtmpF:ufract +* vslopem2I:sint;
  ctc2 $at, $vcc                                     ## L:446  |      ^ | set_vcc(0xCF);
  vmadh $v18, $v04, $v02.v                           ## L:383  |     71 | vnzI = vtmpI:sint +* vslopem2I:sint;
  llv $v06, 8, 12, $a2                               ## L:422  |      ^ | vtex3.XY = load(vtx3, 0x0C).xy;
  llv $v03, 8, 12, $a1                               ## L:421  |     72 | vtex2.XY = load(vtx2, 0x0C).xy;
  vmudl $v29, $v15, $v08.e0                          ## L:411  |      ^ | asm_op("vmudl", VTEMP, vinvwF, vwF.x);
  vmadm $v29, $v14, $v08.e0                          ## L:412  |     73 | asm_op("vmadm", VTEMP, vinvwI, vwF.x);
  llv $v04, 8, 12, $a0                               ## L:420  |      ^ | vtex1.XY = load(vtx1, 0x0C).xy;
  vmadn $v15, $v15, $v07.e0                          ## L:413  |     74 | vinvwF = vinvwF:ufract +* vwI:sint.x;
  or $a3, $a3, $t2                                   ## L:393  |      ^ | triCmd |= tileIdx;
  vmudn $v29, $v19, $v24.v                           ## L:430  |     75 | VTEMP = vnzF:ufract * vhml:sint;
  srl $t5, $t5, 4                                    ## L:352  |      ^ | tex_dmem >>= 4;
  vmadh $v29, $v18, $v24.v                           ## L:431  |     76 | VTEMP = vnzI:sint +* vhml:sint;
  sh $a3, 0($s3)                                     ## L:395  |      ^ | @Barrier("tri-out") store(triCmd:u16, outPtr, 0); ## Barrier: 0x1
  srl $t6, $t6, 3                                    ## L:387  |     77 | z_dmem >>= 3;
  vsar $v20, COP2_ACC_HI                             ## L:433  |      ^ | vslopeI = get_acc_high();
  mtc2 $t1, $v24.e0                                  ## L:439  |     78 | vhml.x = negHX;
  vsar $v21, COP2_ACC_MD                             ## L:432  |      ^ | vslopeF = get_acc_mid();
  mtc2 $t0, $v24.e3                                  ## L:438  |     79 | vhml.w = negMY;
  vsubc $v15, $v15, $v30.e7                          ## L:443  |      ^ | vinvwF -= 1;
  lbu $t0, %lo(RDPQ_SYNCFULL_ONGOING + 0)            ## L:493  |     80 | u8<$t0> syncFull = load(RDPQ_SYNCFULL_ONGOING);
  vmudl $v11, $v11, $v31.e6                          ## L:451  |      ^ | vattr3 >>>= 7;
  vmudl $v09, $v09, $v31.e6                          ## L:449  |     81 | vattr1 >>>= 7;
  andi $t3, $a3, 256                                 ## L:472  |      ^ | u32<$t3> prim_size = triCmd & 0x100;
  vmudl $v10, $v10, $v31.e6                          ## L:450  |     82 | vattr2 >>>= 7;
  lsv $v11, 14, 4, $a2                               ## L:456  |      ^ | vattr3.W = load(vtx3, 0x04).x;
  vmudl $v15, $v15, $v31.e0                          ## L:460  |     83 | vinvwF >>>= 1;
  mfc0 $t1, COP0_DMA_BUSY                            ## L:474  |      ^ | u32<$t1> dmaBusy = get_dma_busy();
  vmudl $v29, $v21, $v26.e4                          ## L:470  |     84 | vec32<$v16> vedges = vslope * vfy.X;
  lsv $v10, 14, 4, $a1                               ## L:455  |      ^ | vattr2.W = load(vtx2, 0x04).x;
  vmadm $v29, $v20, $v26.e4                          ## L:470  |     85 | vec32<$v16> vedges = vslope * vfy.X;
  addu $t5, $t5, $t4                                 ## L:388  |      ^ | tex_dmem += color_dmem;
  vmadn $v17, $v21, $v25.e4                          ## L:470  |     86 | vec32<$v16> vedges = vslope * vfy.X;
  lsv $v09, 14, 4, $a0                               ## L:454  |      ^ | vattr1.W = load(vtx1, 0x04).x;
  vmadh $v16, $v20, $v25.e4                          ## L:470  |     87 | vec32<$v16> vedges = vslope * vfy.X;
  sdv $v15, 8, 16, $s3                               ## L:477  |      ^ | @Barrier("tri-oB", before) @Barrier("tri-out") store(vinvwF.XYZW, outPtr, 16); ## Barrier: 0x1
  vmulf $v04, $v04, $v15.h0                          ## L:483  |     88 | vtex1:sfract *= vinvwF.xxxxXXXX;
  lsv $v11, 12, 20, $s3                              ## L:480  |      ^ | @Barrier("tri-oB", after) @Barrier("tri-oB2", before) vattr3.Z = load(outPtr, 20).x;
  vmulf $v06, $v06, $v15.h2                          ## L:485  |     89 | vtex3:sfract *= vinvwF.zzzzZZZZ;
  lsv $v09, 12, 16, $s3                              ## L:478  |      ^ | @Barrier("tri-oB", after) @Barrier("tri-oB2", before) vattr1.Z = load(outPtr, 16).x;
  vmulf $v03, $v03, $v15.h1                          ## L:484  |     90 | vtex2:sfract *= vinvwF.yyyyYYYY;
  lsv $v10, 12, 18, $s3                              ## L:479  |      ^ | @Barrier("tri-oB", after) @Barrier("tri-oB2", before) vattr2.Z = load(outPtr, 18).x;
  vmudm $v22, $v23, $v31.e1                          ## L:490  |     91 | vec16<$v22> vx12I = vx12F >> 2;
  bne $t1, $zero, LABEL_RDPQ_Triangle_Send_Async_0006## L:503  |      ^ | @Unlikely if(dmaBusy != 0)
  vmudn $v23, $v23, $v31.e1                          ## L:491  |     92 | vx12F <<= 14;
  LABEL_RDPQ_Triangle_Send_Async_0005:
  ssv $v20, 8, 12, $s3                               ## L:527  |     93 | @Barrier("tri-oA2", after) @Barrier("tri-out") store(vslope:sint.X, outPtr, 12); ## Barrier: 0x1
  vmrg $v11, $v11, $v06                              ## L:521  |      ^ | vattr3 = select(vattr3, vtex3);
  vmrg $v10, $v10, $v03                              ## L:520  |     94 | vattr2 = select(vattr2, vtex2);
  lw $a0, %lo(RDPQ_CURRENT + 0)                      ## L:532  |      ^ | u32<$a0> rdramCur = load(RDPQ_CURRENT);
  vmrg $v09, $v09, $v04                              ## L:519  |     95 | vattr1 = select(vattr1, vtex1);
  ssv $v22, 4, 8, $s3                                ## L:528  |      ^ | @Barrier("tri-oA2", after) @Barrier("tri-out") store(vx12I.z, outPtr, 8); ## Barrier: 0x1
  vaddc $v17, $v17, $v23.e0                          ## L:537  |     96 | vedges += vx12.x;
  addu $t6, $t6, $t5                                 ## L:514  |      ^ | z_dmem += tex_dmem;
  ssv $v21, 8, 14, $s3                               ## L:529  |     97 | @Barrier("tri-oA2", after) @Barrier("tri-out") store(vslope:ufract.X, outPtr, 14); ## Barrier: 0x1
  vadd $v16, $v16, $v22.e0                           ## L:537  |      ^ | vedges += vx12.x;
  bne $t0, $zero, LABEL_RDPQ_Triangle_Send_Async_0008## L:545  |     98 | if(syncFull == 0) {
  srl $t3, $t3, 4                                    ## L:515  |     99 | prim_size >>= 4;
  mtc0 $a0, COP0_DP_END                              ## L:546  |    100 | set_rdp_end(rdramCur);
  LABEL_RDPQ_Triangle_Send_Async_0008:
  vsubc $v29, $v00, $v00                             ## L:558  |      ^ | clear_vcc();
  vsub $v12, $v10, $v09.v                            ## L:568  |    101 | vec16<$v12> vma:sint = vattr2 - vattr1;
  ssv $v23, 4, 10, $s3                               ## L:554  |      ^ | @Barrier("tri-oA2", after) @Barrier("tri-out") store(vx12F.z, outPtr, 10); ## Barrier: 0x1
  addu $t3, $t3, $t6                                 ## L:622  |    102 | prim_size += z_dmem;
  vsub $v13, $v11, $v09.v                            ## L:569  |      ^ | vec16<$v13> vha:sint = vattr3 - vattr1;
  vmudn $v19, $v19, $v30.e5                          ## L:576  |    103 | vnz = vnz * 4;
  ssv $v20, 4, 28, $s3                               ## L:590  |      ^ | @Barrier("tri-out") store(vslope:sint.z, outPtr, 28); ## Barrier: 0x1
  vmadh $v18, $v18, $v30.e5                          ## L:576  |    104 | vnz = vnz * 4;
  ssv $v21, 4, 30, $s3                               ## L:591  |      ^ | @Barrier("tri-out") store(vslope:ufract.z, outPtr, 30); ## Barrier: 0x1
  ssv $v16, 4, 24, $s3                               ## L:593  |    105 | @Barrier("tri-out") store(vedges:sint.z, outPtr, 24); ## Barrier: 0x1
  vmudh $v29, $v12, $v24.e1                          ## L:583  |      ^ | VTEMP = vma:sint * vhml:sint.y;
  ssv $v17, 4, 26, $s3                               ## L:594  |    106 | @Barrier("tri-out") store(vedges:ufract.z, outPtr, 26); ## Barrier: 0x1
  vmadh $v29, $v13, $v24.e3                          ## L:584  |      ^ | VTEMP = vha:sint +* vhml:sint.w;
  sh $s7, %lo(RDPQ_TRI_BUFF_OFFSET)($zero)           ## L:627  |    107 | store(buffOffset, RDPQ_TRI_BUFF_OFFSET);
  vsar $v03, COP2_ACC_HI                             ## L:586  |      ^ | vdxI = get_acc_high();
  vsar $v04, COP2_ACC_MD                             ## L:585  |    108 | vdxF = get_acc_mid();
  subu $t3, $t3, $s3                                 ## L:623  |      ^ | prim_size -= outPtr;
  vmudh $v29, $v13, $v24.e2                          ## L:602  |    109 | VTEMP = vha:sint * vhml:sint.z;
  ssv $v16, 0, 16, $s3                               ## L:595  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out") store(vedges:sint.x, outPtr, 16); ## Barrier: 0x1
  vmadh $v29, $v12, $v24.e0                          ## L:603  |    110 | VTEMP = vma:sint +* vhml:sint.x;
  ssv $v17, 0, 18, $s3                               ## L:596  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out") store(vedges:ufract.x, outPtr, 18); ## Barrier: 0x1
  ssv $v20, 0, 20, $s3                               ## L:610  |    111 | @Barrier("tri-oB2", after) @Barrier("tri-out") store(vslope:sint.x, outPtr, 20); ## Barrier: 0x1
  vsar $v07, COP2_ACC_HI                             ## L:605  |      ^ | vdyI = get_acc_high();
  vsar $v08, COP2_ACC_MD                             ## L:604  |    112 | vdyF = get_acc_mid();
  ssv $v21, 0, 22, $s3                               ## L:611  |      ^ | @Barrier("tri-oB2", after) @Barrier("tri-out") store(vslope:ufract.x, outPtr, 22); ## Barrier: 0x1
  mtc0 $s3, COP0_DMA_SPADDR                          ## L:613  |    113 | @Barrier("tri-out") set_dma_addr_rsp(outPtr); ## Barrier: 0x1
  vmudl $v29, $v04, $v19.e7                          ## L:616  |      ^ | vdx *= vnz.W;
  vmadm $v29, $v03, $v19.e7                          ## L:616  |    114 | vdx *= vnz.W;
  mtc0 $a0, COP0_DMA_RAMADDR                         ## L:625  |      ^ | @Barrier("tri-out") set_dma_addr_rdram(rdramCur); ## Barrier: 0x1
  vmadn $v04, $v04, $v18.e7                          ## L:616  |    115 | vdx *= vnz.W;
  lw $a2, %lo(RDPQ_SENTINEL + 0)                     ## L:578  |      ^ | u32<$a2> rdramSentinel = load(RDPQ_SENTINEL);
  vmadh $v03, $v03, $v18.e7                          ## L:616  |    116 | vdx *= vnz.W;
  addu $a0, $a0, $t3                                 ## L:631  |      ^ | rdramCur += prim_size;
  vmudl $v29, $v08, $v19.e7                          ## L:618  |    117 | vdy *= vnz.W;
  addu $s3, $s3, $t3                                 ## L:624  |      ^ | outPtr += prim_size;
  sltu $at, $a2, $a0                                 ## L:653  |    118 | @Unlikely if(rdramSentinel < rdramCur)
  vmadm $v29, $v07, $v19.e7                          ## L:618  |      ^ | vdy *= vnz.W;
  vmadn $v08, $v08, $v18.e7                          ## L:618  |    119 | vdy *= vnz.W;
  sdv $v04, 0, 24, $t4                               ## L:634  |      ^ | @Barrier("tri-out") store(vdx:ufract.xyzw, color_dmem, 0x18); ## Barrier: 0x1
  vmadh $v07, $v07, $v18.e7                          ## L:618  |    120 | vdy *= vnz.W;
  sdv $v04, 8, 24, $t5                               ## L:635  |      ^ | @Barrier("tri-out") store(vdx:ufract.XYZW, tex_dmem, 0x18); ## Barrier: 0x1
  sdv $v03, 0, 8, $t4                                ## L:636  |    121 | @Barrier("tri-out") store(vdx:sint.xyzw, color_dmem, 0x08); ## Barrier: 0x1
  vmadl $v29, $v04, $v21.e0                          ## L:640  |      ^ | vec32<$v05> vde = vdx +* vslope.x;
  vmadm $v29, $v03, $v21.e0                          ## L:640  |    122 | vec32<$v05> vde = vdx +* vslope.x;
  sdv $v03, 8, 8, $t5                                ## L:637  |      ^ | @Barrier("tri-out") store(vdx:sint.XYZW, tex_dmem, 0x08); ## Barrier: 0x1
  vmadn $v06, $v04, $v20.e0                          ## L:640  |    123 | vec32<$v05> vde = vdx +* vslope.x;
  sdv $v08, 0, 56, $t4                               ## L:644  |      ^ | @Barrier("tri-out") store(vdy:ufract.xyzw, color_dmem, 0x38); ## Barrier: 0x1
  vmadh $v05, $v03, $v20.e0                          ## L:640  |    124 | vec32<$v05> vde = vdx +* vslope.x;
  bne $at, $zero, LABEL_RDPQ_Triangle_Send_Async_0009## L:653  |      ^ | @Unlikely if(rdramSentinel < rdramCur)
  sdv $v08, 8, 56, $t5                               ## L:647  |    125 | @Barrier("tri-out") store(vdy:ufract.XYZW, tex_dmem, 0x38); ## Barrier: 0x1
  LABEL_RDPQ_Triangle_Send_Async_000A:
  sdv $v07, 0, 40, $t4                               ## L:688  |    126 | @Barrier("tri-out") store(vdy:sint.xyzw, color_dmem, 0x28); ## Barrier: 0x1
  vmudh $v29, $v09, $v30.e7                          ## L:684  |      ^ | VTEMP = vattr1:sint * 1;
  vmadl $v29, $v06, $v26.e4                          ## L:685  |    127 | vec32<$v01> vfinal = vde +* vfy.X;
  sdv $v07, 8, 40, $t5                               ## L:689  |      ^ | @Barrier("tri-out") store(vdy:sint.XYZW, tex_dmem, 0x28); ## Barrier: 0x1
  sdv $v06, 0, 48, $t4                               ## L:691  |    128 | @Barrier("tri-out") store(vde:ufract.xyzw, color_dmem, 0x30); ## Barrier: 0x1
  vmadm $v29, $v05, $v26.e4                          ## L:685  |      ^ | vec32<$v01> vfinal = vde +* vfy.X;
  sdv $v06, 8, 48, $t5                               ## L:692  |    129 | @Barrier("tri-out") store(vde:ufract.XYZW, tex_dmem, 0x30); ## Barrier: 0x1
  vmadn $v02, $v06, $v25.e4                          ## L:685  |      ^ | vec32<$v01> vfinal = vde +* vfy.X;
  sdv $v05, 0, 32, $t4                               ## L:693  |    130 | @Barrier("tri-out") store(vde:sint.xyzw, color_dmem, 0x20); ## Barrier: 0x1
  vmadh $v01, $v05, $v25.e4                          ## L:685  |      ^ | vec32<$v01> vfinal = vde +* vfy.X;
  vmov $v06.e6, $v05.e7                              ## L:718  |    131 | vdeF.Z = vdeI.W;
  sw $a0, %lo(RDPQ_CURRENT)($zero)                   ## L:697  |      ^ | store(rdramCur, RDPQ_CURRENT);
  sdv $v05, 8, 32, $t5                               ## L:694  |    132 | @Barrier("tri-out") store(vde:sint.XYZW, tex_dmem, 0x20); ## Barrier: 0x1
  vmov $v10.e2, $v03.e7                              ## L:722  |      ^ | vzout.z = vdxI.W;
  sdv $v02, 0, 16, $t4                               ## L:725  |    133 | @Barrier("tri-out") store(vfinalF.xyzw, color_dmem, 0x10); ## Barrier: 0x1
  vmov $v08.e6, $v07.e7                              ## L:717  |      ^ | vdyF.Z = vdyI.W;
  vmov $v10.e1, $v02.e7                              ## L:719  |    134 | vzout.y = vfinalF.W;
  sdv $v02, 8, 16, $t5                               ## L:726  |      ^ | @Barrier("tri-out") store(vfinalF.XYZW, tex_dmem, 0x10); ## Barrier: 0x1
  vmov $v10.e0, $v01.e7                              ## L:720  |    135 | vzout.x = vfinalI.W;
  sdv $v01, 0, 0, $t4                                ## L:727  |      ^ | @Barrier("tri-out") store(vfinalI.xyzw, color_dmem, 0x00); ## Barrier: 0x1
  sdv $v01, 8, 0, $t5                                ## L:728  |    136 | @Barrier("tri-out") store(vfinalI.XYZW, tex_dmem, 0x00); ## Barrier: 0x1
  vmov $v10.e3, $v04.e7                              ## L:721  |      ^ | vzout.w = vdxF.W;
  slv $v06, 12, 8, $t6                               ## L:731  |    137 | @Barrier("tri-out") store(vdeF.ZW, z_dmem, 0x08); ## Barrier: 0x1
  slv $v08, 12, 12, $t6                              ## L:732  |    138 | @Barrier("tri-out") store(vdyF.ZW, z_dmem, 0x0C); ## Barrier: 0x1
  addiu $t3, $t3, 65535                              ## L:733  |    139 | prim_size -= 1;
  sdv $v10, 0, 0, $t6                                ## L:734  |    140 | @Barrier("tri-out") store(vzout.xyzw, z_dmem, 0x00); ## Barrier: 0x1
  jr $ra                                             ## L:746  |    141 | }
  mtc0 $t3, COP0_DMA_WRITE                           ## L:745  |   *143 | @Barrier("tri-out", after) set_dma_write(prim_size);
  LABEL_RDPQ_Triangle_Send_Async_0006:
  mfc0 $t1, COP0_DMA_BUSY                            ## L:506  |      1 | dmaBusy = get_dma_busy();
  bne $t1, $zero, LABEL_RDPQ_Triangle_Send_Async_0006## L:506  |    **4 | dmaBusy = get_dma_busy();
  nop                                                ## L:432  |     *6 | vslopeF = get_acc_mid();
  LABEL_RDPQ_Triangle_Send_Async_0007:
  j LABEL_RDPQ_Triangle_Send_Async_0005              ## L:506  |      7 | dmaBusy = get_dma_busy();
  nop                                                ## L:432  |     *9 | vslopeF = get_acc_mid();
  LABEL_RDPQ_Triangle_Send_Async_0009:
  lw $a0, %lo(RDPQ_DYNAMIC_BUFFERS + 4)              ## L:656  |     10 | rdramCur = load(RDPQ_DYNAMIC_BUFFERS, 4);
  sb $t3, 0($t4)                                     ## L:667  |     11 | @Barrier("cb-stash") store(prim_size:u8, color_dmem, 0x00); ## Barrier: 0x2
  or $a3, $zero, $ra                                 ## L:655  |     12 | u32<$a3> savedRA = RA;
  or $a1, $zero, $a0                                 ## L:670  |     13 | u32<$a1> rdramCurCopy = rdramCur;
  lw $t1, %lo(RDPQ_DYNAMIC_BUFFERS + 0)              ## L:657  |     14 | u32<$t1> otherBuff = load(RDPQ_DYNAMIC_BUFFERS, 0);
  sw $a0, %lo(0 + RDPQ_DYNAMIC_BUFFERS)($zero)       ## L:658  |     15 | store(rdramCur, RDPQ_DYNAMIC_BUFFERS, 0);
  lui $at, 0x01                                      ## L:669  |     16 | rdramSentinel = rdramCur + 65536;
  sw $t1, %lo(4 + RDPQ_DYNAMIC_BUFFERS)($zero)       ## L:659  |     17 | store(otherBuff, RDPQ_DYNAMIC_BUFFERS, 4);
  jal RSPQCmd_RdpSetBuffer                           ## L:671  |     18 | RSPQCmd_RdpSetBuffer(rdramCur, rdramCurCopy, rdramSentinel); ## Args: $a0, $a1, $a2
  addu $a2, $a0, $at                                 ## L:669  |    *20 | rdramSentinel = rdramCur + 65536;
  or $ra, $zero, $a3                                 ## L:679  |     21 | RA = savedRA;
  lbu $t3, 0($t4)                                    ## L:677  |     22 | @Barrier("cb-stash") prim_size:u8 = load(color_dmem, 0x00); ## Barrier: 0x2
  mtc0 $a0, COP0_DMA_RAMADDR                         ## L:676  |     23 | @Barrier("tri-out") set_dma_addr_rdram(rdramCur); ## Barrier: 0x1
  j LABEL_RDPQ_Triangle_Send_Async_000A              ## L:679  |     24 | RA = savedRA;
  addu $a0, $a0, $t3                                 ## L:678  |    *26 | rdramCur += prim_size;
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
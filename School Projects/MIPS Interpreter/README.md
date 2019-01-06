A MIPS32 Architecture interpreter written in C. Memory is simulated using a hash table where each bucket is 
the beginning of a linked list. Instructions are written following the standard MIPS convention (e. g. ADDIU rt, rs, imm). 
Our project guidelines did not require the interpreter to support certain instructions (STLIU, NOR, SLTU, MOVN, SRL, SLLV, SRLV, SRAV).

<p>Issues:</p>
<ul>
<li>The linked list fails large stress tests.
<li>SLTI has bugs. 
</ul>

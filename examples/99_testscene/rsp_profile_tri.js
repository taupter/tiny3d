const nStrip = parseInt(ares.args[0] || "6", 10);
const nSeq   = parseInt(ares.args[1] || "3", 10);
const nDraw  = parseInt(ares.args[2] || "6", 10);
const FN_LO = 0x280, FN_HI = 0x6F0;   // T3DCmd_TriDraw .. RDPQ_Triangle_Send_End (excl.)
const dumpSel = ares.args[3] || "";    // e.g. "Tri Strip|288|ret": print the first execution of that bucket

ares.setRenderer("angrylion");
const roms = ["t3d_99_testscene.z64", "examples/99_testscene/t3d_99_testscene.z64"];
let ok = false, err = null;
for (const r of roms) { try { ares.loadRom(r); ok = true; break; } catch (e) { err = e; } }
if (!ok) throw err;
ares.resume();
ares.waitFrames(30);

const buckets = new Map();
function parse(text, cmd) {
  const lines = text.split("\n");
  let cur = null;
  function finish(exitKind, endCyc, lastPc) {
    const key = cur.cmd + "|" + cur.entry.toString(16) + "|" + exitKind + "|" + cur.taken.join(",");
    const b = buckets.get(key) || { key, cmd, entry: cur.entry, exit: exitKind, taken: cur.taken, n: 0, sum: 0, min: 1e9, max: 0, instr: cur.pcs.length, dumped: false };
    if (dumpSel && key.startsWith(dumpSel) && !b.dumped) { b.dumped = true; console.log("// --- execution dump " + key); for (const dl of cur.lines) console.log(dl); console.log("// --- end dump"); }
    const cycles = endCyc - cur.start + 1;
    b.n++; b.sum += cycles; b.min = Math.min(b.min, cycles); b.max = Math.max(b.max, cycles);
    buckets.set(key, b);
    cur = null;
  }
  for (const l of lines) {
    const m = l.match(/^RSP\s+([0-9A-F]+)\s+\[\s*(\d+)\]\s*\S*\s*\|\s*(\S+)\s*(.*)$/);
    if (!m) continue;
    const pc = parseInt(m[1], 16), cyc = +m[2], op = m[3];
    const inside = pc >= FN_LO && pc < FN_HI;
    if (cur && cyc < cur.prevCyc) cur = null;   // new traced occurrence: drop a cut execution
    if (!cur) {
      if (inside && (pc === 0x280 || pc === 0x288)) cur = { entry: pc, start: cyc, pcs: [], taken: [], prevPc: -1, prevOp: "", prevCyc: 0, inCall: false, cmd, lines: [] };
      else continue;
    }
    if (!inside) {
      if (cur.inCall) continue;                 // inside a called helper (e.g. RSPQCmd_RdpSetBuffer)
      if (cur.prevOp === "jal") { cur.inCall = true; continue; }
      // left the function: via jr ra (delay slot already counted) or a reject branch to JrRa
      finish(cur.prevOp === "jr" || cur.pcs.length && cur.exitJr ? "ret" : "reject", cur.prevCyc, cur.prevPc);
      continue;
    }
    if (cur.inCall) { cur.inCall = false; }     // returned from the helper
    cur.pcs.push(pc); cur.lines.push(l);
    if (cur.prevPc >= 0 && pc !== cur.prevPc + 4 && !(cur.pcs.length >= 2 && cur.taken.length && false))
      cur.taken.push(cur.prevPc.toString(16) + ">" + pc.toString(16));
    if (op === "jr" && m[4].startsWith("ra")) cur.exitJr = true;
    cur.prevPc = pc; cur.prevOp = op; cur.prevCyc = cyc;
  }
}
for (const [cmd, n] of [["Tri Strip", nStrip], ["Tri Seq", nSeq], ["Tri Draw", nDraw]]) {
  if (n <= 0) continue;
  const tr = ares.rspTrace(cmd, n);
  parse(tr.text, cmd);
  if (tr.truncated) console.log("// WARNING: trace truncated for " + cmd);
}
const rows = [...buckets.values()].sort((a, b) => b.n - a.n);
console.log("cmd        entry  exit    count  avg     min  max  instr  taken-branches");
for (const b of rows)
  console.log(b.cmd.padEnd(10) + " " + ("0x" + b.entry.toString(16)).padEnd(6) + " " + b.exit.padEnd(7) + " " + String(b.n).padStart(5) + "  " +
              (b.sum / b.n).toFixed(1).padStart(6) + "  " + String(b.min).padStart(3) + "  " + String(b.max).padStart(3) + "  " +
              String(b.instr).padStart(5) + "  " + b.taken.join(" "));

const frames = 30;

ares.setRenderer("angrylion");

ares.loadRom("t3d_99_testscene.z64");
ares.resume();

ares.waitFrames(40);
ares.waitRspCommand("Screen Size");
ares.rspProfileStart();
ares.waitRspCommand("Screen Size", frames);

const t = ares.rspProfile();
const usPerFrame = c => c / frames / 62.5; // 62.5 MHz -> µs per game frame

console.log("");
console.log("RSP profile over " + frames + " game frames  (lostRows: " + t.lostRows + ")");
console.log("total: " + Math.round(t.totalCycles) + " cycles  = " +
            usPerFrame(t.totalCycles).toFixed(2) + " us/frame  (commands " +
            usPerFrame(t.commandCycles).toFixed(2) + ", rspq overhead " +
            usPerFrame(t.overheadCycles).toFixed(2) + ")");
console.log("");
console.log("command".padEnd(30) + "count".padStart(8) + "avg cyc".padStart(10) +
            "cyc/frame".padStart(12) + "us/frame".padStart(10) + "share".padStart(8));

for (const r of t.rows) 
{
  if(r.overlay !== 'tiny3d') continue;
  const name = r.overhead ? "rspq: " + r.overheadType
                          : (r.overlay ? r.overlay + "/" : "") + r.name;
  console.log(name.padEnd(30) +
              String(r.count).padStart(8) +
              r.avg.toFixed(1).padStart(10) +
              String(Math.round(r.cycles / frames)).padStart(12) +
              usPerFrame(r.cycles).toFixed(1).padStart(10) +
              ((r.cycles / t.totalCycles) * 100).toFixed(1).padStart(7) + "%");
}

// grouped summary for the vertex/triangle refactor work
const GROUPS = [
  { label: "triangles (Draw+Strip+Seq)", names: ["Tri Draw", "Tri Strip", "Tri Seq"] },
  { label: "vertices (Vert Load)",       names: ["Vert Load"] },
];
console.log("");
console.log("group".padEnd(30) + "count".padStart(8) + "avg cyc".padStart(10) +
            "cyc/frame".padStart(12) + "us/frame".padStart(10) + "share".padStart(8));
for (const g of GROUPS) {
  let count = 0, cycles = 0;
  for (const r of t.rows) {
    if (!r.overhead && r.overlay === "tiny3d" && g.names.includes(r.name)) {
      count += r.count; cycles += r.cycles;
    }
  }
  console.log(g.label.padEnd(30) +
              String(count).padStart(8) +
              (count ? (cycles / count) : 0).toFixed(1).padStart(10) +
              String(Math.round(cycles / frames)).padStart(12) +
              usPerFrame(cycles).toFixed(1).padStart(10) +
              ((cycles / t.totalCycles) * 100).toFixed(1).padStart(7) + "%");
}

const shot = ares.screenshot();
shot.save("rsp_profile.png");

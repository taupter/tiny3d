// Snapshot test for a single tiny3d example ROM.
//
// Usage: ares-test tests/snapshot.test.js <rom.z64> <snapshot.png> <frames> [options...]
//
// Options:
//   --update       (re-)record the snapshot instead of comparing
//   crop=X,Y,W,H   compare only this subregion (use to mask out FPS/µs overlays)
//   tol=N          per-channel tolerance (e.g. for auto-exposure dither flips)
//   win=K          accept an exact match at any frame in [frames-K, frames+K]
//                  (for animations that shift when ucode timing changes)

const [rom, snapshot, framesArg, ...rest] = ares.args;
if (!rom || !snapshot) {
  throw new Error("usage: snapshot.test.js <rom.z64> <snapshot.png> <frames> [options]");
}
const frames = parseInt(framesArg || "100", 10);
let update = false, tolerance = 0, window_ = 0;
let cropRect = [
  0, 1, 640, 240-2 // ignore first and last row since AA messes things up by sampling garbage data OOB
];
for (const opt of rest) {
  if (opt === "--update") update = true;
  else if (opt.startsWith("crop=")) cropRect = opt.substring(5).split(",").map(Number);
  else if (opt.startsWith("tol=")) tolerance = parseInt(opt.substring(4), 10);
  else if (opt.startsWith("win=")) window_ = parseInt(opt.substring(4), 10);
  else throw new Error("unknown option: " + opt);
}

ares.setRenderer("angrylion");
ares.loadRom(rom);
ares.resume();

function checkCrash() {
  // a crashed ROM shows a static exception screen, which must never
  // pass (or record) a snapshot comparison
  if (ares.log().includes("RSP CRASH") || ares.log().includes("CPU exception")) {
    throw new Error("ROM crashed:\n" + ares.log());
  }
}

function grab() {
  let shot = ares.screenshot();
  shot = shot.crop(cropRect[0], cropRect[1], cropRect[2], cropRect[3]);
  return shot;
}

let snapshotImg = null;
try { snapshotImg = ares.loadImage(snapshot); } catch (e) { /* no snapshot yet */ }

if (update || snapshotImg === null) {
  ares.waitFrames(frames);
  checkCrash();
  const shot = grab();
  shot.save(snapshot);
  console.log((snapshotImg === null ? "created" : "updated"), "snapshot:", snapshot,
              shot.width + "x" + shot.height, shot.sha256);
} else {
  ares.waitFrames(frames - window_);
  checkCrash();
  let shot = grab();
  let cmp = shot.compare(snapshotImg, tolerance);
  let frame = ares.frameCount();
  // shift-window: step frame by frame until one matches exactly
  for (let k = 0; !cmp.match && k < 2 * window_; k++) {
    ares.waitVI();
    shot = grab();
    cmp = shot.compare(snapshotImg, tolerance);
    frame = ares.frameCount();
  }
  checkCrash();
  console.log("snapshot:", rom.split("/").pop(), "frame", frame,
              "size", shot.width + "x" + shot.height, "sha256", shot.sha256);
  if (!cmp.match) {
    const actual = snapshot.replace(/\.png$/, ".actual.png");
    const diff = snapshot.replace(/\.png$/, ".diff.png");
    shot.save(actual);
    cmp.diff.save(diff);
    throw new Error("snapshot mismatch for " + rom + ": " + JSON.stringify(cmp) +
                    " (actual frame saved to " + actual + ")");
  }
  console.log("snapshot: OK" + (frame !== frames ? " (matched at frame " + frame + ")" : ""));
}

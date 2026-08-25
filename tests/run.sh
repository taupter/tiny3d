#!/usr/bin/env bash
# Snapshot tests, requires ares-64 test runner.
#
# Usage: tests/run.sh [--update] [example ...]
#   --update      re-record the snapshot PNGs instead of comparing
#   example       one or more example names (e.g. 00_quad); default: all below
#
# Snapshot PNGs live in tests/snapshots/. A test boots the ROM, runs a fixed
# number of VI frames under the bit-exact angrylion renderer and compares
# the presented frame pixel-for-pixel.

set -u
cd "$(dirname "$0")/.."

ARES_TEST=${ARES_TEST:-ares-test}
SNAPSHOTS=tests/snapshots

# example directory -> "rom-file frames [options]"
# Options (see snapshot.test.js):
#   crop=X,Y,W,H  compare a subregion only (masks out timing-dependent overlays)
#   tol=N         per-channel tolerance
#   win=X         accept an exact match within +- X frames (e.g. VI interrupt timing after changes)

declare -A TESTS=(
  [00_quad]="t3d_00_quad.z64 100"
  [01_model]="t3d_01_model.z64 100"
  [02_lighting]="t3d_02_light.z64 100"
  [03_objects]="t3d_03_objects.z64 100"
  [04_dynamic]="t3d_04_dynamic.z64 100 win=15"
  [05_splitscreen]="t3d_05_splitscreen.z64 100"
#  06_offscreen: ignored due to that noise texture on the TV
  [07_skeleton]="t3d_07_skeleton.z64 100"
  [08_animation]="t3d_08_animation.z64 100"
# 09_anim_viewer: animations a based on exact float time, can shift slightly with changes
  [10_flipbook_tex]="t3d_10_flipbook_tex.z64 100"
  [11_segments]="t3d_11_segments.z64 100"
  [12_uv_gen]="t3d_12_uv_gen.z64 100"
  [13_cel_shading]="t3d_13_cel_shading.z64 100 win=15"
  [14_outline]="t3d_14_outline.z64 100"
  [15_pointlight]="t3d_15_pointlight.z64 100"
  [16_light_clip]="t3d_16_light_clip.z64 100"
  [17_culling]="t3d_17_culling.z64 100 crop=0,24,640,176"
  [18_particles]="t3d_18_particles.z64 100"
  [19_particles_tex]="t3d_19_particles_tex.z64 100"
  [20_mipmaps]="t3d_20_mipmaps.z64 100"
  [21_fresnel]="t3d_21_fresnel.z64 100"
  [22_bigtex]="t3d_22_bigtex.z64 100"
  [23_hdr]="t3d_23_hdr.z64 100 tol=8"
  [24_hdr_bloom]="t3d_24_hdr_bloom.z64 100"
  [99_testscene]="t3d_99_testscene.z64 100"
)
ORDER=(00_quad 01_model 02_lighting 03_objects 04_dynamic 05_splitscreen
       07_skeleton 08_animation 10_flipbook_tex
       11_segments 12_uv_gen 13_cel_shading 14_outline 15_pointlight
       16_light_clip 17_culling 18_particles 19_particles_tex 20_mipmaps
       21_fresnel 22_bigtex 23_hdr 24_hdr_bloom 99_testscene)

update=""
selected=()
for arg in "$@"; do
  case "$arg" in
    --update) update="--update" ;;
    *) selected+=("$arg") ;;
  esac
done
[[ ${#selected[@]} -eq 0 ]] && selected=("${ORDER[@]}")

mkdir -p "$SNAPSHOTS"
pass=0 fail=0

for name in "${selected[@]}"; do
  if [[ -z "${TESTS[$name]:-}" ]]; then
    echo "FAIL $name (unknown example)"
    fail=$((fail + 1))
    continue
  fi
  read -r rom frames opts <<<"${TESTS[$name]}"
  rom="examples/$name/$rom"
  snapshot="$SNAPSHOTS/$name.png"
  if [[ ! -f "$rom" ]]; then
    echo "FAIL $name (ROM not built: $rom)"
    fail=$((fail + 1))
    continue
  fi
  if out=$("$ARES_TEST" tests/snapshot.test.js "$rom" "$snapshot" "$frames" $opts $update 2>&1); then
    echo "PASS $name"
    pass=$((pass + 1))
  else
    echo "FAIL $name"
    sed 's/^/  | /' <<<"$out"
    fail=$((fail + 1))
  fi
done

echo "----"
echo "pass=$pass fail=$fail"
[[ $fail -eq 0 ]] || exit 1

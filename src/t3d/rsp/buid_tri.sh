set -e

~/Documents/projects/rspl/cpp/build/rspl rspq_triangle.rspl \
    --no-rspq=include --reorder --opt-anneal --opt-time=10  \
    --patch RDPQ_Triangle_Send_Async \
    -o rspq_triangle_rspl.inc

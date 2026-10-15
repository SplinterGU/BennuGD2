#!/usr/bin/env bash
set -Eeuo pipefail

usage() {
    cat <<'EOF'
Usage: bash build-pkg-ps3.sh game.prg --id BNGD00001 [options]
  --id ID                 Four uppercase letters followed by five digits.
  --title TITLE           Defaults to the PRG file name.
  --elf FILE              Defaults to $BGD2DEV/build/powerpc64-ps3-elf/bin/bgdi.
  --bgdc COMMAND          Defaults to bgdc.
  --resources FILE        Defaults to resources.txt in the current directory.
  --scan-include-dir DIR  Extra include search directory; repeatable, collector only.
  --sfo-template FILE     Defaults to $PS3DEV/bin/sfo.xml.
  --content-id ID         Defaults to UP0001-<ID>_00-0000000000000000.
  --icon FILE             Optional; installed as ICON0.PNG.
  --out FILE              Defaults to <PRG-name>.pkg.
  --force                 Overwrite an existing output file.
  -DNAME[=VALUE]          Pass a define to bgdc; repeatable.
  -D NAME[=VALUE]         Same as above.
  --no-finalize           Skip package_finalize.

Literal includes are traversed recursively and their quoted strings collected.
Those strings are combined with resources.txt patterns: *, ?, [abc], **, and directories.
Resources are resolved from the directory where this script is executed.
Their relative structure is preserved under USRDIR; no assets folder is added.
Source files are never packaged, even if selected by resources.txt.
Include macros, path variables, and ~ are not expanded.
EOF
}

die() {
    printf 'Error: %s\n' "$*" >&2
    exit 1
}

(($#)) || { usage; exit 1; }
case "$1" in
    -h|--help) usage; exit 0 ;;
esac

PRG="$1"
shift
BASE="$PWD"
NAME="$(basename -- "$PRG")"
NAME="${NAME%.*}"
TITLE="$NAME"
TITLE_ID=""
CONTENT_ID=""
ELF="${BGD2DEV:-.}/build/powerpc64-ps3-elf/bin/bgdi"
BGDC="bgdc"
RESOURCES="$BASE/resources.txt"
RESOURCES_EXPLICIT=0
SCAN_INCLUDE_DIRS=()
SFO_TEMPLATE="${PS3DEV:-/usr/local/ps3dev}/bin/sfo.xml"
ICON=""
OUT="$BASE/$NAME.pkg"
FINALIZE=1
FORCE=0
BGDC_DEFINES=()

while (($#)); do
    case "$1" in
        --id|--title|--elf|--bgdc|--resources|--scan-include-dir|--sfo-template|--content-id|--icon|--out)
            (($# >= 2)) || die "Missing value for $1."
            case "$1" in
                --id) TITLE_ID="$2" ;;
                --title) TITLE="$2" ;;
                --elf) ELF="$2" ;;
                --bgdc) BGDC="$2" ;;
                --resources) RESOURCES="$2"; RESOURCES_EXPLICIT=1 ;;
                --scan-include-dir) SCAN_INCLUDE_DIRS+=("$2") ;;
                --sfo-template) SFO_TEMPLATE="$2" ;;
                --content-id) CONTENT_ID="$2" ;;
                --icon) ICON="$2" ;;
                --out) OUT="$2" ;;
            esac
            shift 2
            ;;
        --force)
            FORCE=1
            shift
            ;;
        -D)
            (($# >= 2)) || die "Missing value for -D."
            [[ -n "$2" && "$2" != -* ]] || die "Invalid define: $2"
            BGDC_DEFINES+=("-D$2")
            shift 2
            ;;
        -D?*)
            BGDC_DEFINES+=("$1")
            shift
            ;;
        --no-finalize)
            FINALIZE=0
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            die "Unknown option: $1"
            ;;
    esac
done

[[ -f "$PRG" ]] || die "PRG does not exist: $PRG"
[[ -f "$ELF" ]] || die "ELF does not exist: $ELF"
[[ -f "$SFO_TEMPLATE" ]] || die "Template does not exist: $SFO_TEMPLATE"
[[ -n "$TITLE" ]] || die "Title cannot be empty."
[[ "$TITLE_ID" =~ ^[A-Z]{4}[0-9]{5}$ ]] || die "Use --id, for example BNGD00001."

if ((RESOURCES_EXPLICIT)); then
    [[ -f "$RESOURCES" ]] || die "Manifest does not exist: $RESOURCES"
fi

for directory in "${SCAN_INCLUDE_DIRS[@]}"; do
    [[ -d "$directory" ]] || die "Include directory does not exist: $directory"
done

[[ -z "$ICON" || -f "$ICON" ]] || die "Icon does not exist: $ICON"

CONTENT_ID="${CONTENT_ID:-UP0001-${TITLE_ID}_00-0000000000000000}"
[[ "$CONTENT_ID" =~ ^[A-Z]{2}[0-9]{4}-[A-Z0-9]{9}_00-[A-Z0-9]{16}$ ]] ||
    die "Invalid CONTENT_ID."
[[ "${CONTENT_ID:7:9}" == "$TITLE_ID" ]] ||
    die "CONTENT_ID and TITLE_ID do not match."

for tool in "$BGDC" python3 ppu-strip sprxlinker make_self_npdrm sfo pkg; do
    command -v "$tool" >/dev/null 2>&1 || die "Tool not found: $tool"
done

if ((FINALIZE)); then
    command -v package_finalize >/dev/null 2>&1 ||
        die "package_finalize is missing; use --no-finalize if you do not need it."
fi

[[ ! -d "$OUT" ]] || die "Output is a directory: $OUT"
if [[ -e "$OUT" || -L "$OUT" ]]; then
    ((FORCE)) || die "Output already exists: $OUT (use --force to overwrite)."
fi

mkdir -p -- "$(dirname -- "$OUT")"
WORK="$(mktemp -d)"
trap 'rm -rf -- "$WORK"' EXIT
ROOT="$WORK/pkg"
mkdir -p "$ROOT/USRDIR"

python3 - "$BASE" "$PRG" "$RESOURCES" "$ROOT/USRDIR" "${SCAN_INCLUDE_DIRS[@]}" <<'PY'
import glob
import os
from pathlib import Path
import re
import shutil
import sys

base = Path(sys.argv[1]).resolve()
prg = Path(sys.argv[2])
manifest = Path(sys.argv[3])
root = Path(sys.argv[4])
scan_dirs = [Path(p).resolve() for p in sys.argv[5:]]
reserved = {'eboot.bin', 'game.dcb'}
source_exts = {'.prg', '.h', '.inc', '.c', '.def'}
copied = set()
sources = set()
resource_strings = []
lexer = re.compile(
    r'(?P<comment>/\*.*?\*/|//[^\r\n]*)'
    r'|(?P<string>"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\')'
    r'|(?P<identifier>[A-Za-z_][A-Za-z_0-9]*)'
    r'|(?P<symbol>[^\s])', re.S
)

def inside(path):
    try:
        path.relative_to(base)
        return True
    except ValueError:
        return False

def decode_string(token):
    value = token[1:-1]
    quote = token[0]
    return value.replace('\\' + quote, quote).replace('\\\\', '\\')

def resolve_include(name, parent):
    candidate = Path(name)
    if candidate.is_absolute():
        candidates = [candidate]
    else:
        candidates = [parent / candidate, base / candidate]
        candidates += [directory / candidate for directory in scan_dirs]
    for path in candidates:
        if path.is_file():
            return path.resolve()
    return None

def scan_source(path):
    path = path.resolve()
    if path in sources:
        return
    if not path.is_file():
        raise ValueError(f'Source does not exist: {path}')
    sources.add(path)
    text = path.read_bytes().decode('utf-8-sig', errors='surrogateescape')
    tokens = [m for m in lexer.finditer(text) if m.lastgroup != 'comment']
    include_strings = set()
    includes = []
    for i, token in enumerate(tokens):
        if token.lastgroup != 'identifier' or token.group().lower() != 'include':
            continue
        line = text.count('\n', 0, token.start()) + 1
        if i + 1 >= len(tokens):
            raise ValueError(f'{path}:{line}: include without a literal name')
        next_token = tokens[i + 1]
        if next_token.lastgroup == 'string':
            name = decode_string(next_token.group())
            include_strings.add(i + 1)
        elif next_token.group() == '<':
            end = next((j for j in range(i + 2, len(tokens))
                        if tokens[j].group() == '>' or '\n' in text[next_token.end():tokens[j].start()]), None)
            if end is None or tokens[end].group() != '>':
                raise ValueError(f'{path}:{line}: incomplete include <...>')
            name = text[next_token.end():tokens[end].start()].strip()
        else:
            raise ValueError(f'{path}:{line}: non-literal include; the collector does not expand macros')
        target = resolve_include(name, path.parent)
        if target is None:
            raise ValueError(f'{path}:{line}: cannot find include {name!r}; use --scan-include-dir if appropriate')
        includes.append(target)
    for i, token in enumerate(tokens):
        if token.lastgroup == 'string' and i not in include_strings:
            resource_strings.append(decode_string(token.group()))
    print(f'Scanned source: {path}')
    for target in includes:
        scan_source(target)

def copy_entry(path, ancestors=frozenset()):
    logical = Path(os.path.abspath(path))
    real = logical.resolve()
    if real in sources or logical.suffix.lower() in source_exts:
        return
    if not inside(logical) or not inside(real):
        raise ValueError(f'Resource outside the working directory: {path}')
    rel = logical.relative_to(base)
    if not rel.parts:
        raise ValueError('Including the complete root with . or an equivalent pattern is not supported')
    if rel.parts[0].lower() in reserved:
        raise ValueError(f'Reserved runtime path: {rel}')
    dest = root / rel
    if logical.is_dir():
        if real in ancestors:
            raise ValueError(f'Symlink cycle in directory: {path}')
        dest.mkdir(parents=True, exist_ok=True)
        for child in sorted(logical.iterdir()):
            copy_entry(child, ancestors | {real})
    elif logical.is_file():
        if rel not in copied:
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(logical, dest)
            copied.add(rel)
            print(f'Resource: {rel}')
    else:
        raise ValueError(f'Resource is not a regular file or has a broken symlink: {path}')

def include_resource(pattern, explicit):
    if not pattern or pattern in {'.', '..'}:
        if explicit and pattern in {'.', '..'}:
            raise ValueError(f'Unsupported pattern: {pattern}')
        return
    candidate = Path(os.path.abspath(base / pattern))
    if not inside(candidate):
        if explicit:
            raise ValueError(f'Pattern outside the working directory: {pattern}')
        return
    if candidate.exists() or candidate.is_symlink():
        matches = [str(candidate)]
    else:
        matches = sorted(glob.glob(str(candidate), recursive=True))
    if not matches and explicit:
        print(f'Warning: pattern has no matches: {pattern}', file=sys.stderr)
    for match in matches:
        copy_entry(Path(match))

try:
    scan_source(prg)
    for value in resource_strings:
        include_resource(value, False)
    if manifest.is_file():
        for number, raw in enumerate(manifest.read_text(encoding='utf-8-sig').splitlines(), 1):
            pattern = raw.strip()
            if not pattern or pattern.startswith('#'):
                continue
            try:
                include_resource(pattern, True)
            except ValueError as exc:
                raise ValueError(f'{manifest}:{number}: {exc}') from exc
    print(f'Total: {len(sources)} sources scanned; {len(copied)} resource files')
except (OSError, ValueError, RecursionError) as exc:
    sys.exit(f'Error collecting resources: {exc}')
PY

"$BGDC" "${BGDC_DEFINES[@]}" "$PRG" -o "$ROOT/USRDIR/game.dcb"
[[ -s "$ROOT/USRDIR/game.dcb" ]] || die "bgdc did not generate game.dcb."

ppu-strip "$ELF" -o "$WORK/bgdi.elf"
sprxlinker "$WORK/bgdi.elf"
make_self_npdrm "$WORK/bgdi.elf" "$ROOT/USRDIR/EBOOT.BIN" "$CONTENT_ID"

sfo \
    "--title=$TITLE" \
    "--appid=$TITLE_ID" \
    --fromxml "$SFO_TEMPLATE" \
    "$ROOT/PARAM.SFO"

if [[ -n "$ICON" ]]; then
    cp -- "$ICON" "$ROOT/ICON0.PNG"
fi

pkg "--contentid=$CONTENT_ID" "$ROOT/" "$WORK/result.pkg"

if ((FINALIZE)); then
    package_finalize "$WORK/result.pkg"
fi

[[ -s "$WORK/result.pkg" ]] || die "PKG was not generated."
if ((FORCE)); then
    rm -f -- "$OUT"
else
    [[ ! -e "$OUT" && ! -L "$OUT" ]] ||
        die "Output already exists: $OUT (use --force to overwrite)."
fi
cp -- "$WORK/result.pkg" "$OUT"

printf '\nPKG: %s\nTITLE_ID: %s\nCONTENT_ID: %s\n' \
    "$OUT" "$TITLE_ID" "$CONTENT_ID"
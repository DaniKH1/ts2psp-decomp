import pathlib

done = set()
for p in pathlib.Path('src/eboot').glob('*.c'):
    done.add(p.stem)

for p in sorted(pathlib.Path('asm/eboot').glob('*.s')):
    if p.stem in done:
        continue
    text = p.read_text(encoding='utf-8', errors='replace')
    chunk = text.split('glabel ' + p.stem, 1)[-1].split('endlabel', 1)[0]
    ins = [l.split('*/',1)[1].strip() for l in chunk.splitlines() if '/*' in l and '*/' in l]
    ins = [i for i in ins if i and not i.startswith('.')]
    if 3 <= len(ins) <= 6:
        print(f'{p.stem}: {len(ins)} ins')
        for i in ins:
            print(f'  {i}')
        print()
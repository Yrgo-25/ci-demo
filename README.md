# CI-demo
Demorepot från **L05**, där en CI-pipeline skrevs fram live, commit för commit. Projektet är
avsiktligt litet: när "firmwaren" består av ett par filer är det pipelinen, och inget annat, som
står i centrum.

Repot har byggts ut efter lektionen. Under **L05** fanns bara en Hello World-firmware, en
`Makefile` samt en `ci.yml`. Timer-drivern, enhetstesterna och skripten i `ci/` tillkom efteråt,
som ett exempel på hur en kodbas kan struktureras:
* Ett interface med en implementation bakom sig.
* Tester i en egen mapp med en egen makefile.
* De kommandon pipelinen kör samlade i skript som ni kan köra lokalt. 

Vill ni se pipelinen växa fram precis som på lektionen, läs historiken fram till commiten 
`Update ci.yml` med `git log --reverse --oneline`.

---

## Vad programmet gör
`source/main.cpp` skapar en timer med 500 ms timeout och kör den i 5000 varv. Varje varv sover
programmet 1 ms och stegar timern ett tick framåt, och vid varje timeout skrivs en rad ut:

```text
Timeout after 500 ms!
```

Timern är `driver::timer::Stub`, en hårdvaruoberoende implementation av
`driver::timer::Interface` som räknar `tick()`-anrop i stället för att mäta riktig tid. Det är
också den som testsviten testar.

```text
include/driver/timer/   Interfacet samt stubben.
source/                 Firmwaren.
test/                   Testsviten, med egen makefile.
ci/                     Skripten som både ni och pipelinen kör.
libs/test               Testramverket yrgo::test (submodul).
```

---

## Bygga och köra
Klona repot och initiera testramverket `libs/test`, som utgör en submodul i detta repo:

```bash
git clone git@github.com:Yrgo-25/ci-demo.git
git submodule update --init --recursive
```

| Kommando | Beskrivning |
|---|---|
| `make` | Bygger och kör firmwaren (tar ungefär sju sekunder). |
| `make unit-test` | Bygger och kör testsviten, 21 testfall för timerstubben. |
| `make check-format` | Kontrollerar kodformatteringen, ändrar ingenting. |
| `make format` | Formaterar om koden enligt `.clang-format`. |
| `make clean` | Tar bort byggda binärer, både firmwarens och testsvitens. |

Samtliga targets anropar skripten i [ci/](./ci), vilket är hela poängen: **ni kör exakt samma
kommandon som pipelinen kör**, så ett rött jobb går alltid att återskapa lokalt.

---

## Pipelinen
Pipelinen ligger i [.github/workflows/ci.yml](./.github/workflows/ci.yml) och körs vid varje push
och pull request. Den består av tre jobb:

| Jobb | Kör | Beskrivning |
|---|---|---|
| `analyze-format` | `ci/format.sh --check` | Kontrollerar kodformatteringen. |
| `unit-tests` | `ci/unit_test.sh` | Bygger och kör testsviten. |
| `firmware-build` | `ci/build.sh` | Bygger firmwaren, kör den och laddar upp binären som artefakt. |

`analyze-format` körs först, och är enda anledningen till att de två andra jobben väntar
(`needs:`). Den kontrollen tar sekunder, medan ett riktigt firmware-bygge tar minuter; misslyckas
formatteringen är det ingen idé att bygga något alls. De två övriga jobben kör därefter parallellt,
var och en på en egen maskin.

Filen gås igenom rad för rad i bilaga A till **L05**.

---

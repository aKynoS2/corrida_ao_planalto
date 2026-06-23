# 🗳️ Corrida ao Planalto

## RPG de terminal ambientado nas Eleições 2026

> _"Você não tem partido, não tem dinheiro, não tem jabá. Mas tem voto — e voto se conquista."_

---

```c
██████╗  ██████╗  ██████╗ ██████╗  ██╗██████╗  █████╗
██╔════╝ ██╔═══██╗██╔══██╗██╔══██╗ ██║██╔══██╗██╔══██╗
██║      ██║   ██║██████╔╝██████╔╝ ██║██║  ██║███████║
██║      ██║   ██║██╔══██╗██╔══██╗ ██║██║  ██║██╔══██║
╚██████╗ ╚██████╔╝██║  ██║██║  ██║ ██║██████╔╝██║  ██║
 ╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝╚═════╝ ╚═╝  ╚═╝
          AO PLANALTO — uma campanha sem retorno
```

---

## Sobre o jogo

**Corrida ao Planalto** é um RPG de terminal desenvolvido em **C puro**, onde você interpreta um candidato independente tentando chegar à presidência do Brasil partindo do absoluto zero — sem partido, sem fundo eleitoral, sem assessores de imagem.

Você vai atravessar as regiões do Brasil enfrentando lobistas, bots de redes sociais, coronéis e políticos veteranos no caminho até o confronto final em Brasília.

Cada região que você conquista contribui à sua experiência política. Cada combate ganha melhora seus atributos. Cada baú aberto traz armas e itens estratégicos.

---

## Status Atual: Week 4 ✅

O projeto está **funcionalmente completo** para a entrega acadêmica da disciplina de Algoritmos e Programação 2.

### Sistemas Implementados

| Sistema               | Status | Detalhes                                                            |
| --------------------- | ------ | ------------------------------------------------------------------- |
| ⚔️ Combate por turnos | ✅     | Menu jogador/turno IA, cálculo de dano, ganho de XP                 |
| 🗺️ Movimentação       | ✅     | WASD, colisão com paredes, viewport 20×20 com câmera centralizada   |
| 📈 Progressão         | ✅     | XP, level up com melhoria automática de stats                       |
| 🎒 Inventário         | ✅     | Adicionar, listar, usar itens; limite 20 slots                      |
| 🎁 Baús               | ✅     | Spawnam itens aleatórios do catálogo                                |
| 💾 Save/Load          | ✅     | 3 slots de save binário; menu de sobrescrita                        |
| ⏸️ Pausa              | ✅     | Tecla `P`, menu com continuar/salvar/sair                           |
| 🌍 Mapas              | ✅     | Carregamento via `.txt`; 2 mapas funcionais (Norte ↔ Nordeste)      |
| 🚪 Transições         | ✅     | Múltiplas saídas por mapa; bidirecional; com persistência de estado |
| 🎨 Símbolos Unicode   | ✅     | Suporte UTF-8; tabela de símbolos com busca por nome                |

### Estrutura Implementada

```c
src/
├── main.c          # Loop principal, menu, navegação do mapa
├── mapa.c/.h       # Renderização, câmera, carregamento de mapas
├── combate.c/.h    # Loop de combate, cálculo de dano
├── personagem.c/.h # Structs, level up, inventário, itens
├── utils.c/.h      # Salvar/carregar, tela de pausa, utilitários
├── inimigos.h      # Catálogo de 5 inimigos predefinidos
├── itens.h         # Catálogo de 5 itens predefinidos
└── simbolos.h      # Tabela de símbolos com comportamento

data/
├── maps/
│   ├── mapa_norte.txt
│   └── mapa_nordeste.txt
└── saves/
    ├── save1.dat
    ├── save2.dat
    ├── save3.dat
    └── slot*.*.state (files de persistência do mapa por slot)
```

---

## Mecânicas Confirmadas

| Mecânica        | Como funciona                                                 |
| --------------- | ------------------------------------------------------------- |
| **Movimento**   | WASD move o jogador; colisão com transitável=0 bloqueia       |
| **Combate**     | Turno jogador ataca ou foge → turno IA → repeat até morte     |
| **XP/Level Up** | Vitória = `xp_recompensa` do inimigo; threshold = `nível*100` |
| **Inventário**  | Tecla `I`; lista itens numerados; `usar_item()` restaura HP   |
| **Baús**        | Encontro automático ao pisar; sorteia do catálogo             |
| **Save/Load**   | Menu em `main.c`; slots 1-3; sobrescrita com confirmação      |
| **Pausa**       | Tecla `P`; salva estado do mapa em arquivo `.state`           |
| **Transição**   | Pisar em saída → carrega novo mapa → reposiciona jogador      |

---

## Controles

| Tecla   | Ação                    |
| ------- | ----------------------- |
| `W`     | Mover para cima         |
| `A`     | Mover para esquerda     |
| `S`     | Mover para baixo        |
| `D`     | Mover para direita      |
| `I`     | Abrir inventário        |
| `P`     | Pausa (salvar/carregar) |
| `Enter` | Confirmar / continuar   |

---

## Atributos do Personagem

| Atributo RPG | Equivalente político    | Escala         |
| ------------ | ----------------------- | -------------- |
| ❤️ HP        | Reputação               | 20 → +5/nível  |
| 💰 Mana      | Verba de campanha       | 30 (not used)  |
| 🗣️ Ataque    | Poder de convencimento  | 8 → +1/nível   |
| 🛡️ Defesa    | Resistência a escândalo | 3 → +1/nível   |
| 🗳️ XP        | Votos acumulados        | 0 → nível\*100 |

---

## Catálogo de Inimigos

| #   | Nome                       | HP  | ATK | DEF | XP  | Tipo |
| --- | -------------------------- | --- | --- | --- | --- | ---- |
| 0   | Lobista                    | 15  | 4   | 2   | 25  | 0    |
| 1   | Bot de Rede Social         | 10  | 6   | 1   | 20  | 1    |
| 2   | Cabo Eleitoral Corrompido  | 12  | 4   | 3   | 22  | 2    |
| 3   | Jornalista Sensacionalista | 18  | 7   | 1   | 35  | 3    |
| 4   | Político Veterano          | 25  | 5   | 6   | 50  | 4    |

---

## Catálogo de Itens

| #   | Nome              | Tipo     | Valor | Efeito                     |
| --- | ----------------- | -------- | ----- | -------------------------- |
| 0   | Santinho          | Poção    | 10    | Recupera 10 HP             |
| 1   | Horário Eleitoral | Arma     | 5     | Buff de ataque (planejado) |
| 2   | Dossiê            | Poção    | 0     | Especial (planejado)       |
| 3   | Caixa 2           | Arma     | 15    | Buff ataque (planejado)    |
| 4   | Apoio Popular     | Armadura | 3     | Buff defesa (planejado)    |

---

## Mapas Atuais

| Mapa     | Tamanho | Inimigos | Baús | Saídas | Status      |
| -------- | ------- | -------- | ---- | ------ | ----------- |
| Norte    | 20×20   | 3        | 2    | 1      | ✅ Pronto   |
| Nordeste | 20×20   | 3        | 2    | 1      | ✅ Pronto   |
| C. Oeste | —       | —        | —    | —      | ⏳ Pendente |
| Sudeste  | —       | —        | —    | —      | ⏳ Pendente |
| Sul      | —       | —        | —    | —      | ⏳ Pendente |
| Brasília | —       | —        | —    | —      | ⏳ Pendente |

---

## Como Compilar e Rodar

### Requisitos

- **GCC** ou outro compilador C compatível com C99
- **Terminal** com suporte a Unicode e cores ANSI

### Build

```bash
gcc src/*.c -I./include -o jogo -Wall -Wextra -g
```

### Executar

```bash
# Windows
jogo.exe

# Linux/macOS
./jogo
```

---

## Decisões de Design

### Coordinate System (0-indexed arrays)

- Eixo Y: linha 0 é o topo; aumenta para baixo
- Eixo X: coluna 0 é a esquerda; aumenta para direita
- Jogador sempre representado por `@` na posição correta

### Buffer Management

- `limpar_buffer()` é chamado **após** `scanf()` para limpar Enter residual
- Nunca antes, ou exigirá double-Enter do usuário

### Save/Load

- Estrutura `PERSONAGEM` e `MAPA` salvas em binário (`.dat`)
- Estado do mapa (inimigos mortos, baús abertos) em arquivo `.state` separado
- Permite recarregar mesmo mapa sem ressetar progresso

### Transições de Mapa

- Metadados em bloco após `---` no `.txt` do mapa
- Formato: `S x y destino_arquivo destino_x destino_y`
- Saídas copiadas para variáveis locais antes de `carregar_mapa()` (que sobrescreve)

### Símbolos

- Tabela `DEFINICAO_SIMBOLO` mapeia símbolo → transitável/inimigo/baú/saída
- Função `buscar_simbolo()` evita chains de `if/else`
- Suporte UTF-8: `char simbolo[5]`

---

## Pendências Conhecidas (Post-Week 4)

### Alto Impacto

- **Grid Dinâmico**: Array `CELULA grid[20][20]` é fixo; mapas maiores que 20×20 vão exigir alocação dinâmica
- **Mais Mapas**: Faltam 4 regiões (Centro-Oeste, Sudeste, Sul, Brasília)

### Escopo Planejado

- Sistema de aliados (até 2 por run)
- Sistema de escândalos (flags que afetam combate)
- Boss final escalável por nível

### Polish

- Input sem Enter (termios/raw mode) — fora do escopo, complexidade alta
- Animações de combate
- Cores por tipo de célula

---

## Arquitetura do Código

### Fluxo Principal (`main.c`)

```
MENU
├─ Novo Jogo → Carrega mapa inicial → Jogo
├─ Carregar → Lê save + recarrega mapa com estado → Jogo
└─ Sair → Exit

JOGO (loop infinito)
├─ Renderiza viewport
├─ Lê input (WASD / I / P)
├─ Valida movimento (colisão)
├─ Detecta inimigo → Combate
├─ Detecta baú → Adiciona item
├─ Detecta saída → Transição de mapa
└─ [P] Pausa → Menu (continuar/salvar/sair)
```

### Persistência

**Save Slot (`.dat`)**

```
[PERSONAGEM struct]
[MAPA struct]
```

**Mapa State (`.state`)**

```
[CELULA grid[20][20] — estado atual do mapa]
```

Exemplo: `data/saves/slot1_mapa_norte.txt.state`

---

## Contexto Acadêmico

Projeto para disciplina **Algoritmos e Programação 2** — FTEC Faculdades.

Requisitos obrigatórios: **✅ Todos atendidos**

- [x] Structs
- [x] Condicionais
- [x] Laços
- [x] Vetores/Matrizes
- [x] Leitura/Escrita de arquivos

---

## Logs de Bugs Históricos (Referência)

| Bug                                      | Sintoma                         | Causa                                | Fix                                           |
| ---------------------------------------- | ------------------------------- | ------------------------------------ | --------------------------------------------- |
| Double Enter em combate                  | Exigia 2× Enter para ação       | `limpar_buffer()` no lugar errado    | Mover para após `scanf()`                     |
| Mapa reversava ao transicionar           | Voltava pra região anterior     | Missing `continue;` / indexação      | Adicionar `continue;` após carregar novo mapa |
| Baú não exibia mensagem                  | Baú sumia silenciosamente       | Não tinha `getchar()` antes de loop  | `fflush(stdout)` + `getchar()`                |
| Estado antigo persiste ao sobrescrever   | Save antigo não era apagado     | `.state` files não eram deletados    | `limpar_estados_slot()` antes de novo jogo    |
| Símbolo `@` duplicava ao voltar pro mapa | `@` aparecia 2× na tela         | `@` hardcoded no `.txt` do mapa      | Remover `@` e usar `.` no arquivo             |
| Transição carregava mapa errado          | Destino do arquivo sobrescrevia | Variáveis lidas diretamente do array | Copiar para locais antes de `carregar_mapa()` |

---

## Versionamento

- **Versão**: 1.0 (Week 4 Final)
- **Linguagem**: C (C99, stdlib apenas)
- **Plataforma**: Terminal (Windows/Linux/macOS)
- **Repositório**: https://github.com/aKynoS2/corrida_ao_planalto

---

> _Desenvolvido com C puro, café e uma quantidade irresponsável de `printf` + `getchar()`._ ☕💀

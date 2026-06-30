# 🗳️ Corrida ao Planalto

## RPG de terminal ambientado nas Eleições 2026

> _"Você não tem partido, não tem dinheiro, não tem jabá. Mas tem voto — e voto se conquista."_

---

```
 _______  _______  _______  _______ _________ ______   _______
(  ____ \(  ___  )(  ____ )(  ____ )\__   __/(  __  \ (  ___  )
| (    \/| (   ) || (    )|| (    )|   ) (   | (  \  )| (   ) |
| |      | |   | || (____)|| (____)|   | |   | |   ) || (___) |
| |      | |   | ||     __)|     __)   | |   | |   | ||  ___  |
| |      | |   | || (\ (   | (\ (      | |   | |   ) || (   ) |
| (____/\| (___) || ) \ \__| ) \ \_____) (___| (__/  )| )   ( |
(_______/(_______)|/   \__/|/   \__/\_______/(______/ |/     \|
          AO PLANALTO — uma campanha sem retorno
```

---

## Sobre o jogo

**Corrida ao Planalto** é um RPG de terminal desenvolvido em **C puro**, onde você interpreta um candidato independente tentando chegar à presidência do Brasil partindo do absoluto zero — sem partido, sem fundo eleitoral, sem assessores de imagem.

Você vai atravessar as 6 regiões do Brasil enfrentando lobistas, bots de redes sociais, coronéis e políticos veteranos no caminho até o confronto final em Brasília.

Cada região conquistada contribui à sua experiência política. Cada combate vencido melhora seus atributos. Cada baú aberto traz itens estratégicos.

---

## Status: Campanha Completa ✅

### Sistemas Implementados

| Sistema               | Status | Detalhes                                                                    |
| --------------------- | ------ | --------------------------------------------------------------------------- |
| ⚔️ Combate por turnos | ✅     | Tela dedicada, opções: atacar/usar item/fugir, cálculo de dano, ganho de XP |
| 🗺️ Movimentação       | ✅     | WASD, colisão, viewport 40×20 com câmera centralizada                       |
| 📈 Progressão         | ✅     | XP, level up automático com melhoria de stats                               |
| 🎒 Inventário         | ✅     | 20 slots, tela dedicada, listar e usar itens                                |
| 🗡️ Equipamentos       | ✅     | Armas e armaduras equipáveis com bônus de ataque/defesa                     |
| 🎁 Baús               | ✅     | Item aleatório do catálogo ao pisar                                         |
| 💾 Save/Load          | ✅     | 3 slots binários com serialização manual do grid                            |
| ⏸️ Pausa              | ✅     | Tecla `P`, menu continuar/salvar/sair                                       |
| 🌍 Mapas              | ✅     | 6 mapas temáticos completos com transições bidirecionais                    |
| 🚪 Transições         | ✅     | Múltiplas saídas por mapa com persistência de estado por slot               |
| 🎨 Cores              | ✅     | Cada símbolo com cor ANSI, renderização via buffer único                    |
| 🔤 Símbolos ASCII     | ✅     | 20 símbolos com comportamento via tabela                                    |
| 📋 Menus              | ✅     | ASCII art, cores, seleção de slot com nível e região                        |
| 📊 HUD                | ✅     | HP, XP, nível, mana e região visíveis durante o jogo                        |
| 💬 Buffer de Eventos  | ✅     | Fila FIFO de 3 mensagens exibidas no HUD e no combate                       |
| 🔒 Segurança          | ✅     | snprintf + strncpy com proteção de buffer overflow                          |

---

## Mapa da Campanha

```
        [NORTE]
        /      \
  [NORDESTE]  [CENTRO-OESTE] --- [BRASÍLIA/DF]
        \      /    \
        [SUDESTE]  [SUL]
              \    /
               (conexão)
```

| Região       | Tamanho | Tema Visual                                               |
| ------------ | ------- | --------------------------------------------------------- |
| Norte        | 60×25   | Amazônia — rio sinuoso, floresta densa                    |
| Nordeste     | 45×20   | Caatinga — árido, açude central, vegetação esparsa        |
| Centro-Oeste | 50×22   | Cerrado — rio vertical, pontes, muralha de Brasília       |
| Sudeste      | 35×18   | Mata Atlântica — densa, estruturas urbanas, rio diagonal  |
| Sul          | 30×15   | Campo gaúcho — vegetação compacta                         |
| Brasília/DF  | 27×14   | Mapa final — estruturas `&`, colunas `+`, confronto final |

---

## Controles

| Tecla   | Ação                  |
| ------- | --------------------- |
| `W`     | Mover para cima       |
| `A`     | Mover para esquerda   |
| `S`     | Mover para baixo      |
| `D`     | Mover para direita    |
| `I`     | Abrir inventário      |
| `P`     | Pausa                 |
| `Enter` | Confirmar / continuar |

---

## Símbolos do Mapa

| Símbolo         | Significado               |  Transitável   | Cor               |
| --------------- | ------------------------- | :------------: | ----------------- |
| `#`             | Parede/borda              |       ❌       | Branco            |
| `%`             | Parede vegetal            |       ❌       | Verde             |
| `T`             | Árvore/vegetação          |       ❌       | Verde             |
| `&`             | Estrutura urbana          |       ❌       | Cinza             |
| `=`             | Água (rio/lago)           |       ❌       | Azul              |
| `^`             | Montanha                  |       ❌       | Cinza claro       |
| `.`             | Chão aberto               |       ✅       | Branco            |
| `,`             | Grama                     |       ✅       | Verde claro       |
| `_`             | Estrada/ponte             |       ✅       | Amarelo           |
| `:`             | Terra seca                |       ✅       | Amarelo           |
| `'`             | Pedra solta               |       ✅       | Cinza             |
| `~`             | Raso (beira d'água)       |       ✅       | Ciano             |
| `E`             | Inimigo                   |  ✅ (combate)  | Vermelho          |
| `?`             | Baú                       |   ✅ (item)    | Magenta           |
| `>` `<` `v` `*` | Saídas direcionais        | ✅ (transição) | Amarelo brilhante |
| `+`             | Cidade/ponto de interesse |       ✅       | Amarelo           |
| `@`             | Jogador                   |       —        | Ciano brilhante   |

---

## Atributos do Personagem

| Atributo RPG | Equivalente político    | Valor inicial | Escala               |
| ------------ | ----------------------- | ------------- | -------------------- |
| ❤️ HP        | Reputação               | 20            | +5/nível             |
| 💰 Mana      | Verba de campanha       | 30            | —                    |
| 🗣️ Ataque    | Poder de convencimento  | 8             | +1/nível             |
| 🛡️ Defesa    | Resistência a escândalo | 3             | +1/nível             |
| 🗳️ XP        | Votos acumulados        | 0             | threshold: nível×100 |

---

## Catálogo de Inimigos

| #   | Nome                       | HP  | ATK | DEF | XP  |
| --- | -------------------------- | --- | --- | --- | --- |
| 0   | Lobista                    | 15  | 4   | 2   | 25  |
| 1   | Bot de Rede Social         | 10  | 6   | 1   | 20  |
| 2   | Cabo Eleitoral Corrompido  | 12  | 4   | 3   | 22  |
| 3   | Jornalista Sensacionalista | 18  | 7   | 1   | 35  |
| 4   | Político Veterano          | 25  | 5   | 6   | 50  |

---

## Catálogo de Itens

| #   | Nome              | Tipo     | Efeito                   |
| --- | ----------------- | -------- | ------------------------ |
| 0   | Santinho          | Poção    | Recupera 10 HP           |
| 1   | Horário Eleitoral | Arma     | +5 de ataque ao equipar  |
| 2   | Dossiê            | Poção    | Recupera 20 HP           |
| 3   | Caixa 2           | Arma     | +10 de ataque ao equipar |
| 4   | Apoio Popular     | Armadura | +3 de defesa ao equipar  |

---

## Como Compilar e Rodar

### Requisitos

- GCC ou compilador C compatível com C99
- Terminal com suporte a cores ANSI

### Build

```bash
gcc src/*.c -I./include -o jogo -Wall -Wextra -g -fno-strict-aliasing
```

### Executar

```bash
# Windows
jogo.exe

# Linux/macOS
./jogo
```

---

## Arquitetura do Código

```
src/
├── main.c          # inicialização + loop de menu + chamada rodar_jogo()
├── mapa.c/.h       # renderização, câmera, carregamento, estado, cores, movimento
├── combate.c/.h    # loop de combate por turnos + tela de combate
├── personagem.c/.h # structs, level up, inventário, tela de inventário, equipamentos
├── utils.c/.h      # menus, save/load, rodar_jogo(), HUD, buffer de mensagens
├── simbolos.c/.h   # tabela de 20 símbolos ASCII
├── inimigos.c/.h   # catálogo de 5 inimigos
└── itens.c/.h      # catálogo de 5 itens

data/
├── maps/           # 6 arquivos .txt de mapa
└── saves/          # .dat (save) + .state (estado do mapa por slot)
```

### Decisões de Design

**Grid Dinâmico**: `CELULA **grid` alocado via `malloc` — suporta mapas de qualquer tamanho.

**Serialização Manual**: `fwrite` de ponteiro salva endereço, não dados. Grid é serializado linha por linha no `.dat`.

**Renderização com buffer**: frame inteiro montado em `char buffer[32768]` com agrupamento de cores ANSI — um único `printf` por frame.

**Buffer de mensagens**: array global `char mensagens[3][80]` em `utils.c` — fila FIFO de eventos visíveis no HUD e na tela de combate. Qualquer módulo pode chamar `adicionar_mensagem()`.

**Equipamentos com slot único**: `arma_equipada` e `armadura_equipada` na struct `PERSONAGEM` guardam o índice do item equipado. Ao trocar, o bônus anterior é revertido antes de aplicar o novo.

**Telas dedicadas**: combate e inventário limpam a tela e exibem interface própria — sem sobreposição com o mapa.

**Catálogos em `.c`**: `tabela_simbolos[]`, `catalogo_inimigos[]` e `catalogo_itens[]` definidos em arquivos `.c` com `extern` nos `.h`.

**Dois arquivos de persistência por slot**:

- `.dat` — personagem + mapa atual completo
- `.state` — grid de cada mapa visitado (inimigos mortos, baús abertos)

**Transições metadata-driven**: saídas definidas no `.txt` após `---`, não hardcoded no código.

**Modularização**: `main.c` só inicializa e chama `rodar_jogo()`. Lógica de mapa em `mapa.c`, menus e fluxo em `utils.c`. Retornos por `int` eliminam `goto`.

**Segurança**: `snprintf` com sizeof para todos os paths, `strncpy` com null terminator garantido.

---

## Pendências Conhecidas (Post-Entrega)

- Sistema de aliados (planejado)
- Sistema de escândalos/flags (planejado)
- Boss final escalável (planejado)
- Input sem Enter/raw mode — fora do escopo

---

## Contexto Acadêmico

Projeto para disciplina **Algoritmos e Programação 2** — FTEC Faculdades.

Requisitos obrigatórios: **✅ Todos atendidos**

- [x] Structs | [x] Condicionais | [x] Laços | [x] Vetores/Matrizes | [x] I/O Arquivos

---

## Versionamento

- **Versão**: 1.4 (Week 5 — HUD + Equipamentos + Telas Dedicadas + Segurança)
- **Linguagem**: C (C99, stdlib apenas)
- **Plataforma**: Terminal (Windows/Linux/macOS)
- **Repositório**: https://github.com/aKynoS2/corrida_ao_planalto

---

> _Desenvolvido com C puro, café e uma quantidade irresponsável de `printf` + `getchar()`._ ☕

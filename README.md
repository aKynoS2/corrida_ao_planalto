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

Atravesse as 6 regiões do país enfrentando lobistas, bots de redes sociais, coronéis e políticos veteranos no caminho até o confronto final em Brasília.

Cada região que você conquista torna o boss final mais poderoso. Cada escândalo que você acumula corrói sua reputação. Cada aliado recrutado muda como o jogo funciona.

---

## Mecânicas

| Sistema               | Descrição                                                     |
| --------------------- | ------------------------------------------------------------- |
| ⚔️ Combate por turnos | Enfrente inimigos com stats únicos e comportamentos especiais |
| 🗺️ Exploração         | Navegue por mapas 2D com símbolos Unicode no terminal         |
| 📈 Progressão         | Ganhe XP, suba de nível e melhore seus atributos              |
| 🎒 Inventário         | Colete itens com efeitos políticos e estratégicos             |
| 🤝 Aliados            | Recrute até 2 aliados com bônus passivos únicos               |
| 💾 Save/Load          | Salve e carregue seu progresso em arquivo binário             |
| ⚡ Escândalos         | Ações desonestas acumulam flags que afetam o jogo             |

---

## Atributos do personagem

| Atributo RPG | Equivalente político    |
| ------------ | ----------------------- |
| ❤️ HP        | Reputação               |
| 💰 Mana      | Verba de campanha       |
| 🗣️ Ataque    | Poder de convencimento  |
| 🛡️ Defesa    | Resistência a escândalo |
| 🗳️ XP        | Votos acumulados        |

---

## Regiões

```c
[ NORTE ] ──► [ NORDESTE ] ──► [ CENTRO-OESTE ]
                                      │
                                      ▼
[ SUL ] ◄──── [ SUDESTE ] ◄────────────
                    │
                    ▼
              [ BRASÍLIA ] ← boss final
```

Ordem de exploração é **livre** — mas cada região completada escala o boss de Brasília.

| Região          | Boss                   |
| --------------- | ---------------------- |
| 🌿 Norte        | O Concessionário       |
| ☀️ Nordeste     | Coronel Zé das Quantas |
| 🌾 Centro-Oeste | O Senador Ruralista    |
| 🏙️ Sudeste      | Prefeito + Governador  |
| 🧉 Sul          | O Separatista          |
| 🏛️ Brasília     | O Candidato do Sistema |

---

## Inimigos

| Inimigo                       | Comportamento especial                |
| ----------------------------- | ------------------------------------- |
| 🕴️ Lobista                    | Drena verba (mana) por turno          |
| 📱 Bot de Rede Social         | Se multiplica se não eliminado rápido |
| 🎙️ Jornalista Sensacionalista | Ataque de área, reduz HP de aliados   |
| 🤡 Cabo Eleitoral Corrompido  | Aplica debuff "desinformação"         |
| 👴 Político Veterano          | Escudo de imunidade parlamentar       |

---

## Como rodar

### Requisitos

- GCC ou qualquer compilador C compatível com C99
- Terminal com suporte a Unicode e cores ANSI (Windows Terminal, qualquer terminal Linux/macOS)

### Compilar e rodar

```bash
git clone https://github.com/aKynoS2/corrida_ao_planalto.git
cd corrida-ao-planalto
gcc gcc src/*.c -I./include -o jogo -Wall -Wextra -g
jogo.exe (Windows)
./jogo (Linux)
```

### Controles

| Tecla           | Ação                  |
| --------------- | --------------------- |
| `W` `A` `S` `D` | Mover pelo mapa       |
| `1`             | Atacar no combate     |
| `2`             | Fugir do combate      |
| `Enter`         | Confirmar / continuar |

---

## Estrutura do projeto

```c
corrida-ao-planalto/
├── include/
│   ├── personagem.h    # structs: PERSONAGEM, INIMIGO, ITEM
│   ├── mapa.h          # structs: MAPA, CELULA + funções de mapa
│   ├── combate.h       # sistema de combate por turnos
│   ├── inimigos.h      # catálogo de inimigos predefinidos
│   ├── simbolos.h      # tabela de símbolos Unicode do mapa
│   └── utils.h         # utilitários: cores ANSI, limpar tela/buffer
├── src/
│   ├── main.c          # loop principal do jogo
│   ├── personagem.c    # lógica de personagem e level up
│   ├── combate.c       # loop de combate
│   ├── mapa.c          # renderização e navegação do mapa
│   └── utils.c         # funções utilitárias
└── saves/
    └── save1.dat       # arquivo de save binário
```

---

## Status do desenvolvimento

- [x] Mapa 2D com movimentação e colisão
- [x] Sistema de combate por turnos
- [x] XP e level up
- [x] Catálogo de inimigos com stats únicos
- [x] Símbolos Unicode no mapa
- [x] Cores ANSI por tipo de célula
- [x] Inventário e itens
- [x] Baús e recompensas
- [ ] Múltiplas regiões
- [ ] Sistema de aliados
- [ ] Sistema de escândalos
- [ ] Save/Load
- [ ] Boss final

---

## Contexto acadêmico

Projeto desenvolvido para a disciplina de **Algoritmos e Programação 2**.
Implementa obrigatoriamente: structs, condicionais, laços, vetores, matrizes e leitura/escrita em arquivos.

---

> _Desenvolvido com C puro, café e uma quantidade irresponsável de `printf`._

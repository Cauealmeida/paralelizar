# Paralelização do Conjunto de Mandelbrot com OpenMP

Grupo:
- Cauê de Almeida Gonçalves
- Kayke Guilherme Vertu
- Thais Larriane Guimarães Barbosa

Aplicação: <https://github.com/hpc-fci-mackenzie/mpp/blob/master/17/mandelbrot.zip>

## Compilação e execução

Compilar:

- Versão Sequêncial:
  - Compilar o projeto: 
    ```sh
    make seq
    ```
  
  - Gerar as saídas 5 vezes:
    ```sh
    ./executar.sh seq.csv 5 "cat mandelbrot.in | ./seq"
    ```

Versão Paralela:

-- A FAZER.

## Descrição do problema e análise do código sequencial
O programa irá gerar uma imagem em texto do conjunto de Mandelbrot, ele é formado pelos números complexos c para os quais a sequência z = z² + c não diverge.

Cada pixel da imagem (230 linhas × 790 colunas) representa um ponto do plano complexo. O programa executa a fórmula até z passar de 2 ou até atingir 24.000 iterações. Pontos do conjunto são marcados com "#", e os demais com "." O cálculo ocupa [inserir tempo]% do tempo de execução ([ inserir tempo ] s). 

O código tem três etapas: leitura da entrada e alocação da matriz, cálculo de todos os pixels e escrita da matriz na saída. Medindo cada etapa separadamente, o cálculo ocupa [ Inserir tempo ]% do tempo total ([inserir tempo] s), enquanto a leitura e a escrita juntas levam menos de [ inserir tempo ] ms. O custo computacional está, portanto, quase todo no laço de cálculo.

A leitura e a escrita são descartáveis. Os pixels são independentes entre si, o que deixa possível paralelizar o cálculo. Porém, as iterações de um mesmo pixel são sequenciais, já que os valores dependem dos anteriores. O custo por pixel varia muito: cerca de 68.483 dos 181.700 pixels (38%) dos pixels pertencem ao conjunto e executam as 24.000 iterações, enquanto os demais terminam em poucas iterações. Esses pixels custosos estão nas linhas centrais, causando desbalanceamento de carga entre as threads.

## Estratégia de paralelização

O trabalho foi dividido por linhas da imagem. O laço externo, que percorre as linhas, foi paralelizado com OpenMP, de modo que cada thread calcula um conjunto de linhas completas:

```c
#pragma omp parallel for schedule(static ou guided ou dynamic)
for (int r = 0; r < max_row; ++r) {
    for (int c = 0; c < max_column; ++c) {
        // cálculo do pixel (r, c), igual ao código sequencial
    }
}
```

Como dito anteriormente do laço externo cada pixel é calculado de forma independente: o resultado de um pixel não depende do outro. Em contrapartida O laço while, que faz as iterações de cada pixel, não pode ser paralelizado, porque cada novo valor de z depende do valor anterior.

Não há condição de corrida no cálculo. Pois, cada thread escreve nas posições da matriz que das suas linhas, e nenhuma thread lê o que outra escreveu. Por isso, não foi necessário nenhum lock.

As variáveis declaradas dentro do laço (c, z e n) são privadas de cada thread. A matriz mat e os parâmetros da entrada (max_row, max_column e max_n) são compartilhados.

Como visto em aula no nosso projeto a única sincronização é a barreira implícita no fim do parallel for: o programa espera todas as threads terminarem antes de escrever a imagem na saída.

O custo das linhas é muito desigual: as linhas centrais contêm a maior parte dos pontos do conjunto, que executam as 24.000 iterações, enquanto as linhas das bordas terminam rápido. Por este motivo, a forma de dividir as linhas entre as threads tem grande impacto no desempenho.

Foi usado para fins de relatório e estudo:
  - `schedule(static)`: divide as linhas em blocos contíguos, um por thread. As threads que recebem as linhas centrais ficam com quase todo o trabalho, e as outras terminam cedo e ficam ociosas.

  - `schedule(static, 1)`: distribui as linhas de forma intercalada (linha 0 para a thread 0, linha 1 para a thread 1, e assim por diante). As linhas caras ficam espalhadas entre todas as threads, sem custo extra de escalonamento.

  - `schedule(dynamic, [chunk])`: cada thread pega um novo bloco de linhas quando termina o anterior. Adapta-se ao custo real de cada linha, com um pequeno custo de escalonamento.

  - `schedule(guided)`: semelhante ao dynamic, mas começa com blocos grandes e diminui o tamanho ao longo da execução.


E analisando escolhemos [inserir aqui]

## Ambiente de testes

- CPU: AMD Ryzen 9 5900X
- Núcleos: 12
- Memória: 16GB
- Sistema operacional: CachyOS x86_64
- Compilador: GCC 16.2.1
- Flags: `-O3 -std=c++11 -fopenmp`

## Metodologia

- Entrada: `mandelbrot.in` (230 linhas, 790 colunas, 24.000 iterações)
- Número de execuções: [ inserir aqui quantidade de execuções ]
- [Como os tempos foram medidos e como foram calculados a média e o desvio padrão.]

## Resultados

[Tempo, speedup e eficiência para 1, 2, 4 e 8 threads.]


## Discussão dos resultados e limitações

[Lei de Amdahl, gargalos, overhead e limitações encontradas.]

## Contribuição de cada membro

- [Nome 1]: [ ]
- [Nome 2]: [ ]
- [Nome 3]: [ ]

## Uso de LLM

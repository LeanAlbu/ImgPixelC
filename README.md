# Processador de Imagens PPM: Efeito Pixel Art

Um programa em C desenvolvido para realizar o processamento de imagens brutas no formato PPM (P6 - Binário). O algoritmo aplica um efeito de mosaico (pixel art) através da técnica de downsampling, calculando a média de cores em blocos dinâmicos baseados na resolução original da imagem.

O projeto foi estruturado com foco em manipulação direta de memória e alocação dinâmica de arrays unidimensionais, servindo como projeto base para futura implementação de processamento paralelo.

## Funcionalidades

* Leitura e validação de cabeçalhos de arquivos PPM (Portable Pixmap).
* Alocação dinâmica de memória para matrizes de pixels utilizando structs.
* Cálculo de tamanho de bloco dinâmico e autoajustável com base na largura da imagem.
* Processamento de imagem em memória (Downsampling/Mosaico).
* Geração de um novo arquivo binário P6 com a imagem processada.

## Como o Algoritmo Funciona

1. O arquivo `.ppm` é lido e seus metadados (largura, altura e cor máxima) são extraídos.
2. Os dados binários dos pixels (RGB) são carregados para um array linear (1D) na memória.
3. O algoritmo divide a imagem em blocos (quadrantes).
4. Para cada bloco, é calculada a média aritmética dos valores de vermelho, verde e azul de todos os pixels internos.
5. Todos os pixels desse bloco são sobrescritos com a cor média calculada, gerando o aspecto "pixelado".
6. O array resultante é gravado em um novo arquivo de saída.

## Evolução Futura (Roadmap)

Este código foi propositalmente projetado para não depender de bibliotecas externas complexas (como stb_image). A estrutura de alocação de matriz linear e loops independentes foi pensada para facilitar a implementação de paralelismo de dados.

* Implementação de multithreading para dividir o processamento dos blocos da imagem entre os núcleos da CPU.
* Otimização de leitura e escrita em disco.

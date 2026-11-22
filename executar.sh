#!/bin/bash

# Verifica os parâmetros
if [ "$#" -lt 3 ]; then
    echo "Uso: $0 <arquivo de saída> <quantidade> <comando> [argumentos...]"
    exit 1
fi

SAIDA=$1
QUANTIDADE=$2
# Ignore os primeiro dois parâmetros a partir de agora
shift
shift

# Limpa o arquivo de saída e preenche com apenas o cabeçalho
echo leitura_s,calculo_s,escrita_s,total_s > "$SAIDA"
echo "Executando $@..."

# Executa o programa a quantidade de vezes especificada
for ((i = 1; i <= QUANTIDADE; i++)); do
    echo "===== Execução $i/$QUANTIDADE ====="

    bash -c "$@" | tee -a "$SAIDA"
done

echo "Resultados salvos em $SAIDA"

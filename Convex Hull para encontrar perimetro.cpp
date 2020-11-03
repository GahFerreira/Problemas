/// O problema em questão é este: https://www.urionlinejudge.com.br/judge/en/problems/view/1982
/// Poderia ser melhor, mas honestamente ainda estou aprendendo sobre e esse resolve.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Ponto
{
    int x;
    int y;
};
typedef struct Ponto tPonto;

int max (int a, int b)
{
    if (a > b) return a;
    else return b;
}

int min (int a, int b)
{
    if (a < b) return a;
    else return b;
}

int ponto_colinear_no_segmento (tPonto p1, tPonto p2, tPonto q)
{
    if (q.x >= min(p1.x, p2.x) && q.x <= max(p1.x, p2.x) &&
        q.y >= min(p1.y, p2.y) && q.y <= max(p1.y, p2.y))
            return 1;

    return 0;
}

int orient (tPonto p1, tPonto p2, tPonto p3)
{
    int a = (p2.y - p1.y) * (p3.x - p2.x) - (p3.y - p2.y) * (p2.x - p1.x);

    if (a == 0) return 0;
    if (a < 0) return -1;
    /*if (a > 0)*/ return 1;
}

int intersecta (tPonto p1, tPonto p2, tPonto q1, tPonto q2)
{
    int o1, o2, o3, o4;

    o1 = orient(p1, p2, q1);
    o2 = orient(p1, p2, q2);
    o3 = orient(q1, q2, p1);
    o4 = orient(q1, q2, p2);

    if ((o1 != o2) && (o3 != o4)) return 1;

    if (o1 == 0 && ponto_colinear_no_segmento(p1, p2, q1) == 1) return 1;
    if (o2 == 0 && ponto_colinear_no_segmento(p1, p2, q2) == 1) return 1;
    if (o3 == 0 && ponto_colinear_no_segmento(q1, q2, p1) == 1) return 1;
    if (o4 == 0 && ponto_colinear_no_segmento(q1, q2, p2) == 1) return 1;

    return 0;
}

int cmpfunc (const void * a, const void * b)
{
    int pri = ((tPonto *)a)->x;
    int seg = ((tPonto *)b)->x;

   return (pri-seg);
}

double distance (tPonto p1, tPonto p2)
{
    int catA, catB;
    double result;

    catA = (p2.x - p1.x);
    catB = (p2.y - p1.y);

    result = sqrt ((catA * catA) + (catB * catB));

    return result;
}

tPonto *convex_hull (tPonto *ponto, int size)
{
    int i, j, k = 0, or, p, q;  // 'i, j' sao contadores. 'k' marca quantos pontos estão no hull. 'or' salva orientacao.
    tPonto *hull;               // 'p, q' salvam a posicao dos pontos no vetor 'ponto[size]'. 'hull' eh o vetor resposta.

    hull = (tPonto *) calloc (size+1, sizeof(tPonto));

    for (i = 0; i < size+1; i++)    // Isso eh util para saber onde termina o vetor 'hull'.
    {                               // No problema em questao, o minimo para 'x' era -2000. 
        hull[i].x = -10000;
    }

    p = 0;  // 'p' comeca valendo 0. Isso porque o vetor 'ponto[size]' esta ordenado da esquerda para a direita'.
    hull[k++] = ponto[0];   // E o ponto mais a esquerda sempre esta no hull.

    do  // Procuramos a partir de um ponto fixo 'p', um ponto 'q' que qualquer ponto 'r' nao esteja a esquerda de 'pq'.
    {
        q = (p+1)%size; // 'q' eh o proximo do vetor 'ponto[size]' em relacao a 'p'. (Arbitrario, mas 'r' nao pode ser igual a 'p').

        for (i = 0, j = 0; i < size-1; i++, j++) // Para cada ponto nao 'p'... (obs: 'i' vai ate 'size'-1 pois nao analisamos 'p'.
        {
            or = orient(ponto[p], ponto[q], ponto[(q+j)%size]); // ...analisamos orient(p, q, (todos os pontos nao analisados))

            if (or == 1)    // Se o ponto nao analisado 'r' estiver a direita de 'pq', entao 'q' esta a esquerda de 'pr'.
            {
                q = (q+j)%size; // Logo nosso proximo 'q' sera 'r', e analisaremos todos os pontos ainda nao analisados 
                j = 0;  // com orient(p, r, (todos os pontos ainda nao analisados))
            }   // 'j' volta pra 0 pra garantir que o proximo ponto a ser analisado sera o prox do vetor em relacao a 'r'.
        }   // Nao usamos 'i' pois este serve para limitar que facamos 'size' iteracoes, uma por ponto nao analisado.

        hull[k++] = ponto[q]; // Por fim, 'q' eh o ponto que para qualquer orient(p, q, r), nao ha 'r' a esquerda.
        p = q;  // Isso pode ocorrer para mais de um ponto, mas nesse caso sao colineares e nao importa para o problema.

    } while (hull[0].x != ponto[p].x || hull[0].y != ponto[p].y);   // Repetimos ate que o 'q' encontrado seja 'hull[0]'.

    return hull;
}

int main()
{
    int n, i, j, cont;
    double dist;

    while (1)
    {
        scanf("%d", &n);

        if (n == 0) break;

        tPonto ponto[n], ponto2[n];
        tPonto *hull;
        dist = 0;

        for (i = 0; i < n; i++)
        {
            scanf("%d%d", &ponto[i].x, &ponto[i].y);
        }

        qsort(ponto, n, sizeof(tPonto), cmpfunc);   // Para achar o ponto mais a esquerda facilmente.

        for (i = 0; i < n; i++)
        {
            ponto2[i].x = -10000;
            ponto2[i].y = -10000;
        }

        cont = 0;

        for (i = 0; i < n; i++) // Isso serve para remover pontos iguais (quebra o algoritmo do jeito que esta).
        {
            for (j = 0; j < n; j++)
            {
                if (ponto[i].x == ponto2[j].x && ponto[i].y == ponto2[j].y) break;
            }

            if (j == n) ponto2[cont++] = ponto[i];
        }

        hull = convex_hull(ponto2, cont);

        for (i = 0; i < n && hull[i].x != -10000; i++)
        {
            if (hull[i+1].x != -10000)
            {
                dist+= distance (hull[i], hull[i+1]);
            }
        }

        printf("Tera que comprar uma fita de tamanho %.2lf.\n", dist);
    }

    return 0;
}

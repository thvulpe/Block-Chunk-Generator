TASK 1

Verific daca block-ul se afla in dimensiunile chunk-ului si, in caz afirmativ, actualizez chunk-ul.

TASK 2

Ma asigur de ordinea crescatoare a coordonatelor, dupa care le parcurg si actualizez chunk-ul.

TASK 3

Asemanator functiei de la task 2, calculez colturile unui cuboid cu latura de 2 * raza, in jurul centrului sferei.
Apoi, un block se afla in interiorul sferei numai si numai daca distanta de la centru la acesta este strict mai mica decat raza.

TASK 4

Folosesc vectori de coordonate pentru a trece prin vecinii fiecarui block.
Un caz particular il reprezinta situatia in care target_block coincide cu shell_block, pentru care folosesc un tablou tridimensional ce retine block-urile nou plasate,
evitand astfel luarea in considerare a acestora in construirea shell-ului.

TASK 5

Am implementat un algoritm de tip Fill, folosind vectori de coordonate, un tablou bidimensional pentru vizite (dimensiunea Y este irelevanta) si o structura ce retine coordonatele
X si Z. Adaug block-ul de la coordonatele transmise ca parametri in coada, dupa care adaug in coada si parcurg toate block-urile adiacente care respecta conditiile.

TASK 6

Asemanator task-ului 5, cu exceptia faptului ca tin cont si de dimensiunea Y.

TASK 7

Aloc memorie pentru un chunk auxiliar, care devine chunk-ul initial, rotit la 90 de grade. Pentru rotire am folosit urmatoarea relatie: (x,y,z) devine (z,y,width-x-1).
Intrucat returnez chunk-ul creat de mine, eliberez memoria pentru chunk-ul primit ca parametru.

TASK 8

Pentru acest task, am folosit mai multe functii auxiliare:

get_body_at - intoarce un corp format din block-uri de acelasi tip sub forma unui tablou tridimensional (un chunk cu toate celelalte block-uri eliminate), folosind un algoritm
de tip Fill, asemanator celui de la task 5 si 6.

get_mixed_body_at - acelasi lucru ca get_body_at, cu exceptia faptului ca orice block-uri adiacente formeaza un corp, indiferent daca sunt de acelasi tip sau nu

is_touching_ground - ia ca parametru un corp si verifica daca "atinge pamantul", adica daca vreunul din block-urile sale are dedesubt un block care nu face parte din
corpul respectiv

lower_body - ia ca parametru un corp si un chunk si deplaseaza corpul cu o pozitie in jos pe axa Oy

Mod de functionare: Corpurile sunt cautate la fiecare block din chunk. Deplasez, prima data, toate corpurile formate din block-uri mixte, ca solutie la situatia in care doua
corpuri diferite, considerate uniforme din punct de vedere al tipurilor de block-uri, se considera a "atinge pamantul" din cauza faptului ca sunt in coliziune. Apoi, deplasez
in jos si corpurile "uniforme", ajungand la rezultatul dorit.

TASK 9

Am implementat o functie care encodeaza fiecare run, folosind operatii pe biti, in functie de tipul de block si de numarul de aparitii consecutive. Run-urile care trebuie encodate
le obtin prin aflarea secventelor de block-uri identice din chunk.

TASK 10

Folosind mai multe functii care decodifica fiecare caracteristica a run-ului, construiesc noul chunk.

(* Autor: Marek Puzyna 371359, Recenzent: Piotr Haryza *)                      

(* DEFINICJE TYPÓW *)
(* Typ "punkt", przedstawiający punkt na płaszczyźnie 
   jako jego współrzędne (x, y) typu float *)
type point = float * float

(* Typ "kartka", przedstawiony jako funkcja, która przyjmuje punkt,
   a zwraca ilość przebitych w tym punkcie złożonych kartek *)
type kartka = point -> int

(* FUNKCJE POMOCNICZE *)
(* Funkcja porównywawcza dla float-ów *)
let (===) x y = abs_float (x -. y) < 1e-10

(* Liczy wyznacznik macierzy 3x3, zadanej trójką trójek *)
let det3 ((a11, a12, a13), (a21, a22, a23), (a31, a32, a33)) =
   a11 *. a22 *. a33 +. a12 *. a23 *. a31 +. a13 *. a21 *. a32 -.
   a31 *. a22 *. a13 -. a32 *. a23 *. a11 -. a33 *. a21 *. a12

(* Przerzuca punkt (x, y) symetrycznie względem prostej, 
   przechodzącej przez (pz, py) i (qx, qy) *)
let rzut (px, py) (qx, qy) (x, y) =
   let k = (py -. qy) /. (px -. qx)
   and b = (px *. qy -. qx *. py) /. (px -. qx) in
   if classify_float k = FP_normal || classify_float k = FP_subnormal then
      let dx = 2. *. (y *. k +. x -. b *. k) /. (k *. k +. 1.) -. 2. *. x in
      (x +. dx, y -. k *. dx)
   else if classify_float k = FP_zero then (x, 2. *. b -. y)
   else (2. *. px -. x, y)

(* FUNKCJE WŁAŚCIWE *)	
(* Funkcja, która dla punktów p i q zwraca prostokątną domknietą kartkę,
   lewym dolnym wierzchołkiem której jest p, a prawym górnym - q *)
let prostokat (px, py) (qx, qy) = if px <= qx && py <= qy
      then function ((x, y):point) -> 
         if x <= qx && x >= px && y <= qy && y >= py
            then 1 else 0
      else invalid_arg "points"

(* Funkcja, kra dla punktu p i liczby zmiennoprzecinkowej f zwraca 
   okrągłą, domkniętą kartke ze środkiem w p i o promieniu f *)
let kolko (px, py) f = fun (x, y) -> if f < 0. 
   then invalid_arg "float" else
   if ((px -. x) ** 2.) +. ((py -. y) ** 2.) <= f ** 2.
      then 1 else 0

(* Funkcja, która składa kartkę "k" wzdłuż prostej,
   przechodzącej przez punkty "p" i "q" *)
let zloz (px, py) (qx, qy) (k:kartka) = 
   if px === qx && py === qy then invalid_arg "points" else
   fun (x, y) -> 
      let det = det3 ((x, y, 1.), (px, py, 1.), (qx, qy, 1.)) in
	  if det === 0. then k (x, y)
      else if det < 0. then 0
	  else k (x, y) + k (rzut (px, py) (qx, qy) (x, y))

(* Funkcja, która składa kartkę "k" wzdłuż prostych,
   zadanych parami punktów z zadanej listy "l" *)
let skladaj l k = List.fold_left (fun k (p, q) -> zloz p q k) k l
   
(* TESTY *)
let p = (1., 2.)
let q = (3., 4.)
let sk1 = skladaj [(1., 1.), (-1., -1.); (-1., 1.), (1., -1.)]
let sk2 = skladaj [(0., 1.), (-1., 0.); (1., 0.), (0., 1.); 
   (0., -1.), (1., 0.); (-1., 0.), (0., -1.)]
let sk3 = skladaj [(0.5, -0.5), (0.5, 0.5); (0.5, 0.5), (-0.5, 0.5); 
   (-0.5, 0.5), (-0.5, -0.5); (-0.5, -0.5), (0.5, -0.5)] 
let sk4 = skladaj [(9., 0.), (9., 1.); (8., 0.), (8., 1.); (7., 0.), (7., 1.);
   (6., 0.), (6., 1.); (5., 0.), (5., 1.); (4., 0.), (4., 1.);
   (3., 0.), (3., 1.); (2., 0.), (2., 1.); (1., 0.), (1., 1.)] 
let _ =
assert(prostokat p q (2., 3.) = 1);
assert(prostokat p q p = 1);
assert(prostokat p q (0., 0.) = 0);
assert(kolko p 5. q = 1);
assert(kolko p 5. (-5., -5.) = 0);
assert(zloz (1., 1.) (5., 5.) (prostokat p q) (1., 4.) = 1);
assert(zloz (1., 1.) (5., 5.) (prostokat p q) (2., 3.) = 2);
assert(sk1 (prostokat (-1., -1.) (1., 1.)) (1., 0.) = 4);
assert(sk1 (kolko (0., 0.) 1.) (1., 0.) = 4);
assert(sk1 (prostokat (-2., -2.) (2., 2.)) (0., 0.) = 1);
assert(sk3 (sk2 (kolko (0., 0.) 2.)) (0., 0.) = 13);
assert(sk4 (prostokat (0., 0.) (10., 2.)) (0.5, 0.5) = 10);
(*-------------------------------------------*)
assert(sk3 (sk2 (prostokat (-1., -1.) (1., 1.))) (0., 0.) = 9);
assert(sk3 (sk2 (prostokat (-1., -1.) (1., 1.))) (0.5, 0.5) = 1);
assert(sk3 (sk2 (prostokat (-1., -1.) (1., 1.))) (0.25, 0.) = 6);
assert(sk3 (sk2 (prostokat (-1., -1.) (1., 1.))) (0.5, 0.) = 3);
assert(sk3 (sk2 (prostokat (-1., -1.) (1., 1.))) (0.25, 0.25) = 4);
(*-------------------------------------------*)
assert(sk3 (prostokat (-10., -10.) (10., 10.)) (0., 0.) = 16);
assert(sk3 (prostokat (-10., -10.) (10., 10.)) (1., 1.) = 4);
assert(sk3 (prostokat (-10., -10.) (10., 10.)) (0., 1.) = 8);
assert(sk3 (prostokat (-10., -10.) (10., 10.)) (9., 9.) = 1);
(*-------------------------------------------*)
assert(sk3 (kolko (0., 0.) 1.) (0., 0.) = 5);
assert(sk3 (kolko (0., 0.) 1.) (0.2, 0.2) = 3);
assert(sk3 (kolko (0., 0.) 1.) (0., 0.25) = 2);
assert(sk3 (kolko (0., 0.) 1.) (0.45, 0.45) = 4);
(*-------------------------------------------*)
assert(sk4 (kolko (5., 5.) 5.) (0.5, 5.) = 10);
assert(sk4 (kolko (5., 5.) 5.) (0.5, 8.) = 8);
assert(sk4 (kolko (5., 5.) 5.) (0.5, 9.) = 6);
assert(sk4 (kolko (5., 5.) 5.) (0.5, 9.5) = 4);
assert(sk4 (kolko (5., 5.) 5.) (0.75, 9.75) = 2);
assert(sk4 (kolko (5., 5.) 5.) (0., 5.) = 6)

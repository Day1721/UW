(* Autor: Marek Puzyna, Recenzent: Przemysław Kozłowski *)
(* Przy tworzeniu danego programu kożystałem z PSet *)
(** Interval Set.

    This is an interval set, i.e. a set of integers, where large
    intervals can be stored as single elements. Intervals stored in the
    set are disjoint.

*)

(* Typ drzewa przedziałowego *)
type t = 
   | Empty
   | Set of t * (int * int) * t * int * Nativeint.t

(* Puste drzewo *)
let empty = Empty

(* Aliasy dla działań na nativeint-ach *)
let (++) = Nativeint.add
let (--) = Nativeint.sub
let of_int = Nativeint.of_int

(* Predykat, sprawdzający czy drzewo jest puste *)
let is_empty set =
   set = Empty

(* Zwraca wysokość drzewa *)
let height = function
   | Empty -> 0
   | Set (_, _, _, h, _) -> h

(* Zwraca liczbę elementów w drzewie *)
let size = function
   | Empty -> Nativeint.zero
   | Set (_, _, _, _, s) -> s

(* Tworzy drzewo z kożeniem "(x, y)" i poddrzewami "l" i "r" *)
let make l (x, y) r =
   let str = (size l) ++ (size r) ++ (of_int y) -- (of_int x) in
   if str <> ((of_int max_int) -- (of_int min_int)) then
      Set (l, (x, y), r, max (height l) (height r) + 1, str ++ Nativeint.one)
   else Set (l, (x, y), r, max (height l) (height r) + 1, str)
 
(* Funkcja balansująca drzewo o kożeniu "i" i poddrzewach "l" i "r" *)
let bal l i r =
   let hl = height l in
   let hr = height r in
   if hl > hr + 2 then
      match l with
         | Empty -> assert false
         | Set (ll, li, lr, _, _) ->
            if height ll >= height lr then
               make ll li (make lr i r)
            else match lr with
               | Empty -> assert false
               | Set (lrl, lri, lrr, _, _) ->
                  make (make ll li lrl) lri (make lrr i r)
   else if hr > hl + 2 then
      match r with
         | Empty -> assert false
         | Set (rl, ri, rr, _, _) ->
            if height rr >= height rl then
               make (make l i rl) ri rr
            else match rl with
               | Empty -> assert false
               | Set (rll, rli, rlr, _, _) ->
                  make (make l i rll) rli (make rlr ri rr)
   else make l i r

(* Funkcja balansująca dane drzewo *)
let balance = function
   | Empty -> Empty
   | Set (l, i, r, _, _) -> bal l i r

(* Zwraca najmniejszy (w sensie <=) przedział w drzewie *)
let rec min_elt = function
   | Set (Empty, i, _, _, _) -> i
   | Set (l, _, _, _, _) -> min_elt l
   | Empty -> raise Not_found
  
(* Zwraca największy (w sensie <=) przedział w drzewie *)
let rec max_elt = function
   | Set (_, i, Empty, _, _) -> i
   | Set (_, _, r, _, _) -> max_elt r
   | Empty -> raise Not_found

(* Usuwa najmniejszy (w sensie <=) przedział z drzewa *)
let rec remove_min_elt = function
   | Set (Empty, _, r, _, _) -> r
   | Set (l, i, r, _, _) -> bal (remove_min_elt l) i r
   | Empty -> invalid_arg "PSet.remove_min_elt"

(* Usuwa najnajwiększy (w sensie <=) przedział z drzewa *)
let rec remove_max_elt = function
   | Set (l, _, Empty, _, _) -> l
   | Set (l, i, r, _, _) -> bal l i (remove_max_elt r)
   | Empty -> invalid_arg "PSet.remove_min_elt"

(* Łączy dwa drzewa *)
let merge t1 t2 =
   match t1, t2 with
   | Empty, _ -> t2
   | _, Empty -> t1
   | _ ->
      let i = min_elt t2 in
      bal t1 i (remove_min_elt t2)

(* Usuwa dany przedział z danego drzewa *)
let remove (a, b) set =
   let rec loopl = function
      | Empty -> Empty
      | Set (l, (x, y), r, _, _) ->
         if a <= x then loopl l else
         if a > y then bal l (x, y) (loopl r) else
         bal l (x, a - 1) empty
   and loopr = function
      | Empty -> Empty
      | Set (l, (x, y), r, _, _) ->
         if b >= y then loopr r else
         if b < x then bal (loopr l) (x, y) r else
         bal empty (b + 1, y) r in
   let rec loop = function
      | Empty -> Empty
      | Set (l, (x, y), r, _, _) ->
         if b < x then bal (loop l) (x, y) r else
         if a > y then bal l (x, y) (loop r) else
         if a <= x && y <= b then balance (merge (loopl l) (loopr r)) else
         if a <= x then bal (loop l) (b + 1, y) r else
         if b >= y then bal l (x, a - 1) (loop r) else
         merge (merge l (make Empty (x, a - 1) Empty))
         (merge (make Empty (b + 1, y) Empty) r) in
   loop set

(* Sumuje 2 przedziały z niepustym przecięcięm *)
let sum (a, b) (c, d) =
   (min a c, max b d)

(* Dodaje dany przedział do danego drzewa *)
let rec add (a, b) = function
   | Empty -> make Empty (a, b) Empty
   | Set (l, (x, y), r, _, _) ->
      let rml = remove (a, b) l
      and rmr = remove (a, b) r 
      and (x', y') = sum (a, b) (x, y) in
      let pnt = ref (x', y') in
      if b <> max_int && x > 1 + b then
         let nl = add (a, b) l in
         bal nl (x, y) r
      else if y <> max_int && a > y + 1 then
         let nr = add (a, b) r in
         bal l (x, y) nr
      else if rml <> empty && rmr <> empty then
         let (lx, ly) = max_elt rml
         and (rx, ry) = min_elt rmr
         and pntl = ref rml
         and pntr = ref rmr in
         if ly + 1 = x' then begin
            pntl := remove_max_elt !pntl;
            pnt := (lx, y') end;
         if rx = y' + 1 then begin
            pntr := remove_min_elt !pntr;
            pnt := (fst !pnt, ry) end;
         bal !pntl !pnt !pntr
      else if rml <> empty then
         let (lx, ly) = max_elt rml
         and pntl = ref rml in
         if ly + 1 = x' then begin
            pntl := remove_max_elt !pntl;
            pnt := (lx, y') end;
         bal !pntl !pnt rmr
      else if rmr <> empty then
         let (rx, ry) = min_elt rmr
         and pntr = ref rmr in
         if rx = y' + 1 then begin
            pntr := remove_min_elt !pntr;
            pnt := (fst !pnt, ry) end;
         bal rml !pnt !pntr
      else make empty !pnt empty

(* Sprawdza, czy dana liczba jest w danym drzewie *)
let mem a set =
   let rec loop = function
      | Empty -> false
      | Set (l, (x, y), r, _, _) ->
         let c = a >= x && a <= y in
         c || loop (if a < x then l else r) in
   loop set

(* Iteruje daną funkcję po wszystkich elementach drzewa *)
let iter f set =
   let rec loop = function
      | Empty -> ()
      | Set (l, i, r, _, _) -> loop l; f i; loop r in
   loop set

(* Funkcja "fold" dla drzew *)
let fold f set a =
   let rec loop a = function
      | Empty -> a
      | Set (l, i, r, _, _) ->
         loop (f i (loop a l)) r in
   loop a set

(* Rzutuje dane drzewo na listę przedziałów *)
let elements set =
   let rec loop li = function
      | Empty -> li
      | Set (l, i, r, _, _) -> loop (i::loop li r) l in
   loop [] set

(* Zwraca liczbe elementów w drzewie, nie większych od danej *)
let below a set =
   let rec loop acc = function
      | Empty -> acc
      | Set (l, (x, y), r, _, s) ->
         if a < x then loop acc l
         else if a > y then loop (acc ++ s -- size r) r
         else let ss = size l ++ acc ++ (of_int a) -- (of_int x) in
            if ss <> ((of_int max_int) -- (of_int min_int)) 
               then ss ++ Nativeint.one else ss in
   let acc = loop Nativeint.zero set in
   let c = Nativeint.compare acc (of_int max_int) in
   if c = (-1) then Nativeint.to_int acc else max_int

(* łączy dwa drzewa i przedział "i", jednocześnie balansując drzewo *)
let rec join l i r =
   match l, r with
   | Empty, _ -> add i r
   | _, Empty -> add i l
   | Set (ll, li, lr, lh, ls), Set (rl, ri, rr, rh, rs) ->
      if lh > rh + 2 then bal ll li (join lr i r) else
      if rh > lh + 2 then bal (join l i rl) ri rr else
      make l i r

(* Dla danej liczby "a" zwraca trójkę, pierwszym elementem której jest zbiór
   liczb mniejszych od "a", trzecim - zbiór liczb większych od "a", drugim -
   wartość logiczną - czy a występuje w danym drzewie *)
let split a set =
   let rec loop = function
      | Empty -> Empty, false, Empty
      | Set (l, (x, y), r, _, _) ->
         if a < x then
            let ll, b, lr = loop l in
            ll, b, join lr (x, y) r else
         if a > y then
            let rl, b, rr = loop r in
            join l (x, y) rl, b, rr else
         if a = x then
            if x <> y then l, true, add (x + 1, y) r else
            l, true, r else
         if a = y then
            if x <> y then add (x, y - 1) l, true, r else
            l, true, r else
         (add (x, a - 1) l), true, (add (a + 1, y) r) in
   loop set
   
(* TESTY *)
(*
let _ =
let first  (a,_,_) = a in
let second (_,b,_) = b in
let third  (_,_,c) = c in
let a = ref empty in
assert (elements !a = []);
assert (is_empty !a);
a := add (0,0) !a;
assert (not (is_empty !a));
a := remove (0,0) !a;
assert (is_empty !a);
a := add (1,1) !a;
a := add (2,2) !a;
assert (is_empty (first (split 1 !a)));
assert (second (split 1 !a));
assert (not (second (split 3 !a)));
assert (is_empty (third (split 3 !a)));
a := add (3,3) !a;
assert(mem 1 !a);
assert (mem 2 !a);
assert (mem 3 !a);
assert (not (mem 4 !a));
assert (not (mem 0 !a));
assert (below 42 !a = 3);
let b = ref !a in
b := remove (2,2) !a;
b := add (1,3) !b;
assert (!b = !a);
assert (List.length (elements !b) = 1);
b := remove (2,2) !b;
assert (List.length (elements !b) = 2);
assert (List.length (elements (first (split 2 !b))) = 1);
assert (List.length (elements (third (split 2 !b))) = 1);
assert (not (second (split 2 !b)));
b := remove (min_int, max_int) !b;
assert (is_empty !b);
b := add (min_int, max_int) !b;
assert (List.length (List.rev (fold (fun s acc -> s :: acc) !b [])) = 1);
assert (below max_int !b = max_int);
assert (below (max_int-1) !b = max_int);
assert (below 0 !b = max_int);
assert (below (max_int/2) !b = max_int);
assert (below (-2) !b = max_int);
assert (below (-3) !b = max_int - 1);
assert (below (-4) !b = max_int - 2);
assert (below min_int !b = 1);
assert (below (min_int/2) !b = (max_int/2)+2);
assert (below (min_int+1) !b = 2);
assert (is_empty (first (split min_int !b)));
let tmp = first (split (max_int/2) (third (split (min_int/2) !b))) in
assert (snd (List.hd (elements tmp)) = max_int/2 - 1);
assert (below max_int tmp = max_int - 1);
assert (below ((max_int/2)-2) tmp = max_int - 2);
let tmp = third (split 0 tmp) in
assert (below 0 tmp = 0);
assert (below 1 tmp = 1);
assert (below (max_int/4) tmp = max_int/4);
assert (below max_int tmp = (max_int/2) - 1);
assert (below min_int tmp = 0);
a := remove (min_int, max_int) !a;
a := add (min_int,min_int) !a;
let aux x = List.length (elements x) in
assert (aux !a = 1);
a := add (min_int+2,min_int+2) !a;
assert (aux !a = 2);
a := add (min_int+1,min_int+1) !a;
assert (aux !a = 1);
a := add (max_int, max_int) !a;
assert (aux !a = 2);
a := add (0,0) !a;
assert (aux !a = 3);
a := add (-42,42) !a;
assert (aux !a = 3);
a := add (-44,-44) !a;
a := add (44, 44) !a;
assert (aux !a = 5);
a := add (43, 43) !a;
assert (aux !a = 4);
a := add (-43, -43) !a;
assert (aux !a = 3);
let a = ref empty in
assert (elements !a = []);
a := add (1,1) !a;
assert (below max_int !a = 1);
assert (elements !a = [(1,1)]);
a := add (2,2) !a;
assert (below max_int !a = 2);
assert (elements !a = [(1,2)]);
a := add (3,3) !a;
assert (below max_int !a = 3);
assert (elements !a = [(1,3)]);
a := add (4,4) !a;
assert (below max_int !a = 4);
assert (elements !a = [(1,4)]);
a := add (5,5) !a;
assert (below max_int !a = 5);
assert (elements !a = [(1,5)]);
a := remove (2,5) !a;
assert (below max_int !a = 1);
assert (elements !a = [(1,1)]);
a := add (0,0) !a;
assert (below max_int !a = 2);
assert (elements !a = [(0,1)]);
a := add (-1,-1) !a;
assert (below max_int !a = 3);
assert (elements !a = [(-1,1)]);
a := add (-2,-2) !a;
assert (below max_int !a = 4);
assert (elements !a = [(-2,1)]);
a := add (-3,-3) !a;
assert (below max_int !a = 5);
assert (elements !a = [(-3,1)]);
a := add (-4,-4) !a;
assert (below max_int !a = 6);
assert (elements !a = [(-4,1)]);
a := add (-5,-5) !a;
assert (below max_int !a = 7);
assert (elements !a = [(-5,1)]);
a := remove (-2, -2) !a;
assert (below max_int !a = 6);
assert (elements !a = [(-5,-3);(-1,1)]);
a := remove (-4, -4) !a;
assert (below max_int !a = 5);
a := empty;
assert (is_empty !a);
a := add (min_int, min_int) !a;
assert (elements !a = [(min_int,min_int)]);
a := add (min_int+2,min_int+2) !a;
assert (elements !a = [(min_int,min_int);(min_int+2,min_int+2)]);
a := add (min_int+1,min_int+1) !a;
assert (elements !a = [(min_int,min_int+2)]);
a := remove (min_int+2, min_int+2) !a;
assert (elements !a = [(min_int,min_int+1)]);
a := remove (min_int, min_int) !a;
assert (elements !a = [(min_int+1,min_int+1)]);
a := remove (min_int+1, min_int+1) !a;
assert (elements !a = []);
a := add (min_int+2,min_int+2) !a;
assert (elements !a = [(min_int+2,min_int+2)]);
a := add (min_int, min_int) !a;
assert (elements !a = [(min_int,min_int);(min_int+2,min_int+2)]);
a := add (min_int+1,min_int+1) !a;
assert (elements !a = [(min_int,min_int+2)]);
a := remove (min_int+1, min_int+1) !a;
assert (elements !a = [(min_int,min_int);(min_int+2,min_int+2)]);
a := remove (min_int, min_int) !a;
assert (elements !a = [(min_int+2,min_int+2)]);
a := remove (min_int+2, min_int+2) !a;
assert (elements !a = []); 
a := add (max_int, max_int) !a;
assert (elements !a = [(max_int,max_int)]);
a := add (max_int-2, max_int-2) !a;
assert (elements !a = [(max_int-2,max_int-2);(max_int,max_int)]);
a := add (max_int-1, max_int-1) !a;
assert (elements !a = [(max_int-2,max_int)]);
a := remove (max_int-2, max_int-2) !a;
assert (elements !a = [(max_int-1,max_int)]);
a := remove (max_int, max_int) !a;
assert (elements !a = [(max_int-1,max_int-1)]);
a := remove (max_int-1, max_int-1) !a;
assert (elements !a = []);
a := add (max_int-2, max_int-2) !a;
assert (elements !a = [(max_int-2,max_int-2)]);
a := add (max_int, max_int) !a;
assert (elements !a = [(max_int-2,max_int-2);(max_int,max_int)]);
a := add (max_int-1, max_int-1) !a;
assert (elements !a = [(max_int-2,max_int)]);
a := remove (max_int-1, max_int-1) !a;
assert (elements !a = [(max_int-2,max_int-2);(max_int,max_int)]);
a := remove (max_int, max_int) !a; 
assert (elements !a = [(max_int-2,max_int-2)]);
a := remove (max_int-2, max_int-2) !a;
assert (elements !a = []);
assert (is_empty !a);
let fun1 t = fold (fun (a,b) acc -> a+b+acc) t 0 in
assert (fun1 !a = 0);
a := add (5,5) !a;
assert (fun1 !a = 10);
a := add (0, max_int) !a;
assert (fun1 !a = max_int);
a := remove (0, max_int) !a;
a := add (0,0) !a;
assert (fun1 !a = 0);
a := add (2,2) !a;
assert (fun1 !a = 4);
a := add (4,4) !a;
assert (fun1 !a = 12);
a := add (6,6) !a;
assert (fun1 !a = 24);
a := add (5,5) !a;
assert (fun1 !a = 14);
a := add (1,1) !a;
assert (fun1 !a = 12);
a := add (3,3) !a;
assert (fun1 !a = 6);
let fun2 t = fold (fun (a,b) acc -> acc+(a-b)) t 0 in
assert (fun2 !a = -6);
a := remove (3,3) !a;
assert (fun2 !a = -4);
a := remove (1,1) !a;
assert (fun2 !a = -2);
a := remove (5,5) !a;
assert (fun2 !a = 0);
a := add (1,1) !a;
assert (fun2 !a = -2);
a := remove (0,10) !a;
assert (fun2 !a = 0);
a := add (min_int/2, (max_int/2)-100) !a;
assert (fun2 !a = min_int + 101);
a := remove (min_int/4, max_int/4) !a;
assert (fun2 !a = (min_int/2)+102);
a := remove ((min_int/2+100000), (min_int/3)) !a;
assert (fun2 !a = (min_int/3) - 99896);
a := remove (min_int,max_int) !a;
assert (fun2 !a = 0);
let fun3 t = fold (fun (a,_) acc -> a :: acc) t [] in
let fun4 t = fold (fun (_,a) acc -> a :: acc) t [] in
assert (is_empty !a);
a := add (42,42) !a;
assert (fun3 !a = [42]);
assert (fun4 !a = [42]);
a := add (40,40) !a;
assert (fun3 !a = [42;40]);
assert (fun4 !a = [42;40]);
a := add (44,44) !a;
assert (fun3 !a = [44;42;40]);
assert (fun4 !a = [44;42;40]) *)

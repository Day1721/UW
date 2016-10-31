(* Autor: Marek Puzyna, Recenzent: Michał Pawłowski *)
(* Sortowanie topologiczne *)

open PMap 

(* Typ flagi odwiedzenia wierzchołka grafu *)
type flag = Nieodwiedzony | Odwiedzany | Odwiedzony

(* Wyjątek, rzucany, gdy graf jest cykliczny *)
exception Cykliczne

(* Funkcja, tworząca graf, zaimplementowany jako mapa z modułu PMap *)
let graph_of_list lista = 
   List.fold_left (fun acc (a, l) -> add a (Nieodwiedzony, l) acc) empty lista

(* Funkcja, sprawdzająca flagę danego wierzchołka grafu *)
let czy_odwiedzony w g =
   fst (find w g)

(* Funkcja, która dla danego grafu, zaimplementowanego jako lista, 
   zwraca listę jego wierzchołków, posortowaną topologicznie *)
let topol lista =
   let graph = ref (graph_of_list lista)
   and res = ref [] in
   let rec visit w =
      if not (mem w (!graph)) then begin
         graph := add w (Odwiedzony, []) (!graph);
         res := w::(!res)
         end
      else 
         let neighbors = snd (find w (!graph)) in
         match czy_odwiedzony w (!graph) with
            | Nieodwiedzony -> begin
               graph := add w (Odwiedzany, neighbors) (!graph);
               List.iter visit neighbors;
               graph := add w (Odwiedzony, neighbors) (!graph);
               res := w::(!res);
               end
            | Odwiedzany -> raise Cykliczne
            | Odwiedzony -> () in
   List.iter (fun (w, l) -> visit w) lista;
   !res
   
(* TESTY *) (*
let czy_cykliczne l =
   match (try (topol l) with
      Cykliczne -> []) with
         | [] -> true
         | _ -> false
let test input output =
   let rec loop a b f = function
      | [] -> false
      | h::t -> 
         if f then 
            if h = b then true 
            else loop a b f t
         else if h = a then loop a b true t 
            else loop a b f t
   and pom i a = function
      | [] -> (match i with
         | [] -> true
         | g::o -> pom o (fst g) (snd g))
      | h::t -> match (loop a h false output) with
         | true -> pom i a t
         | false -> false in
   pom (List.tl input) (fst (List.hd input)) (snd (List.hd input))
let a = [(1, [2]); (2, [3]); (3, [4]); (4, [1])]
let b = [(1, [2]); (2, [3]); (3, [4])]
let c = [('A', ['B'; 'C'; 'E']); ('D', ['F'; 'E'; 'G']); ('B', ['C'; 'D']);
   ('C', ['D'; 'F']); ('F', ['G'; 'H'])]
let d = [("zolty", ["niebieski"; "bialy"; "czarny"]); ("bialy", ["czarny"]); 
   ("czarny", []); ("czerwony", ["zielony"; "zolty"; "niebieski"; "czarny"])]
let e = [(1, [2; 5; 8; 3]); (5, [8; 6; 4; 7]); (7, [6; 9; 2]); (8, [6; 9; 3])]
let f = [(Nieodwiedzony, [Odwiedzany; Odwiedzony]); (Odwiedzany, [Odwiedzony])]
let _ = assert(czy_cykliczne a);
        assert(not (czy_cykliczne b));
        assert(test b (topol b));
        assert(test c (topol c));
        assert(test (List.tl c) (topol (List.tl c)));
        assert(test d (topol d));
        assert(test e (topol e));
        assert(test (List.tl e) (topol (List.tl e)));
        assert(test f (topol f));
        assert(czy_cykliczne ((Odwiedzony, [Odwiedzany])::f));
        assert(test (b @ e) (topol (b @ e)));
        assert(test (List.tl b @ e) (topol (List.tl b @ e))) *)
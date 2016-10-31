(** Autor: Marek Puzyna 371359 
 Reviewer: Marian Dziubiak 296768 *)

exception NotExist
exception InvalidArguments
(* Deklaracja typu "wartosc" *)
type wartosc = N of float * float * bool   
(**float1 = min_wartosc, float2 = max_wartosc, 
bool: if true then N(a, b) = [a, b] 
      else N(a, b) = [neg_infinity a] U [b, infinity], 
gdzie [x, y] - przedzia³ dwustronnie domkniêty *)

(* procedura pomocnicza*)
let odwroc w = match w with
   |N(x, y, true) -> if (x >= 0. || y < 0.) 
      then N(1. /. y, 1. /. x, true)
      else if (x < 0. && y = 0.) then N(neg_infinity, 1. /. x, true)
      else N(1. /. x, 1. /. y, false)
   |N(x, y, false) -> N(1. /. x, 1. /. y, true)

let min a b = if classify_float a = FP_nan then b
   else if classify_float b = FP_nan then a
   else if a < b then a else b

let max a b = if classify_float a = FP_nan then b
   else if classify_float b = FP_nan then a
   else if a > b then a else b
	   
let min4 a b c d = min (min a b) (min c d)

let max4 a b c d = max (max a b) (max c d)

(* definicje konstruktorów*)
let wartosc_dokladnosc x p = 
   let pp = abs_float (x *. p /. 100.) in
   if p >= 0. then N(x -. pp, x +. pp, true)         
   else raise InvalidArguments                                           
   (* przetwarza x±p% na [min_wartosc, max_wartosc] *)

let wartosc_od_do x y = if x <= y && classify_float (x -. y) <> FP_nan
   then N(x, y, true)
   else raise InvalidArguments

let wartosc_dokladna x =  N(x, x, true)

(* definicje selektorów *)
let in_wartosc w a = match w with
   |N(x, y, true) -> a <= y && a >= x
   |N(x, y, false) -> not (a < y && a > x)
   
let min_wartosc w = match w with
   |N(x, y, true) -> x
   |N(x, y, false) -> neg_infinity
   
let max_wartosc w = match w with
   |N(x, y, true) -> y
   |N(x, y, false) -> infinity

let sr_wartosc w = ( (min_wartosc w) +. (max_wartosc w) ) /. 2.0

(* definicje modyfikatorów *)
let rec plus w z = match w, z with
   |N(x, y, true), N(a, b, true) -> N(x +. a, y +. b, true)
   |N(x, y, true), N(a, b, false) -> plus z w
   |N(x, y, false), N(a, b, true) -> if (x +. b) >= (a +. y)
      then N(neg_infinity, infinity, true)
      else N(b +. x, a +. y, false)
   |_, _ -> N(neg_infinity, infinity, true)

let minus w z = match z with
   |N(x, y, k) -> plus w (N(-. y, -. x, k))

let rec razy w z = 
   let razy_pom w z = match w, z with
   |N(x, y, true), N(a, b, true) -> 
      N(min4 (x *. a) (x *. b) (y *. a) (y *. b), 
      max4 (x *. a) (x *. b) (y *. a) (y *. b), true)
   |N(x, y, false), N(a, b, false) -> 
      N(min (x *. b) (y *. a), max (x *. a) (y *. b), false)
   |N(x, y, false), N(a, b, true) -> razy z w
   |N(x, y, true), N(a, b, false) -> if x > 0. 
      then N(a *. x, b *. x, false)
      else if y < 0. then N(y *. b, y *. a, false)
      else N(neg_infinity, infinity, true) in
   let razy_st = razy_pom w z in
   match razy_st with
   |N(x, y, b) -> if (classify_float x = FP_nan) || (classify_float y = FP_nan)
      then raise NotExist
      else razy_st
   
let podzielic w z = razy w (odwroc z)

(*  (* asercje*)
let _ = assert(sr_wartosc (N(-4., 8., true)) = 2.);
        assert(wartosc_dokladnosc 6. 50. = N(3., 9., true));
        assert(wartosc_dokladnosc (-5.) 20. = N(-6., -4., true));
        assert(plus (N(1., 2., true)) (N(3., 4., true)) 
               = (N(4., 6., true)));
        assert(razy (wartosc_dokladnosc 8. 40.) (wartosc_od_do (-5.) 4.) 
               = (N(-56., 44.8, true)));
        assert(in_wartosc (N(-1., 1., false)) 0. = false) *)

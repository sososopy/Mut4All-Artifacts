use std :: collections :: HashMap ; type Insertions = HashMap < (usize , usize) , usize > ; type CountMap = HashMap < (usize , usize , usize) , Vec < usize > > ; struct Polymerizer < 'a > { cur_depth : usize , max_depth : usize , counts : CountMap , insertions : & 'a Insertions , } impl < 'a > Polymerizer < 'a > { fn new (max_depth : usize , insertions : & 'a Insertions) -> Self { let mut ret = Polymerizer { cur_depth : 0 , max_depth , counts : HashMap :: new () , insertions , } ; for keys in insertions . keys () { let mut v = vec ! [0 ; 128] ; v [keys . 0] = 1 ; v [keys . 1] = 1 ; ret . counts . insert ((keys . 0 , keys . 1 , max_depth) , v) ; } ret } fn polymerize_pairs (& mut self , p1 : usize , p2 : usize) { if self . cur_depth == self . max_depth || self . counts . get (& (p1 , p2 , self . cur_depth)) . is_some () { return ; } let ins = * self . insertions . get (& (p1 , p2)) . unwrap () ; self . cur_depth += 1 ; self . polymerize_pairs (p1 , ins) ; self . polymerize_pairs (ins , p2) ; self . cur_depth -= 1 ; let v1 = self . counts . get (& (p1 , ins , self . cur_depth + 1)) . unwrap () ; let v2 = self . counts . get (& (ins , p2 , self . cur_depth + 1)) . unwrap () ; let mut v : Vec < usize > = v1 . iter () . zip (v2 . iter ()) . map (| (c1 , c2) | c1 + c2) . collect () ; v [ins] -= 1 ; eprintln ! ("({:2}, {:2}, {:2}) - {:?}" , self . cur_depth , p1 as u8 as char , p2 as u8 as char , v) ; self . counts . insert ((p1 , p2 , self . cur_depth) , v) ; } fn polymerize (mut self , polymer : & str) -> Vec < usize > { polymer . as_bytes () . windows (2) . for_each (| w | self . polymerize_pairs (w [0] . into () , w [1] . into ())) ; polymer . as_bytes () . windows (2) . fold (vec ! [0_usize ; 128] , | acc , key | { let mut vec = self . counts . get (& (key [0] . into () , key [1] . into () , 0_usize)) . unwrap () ; vec [key [1] as usize] -= 1 ; acc . into_iter () . zip (vec . iter ()) . map (| (a , v) | a + v) . collect () }) } } pub fn run (input : & 'static str) -> (usize , usize) { let mut input = input . trim () . lines () ; let template = input . next () . unwrap () ; let insertions = input . skip (1) . filter_map (| line | line . split_once (" -> ")) . map (| (k , v) | { let k = k . as_bytes () ; ((k [0] . into () , k [1] . into ()) , v . chars () . next () . unwrap () as usize ,) }) . collect :: < HashMap < _ , _ > > () ; let polymer = Polymerizer :: new (1 , & insertions) . polymerize (template) ; eprintln ! ("{:?}" , polymer) ; let (min , max) = polymer . into_iter () . filter (| c | * c != 0) . fold ((usize :: MAX , 0) , | (min , max) , v | (min . min (v) , max . max (v))) ; let d14p1 = max - min ; let polymer = Polymerizer :: new (4 , & insertions) . polymerize (template) ; eprintln ! ("{:?}" , polymer) ; let (min , max) = polymer . into_iter () . filter (| c | * c != 0) . fold ((usize :: MAX , 0) , | (min , max) , v | (min . min (v) , max . max (v))) ; let d14p2 = max - min ; (d14p1 , d14p2) } # [test] fn test () { let input = "
NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C
" ; assert_eq ! (run (input) , (1588 , 2188189693529)) ; }
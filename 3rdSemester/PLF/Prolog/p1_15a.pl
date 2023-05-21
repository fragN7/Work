remove_elem([], _,[]).
remove_elem([Head|Tail], Elem, Result) :-
    Head =:= Elem,
    remove_elem(Tail, Elem, Result).

remove_elem([Head|Tail], Elem, [Head|Result]) :-
    Head =\= Elem,
    remove_elem(Tail, Elem, Result).

%remove_elem(l1...ln, elem) :
%[] if list empty
%l1 + remove_elem(l2...ln, elem) if l1 == e
%remove_elem(l2...ln, elem) if l1 != e

%remove_elem(List, Number, List)
%remove_elem(i, i, o)

transform([], []).
transform([Head|Tail], [Head|Result]) :-
    remove_elem(Tail, Head, Result1),
    transform(Result1, Result).

test_transform() :-
    transform([], []),
    transform([1, 2, 3, 4], [1, 2, 3, 4]),
    transform([4, 2, 1, 2, 4, 2, 1], [4, 2, 1]),
    transform([3, 3, 1, 2, 1, 2, 3, 3], [3, 1, 2]).

% transform(l1....ln) :
% [] if list empty
% l1 + transform(remove_elem(l2...ln, l1)), otherwise

%transform(List, List)
%transform(i,o)


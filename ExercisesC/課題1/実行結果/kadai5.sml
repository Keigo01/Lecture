use "lib.sml";

fun compute s =
    let	
	fun EXP nil = raise SyntaxError
          | EXP (h::t) =
            if isInt h then (toInt h, t)
	    else if h = "(" then
		if isAlp( hd t) then 
		    COMP t
		else
		    FUNC t
            else raise SyntaxError

        and FUNC nil = raise SyntaxError
          | FUNC (h::t) = 
	    if h = "fibo" then
		let
		    val (v1,t1) = EXP t
		in
		    (fibo v1,CHE t1)
                end
	    else if h = "fact" then
		let 
		    val (v1,t1) = EXP t
	        in
		    (fact v1,t1)
		end
	    else raise SyntaxError

		      
        and CHE nil = raise SyntaxError
	  | CHE (h::t) = 
	    if h = ")" then
		t
	    else raise SyntaxError

	and COMP nil = raise SyntaxError
          | COMP (h::t) =
            if h = "+" then
                let
                    val (v1,t1) = EXP t
                    val (v2,t2) = EXP t1
                in
                    (v1 + v2, CHE t2)
                end
	   else  if h = "-" then
	       let
                    val (v1,t1) = EXP t
                    val (v2,t2) = EXP t1
                in
                    (v1 - v2,CHE t2)
                end
	   else if h = "*" then
	       let
                    val (v1,t1) = EXP t

                    val (v2,t2) = EXP t1
                in
                    (v1 * v2, CHE t2)
                end
           else if h = "/" then
	       let
                    val (v1,t1) = EXP t
                    val (v2,t2) = EXP t1
                in
                    (v1 div v2, CHE t2)
                end
            else raise SyntaxError
    in
	let
            val (result,rest) = EXP (separate s)
	in
            if rest = nil then result else raise SyntaxError
	end
    end;

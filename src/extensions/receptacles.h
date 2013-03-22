

!!==============================================================================
!!
!!	RECEPTACLES
!!	Contenedores (con capacidad definida)
!!
!!==============================================================================
!!
!!	File:			receptacles.h
!!	Author(s):		Peer Shaefer (peerschaefer@gmx.net) - original
!!					J. Francisco Mart�n (jfm.lisaso@gmail.com) - traducci�n
!!	Language:		ES (Castellano)
!!	System:			Inform/INFSP 6
!!	Platform:		M�quina-Z / Glulx
!!	Version:		2.0
!!	Released:		2012/05/03
!!	Notes:			Las funciones pueden no funcionar como se espera si se usa 
!!					el compilador Inform 6.15 o anteriores
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2002, 2006, Peer Schaefer
!!	Copyright (c) 2012, J. Francisco Mart�n
!!
!!	Este programa es software libre: usted puede redistribuirlo y/o 
!!	modificarlo bajo los t�rminos de la Licencia P�blica General GNU 
!!	publicada por la Fundaci�n para el Software Libre, ya sea la versi�n 
!!	3 de la Licencia, o (a su elecci�n) cualquier versi�n posterior.
!!
!!	Este programa se distribuye con la esperanza de que sea �til, pero 
!!	SIN GARANT�A ALGUNA; ni siquiera la garant�a impl�cita MERCANTIL o 
!!	de APTITUD PARA UN PROP�SITO DETERMINADO. Consulte los detalles de 
!!	la Licencia P�blica General GNU para m�s informaci�n.
!!
!!	Deber�a haber recibido una copia de la Licencia P�blica General GNU 
!!	junto a este programa. En caso contrario, consulte
!!	<http://www.gnu.org/licenses/>.
!!
!!------------------------------------------------------------------------------
!!
!!	REFERENCIA
!!
!!	Esta librer�a implementa una nueva clase de objetos, llamados "receptacles".
!!	Los recept�culos son contenedores (o soportes) capaces de manejar par�metros
!!	de peso, volumen y tama�o. Cada vez que el jugador intenta colocar un objeto
!!	en uno de ellos, el recept�culo comprobar� si la capacidad restante es 
!!	suficiente para almacenar el nuevo objeto o no.
!!
!!	Para definir su peso, volumen y tama�o, los objetos del juego pueden tener 
!!	definidas las siguiente propiedades (no es necesario que lo hagan):
!!		-- weight (peso)
!!		-- volume (volumen)
!!		-- size (tama�o)
!!				("size" represents the maximum length on any axis, e.g.
!!				the length of an arrow or of a staff. It's obvious that
!!				the volume of a quiver is "used up" when you store many
!!				arrows in it, but the length of the quiver is not "used
!!				up" by the length of the stored arrows: it only
!!				indicates whether an arrow is too long for the quiver
!!				or not.)
!!
!!	Cada una de estas tres propiedades puede ser un valor num�rico o una rutina 
!!	que retorne un valor num�rico. Si alguna de las tres propiedades no est� 
!!	definida o tiene el valor 0 (cero), esa medida respectiva del objeto ser� 0 
!!	(lo que significa que el objeto carece de peso, volumen o tama�o, o 
!!	simplemente que esa medida es despreciable).
!!
!!	NOTA:	weight, volume y size se miden en unidades abstractas (son simples 
!!			n�meros sin dimensi�n). Que una unidad se refiera (por ejemplo) a un 
!!			gramo, una libra, una tonelada o la masa de un sol es decisi�n tan 
!!			s�lo del programador.
!!
!!	Para crear un contenedor que compruebe autom�ticamente el peso, volumen y 
!!	tama�o de los objetos, se usar� una nueva clase "Receptacle". Los 
!!	contenedores de esta clase pueden definir tres nuevas propiedades:
!!		-- capacity_weight
!!		-- capacity_volume
!!		-- capacity_size
!!	Cada una de estas tres propiedades puede ser un valor num�rico o una rutina 
!!	que devuelva un valor num�rico. Si alguna de estas propiedades no est� 
!!	definida o tiene el valor INFINITE_CAPACITY (una constante predefinida por 
!!	la librer�a), la capacidad respectiva para esa medida del recept�culo ser� 
!!	infinita.
!!
!!	Por supuesto los contenedores/recept�culos pueden tener tambi�n definido un 
!!	peso, volumen y tama�o (ya que no son s�lo contenedores/recept�culos, si no 
!!	tambi�n objetos que puedes ser colocados dentro de otros contenedores).
!!
!!	En res�men:
!!	 *	El jugador puede almacenar m�ltiples objetos en un recept�culo, siempre 
!!		que el peso total no exceda el valor 'capacity_weight' del recept�culo.
!!	 *	El jugador puede almacenar m�ltiples objetos en un recept�culo, siempre 
!!		que el volumen total no exceda el valor 'capacity_volume' del 
!!		recept�culo.
!!	 *	En otras palabras: 'capacity_weight' y 'capacity_volume' son valores 
!!		empleados por los objetos que se almacenar�n en el recept�culo.
!!	 *	La propiedad 'capacity_size' se comporta de forma diferente: no se trata
!!		de un l�mite que se agote a medida que se va a�adiendo m�s peso o 
!!		volumen al recept�culo, si no que se tiene que comprobar por separado 
!!		cada vez que se intenta introducir un nuevo objeto en �l.
!!
!!	A forma de peque�o extra, un recept�culo puede tener tambi�n definida una 
!!	nueva propiedad 'capacity_number'. Esta propiedad tiene que ser bien un 
!!	valor num�rico o bien una rutina que devuelva un valor num�rico. Indica 
!!	cu�ntos objetos (como m�ximo) se pueden almacenar dentro de un recept�culo. 
!!	Saltar� un mensaje de error cada vez que se intente introducir cualquier 
!!	objeto una vez que se ha alcanzado ese m�ximo.
!!
!!------------------------------------------------------------------------------
!!
!!	ALGUNOS DETALLES T�CNICOS:
!!
!!	*	Calcular el peso de un contenedor (o soporte) es un proceso recursivo: 
!!		los pesos de todos los objeto-hijo (inmediatos) se a�aden al peso del 
!!		propio contenedor; el peso total de estos objetos-hijo depende tambi�n 
!!		de si ellos albergan a su vez otros objetos en su interior, y as� 
!!		sucesivamente.
!!	*	Si el peso de un objeto viene dado por una rutina, esta rutina deber�a 
!!		devolver el peso TOTAL del objeto (incluyendo los pesos de todos los 
!!		objeto-hijo que tenga en su interior, el de sus objeto-nieto, etc.) Los 
!!		pesos de los objeto-hijo NO se calculan autom�ticamente en este caso. Lo
!!		que a�ade flexibilidad a la hora de crear recept�culos especiales (e.j. 
!!		una bolsa m�gica cuyo peso es menor que el de los objetos que contiene 
!!		en su interior).
!!	*	El volumen y tama�os de los hijos NO se a�ade ni al volumen ni al tama�o
!!		del objeto (se asume que un contenedor est�ndar no es flexible y tiene 
!!		unas proporciones fijas y predeterminadas). Para reescribir este 
!!		comportamiento puedes definir tus porpias propiedades VOLUME y SIZE, 
!!		de forma que calculen el volumen y tama�o del objeto en tiempo de 
!!		ejecuci�n (mirar el ejemplo de m�s abajo).
!!	*	Est� totalmente permitido crear un contenedor cuya capacidad sea 
!!		superior a sus propias medidas (se trata de un comportamiento altamente 
!!		improbable en un objeto, pero de esta manera pueden crearse cajas 
!!		m�gicas, agujeros negros y otros objetos extra�os).
!!
!!------------------------------------------------------------------------------
!!
!!	VERBOS DE DEPURACI�N:
!!
!!	Al compilar en modo-debug, 'recept_sp.h' define tres meta-acciones nuevas:
!!		peso OBJETO				- imprime el peso del objeto
!!		dimensiones OBJETO		- imprime las dimensiones del objeto
!!		capacidad OBJETO		- imprime las capacidades como recept�culo
!!
!!------------------------------------------------------------------------------
!!
!!	EJEMPLO 1:
!!
!!	Receptacle box "box" with name "box",
!!	       volume 10,                ! The box itself has a volume of 10
!!	       capacity_volume 9,        ! And it can store a volume of 9
!!	       has container;
!!
!!	Object stone "stone" with name "stone",
!!	       volume 2;                 ! This stone has a volume of 2
!!
!!	The lines above create a box with a volume of 10 and a capacity of 9,
!!	and a stone with a volume of 2. The player can put up to 4 stones in
!!	the box (with a total volume of 4x2=8). A fifth stone can't be stored
!!	in the box, since the capacity of the box is 9 and the total volume
!!	of 5 stones would be 10.
!!
!!-------------------
!!
!!	EJEMPLO 2:
!!
!!	The following example creates a wooden box and a steel box:
!!
!!	Receptacle wooden_box "wooden box"
!!	       with name "wooden" "box",
!!	       volume 10,              ! The box itself has a volume of 10
!!	                               ! units
!!	       capacity_volume 9,      ! And it can store objects up to a
!!	                               ! volume of 9 units
!!	       has container;
!!
!!	Receptacle steel_box "steel box"
!!	       with name "steel" "box",
!!	       volume 8,               ! The box itself has a volume of 8
!!	                               ! units
!!	       capacity_volume 7,      ! And it can store objects up to a
!!	                               ! volume of 7 units
!!	       has container;
!!
!!	You can put the steel box into the wooden box (volume 8 fits in
!!	capacity 9) but not the wooden box into the steel box (volume 10
!!	doesn't fit in capacity 7). If you put something with a volume of 2
!!	or greater into the wooden box, you can't put the steel box into
!!	it because that would require a free volume of 8 or more.
!!
!!-------------------
!!
!!	EJEMPLO 3:
!!
!!	Volume and size of childrens are NOT added to the objects volume or
!!	size (assuming that containers are not flexible and have fixed
!!	proportions), so if you want to create a flexible bag whose volume
!!	grows if objects are stored inside, you should code an appropriate
!!	routine as the volume property:
!!
!!	Receptacle -> bag "flexible bag"
!!	       with name "flexible" "bag",
!!	       capacity_volume 20,
!!	       capacity_size 5,
!!	       volume [ v i;
!!	               v = 1;                         ! Minimal volume.
!!	               objectloop (i in bag)          ! Add volumes of all
!!	                       v = v + VolumeOf (i);  ! immediate childs.
!!	               return v;
!!	       ],
!!	       size [ s i;
!!	               s = 1;                  ! Minimal size when empty.
!!	               objectloop (i in bag)   ! Find the largest child:
!!	                       if (SizeOf (i) > s) s = SizeOf (i);
!!	               return s;               ! (The size of the largest
!!	       ],                              ! immediate child determines
!!	       has container;                  ! the size of the bag.)
!!
!!-------------------
!!
!!	EJEMPLO 4:
!!
!!	The weights of childrens are added automatically to the objects
!!	weight. You can override this by providing an own weight-routine.
!!	The following example creates a wonder-bag, whose total-weight is
!!	only half the total-weight of it's contents:
!!
!!	Receptacle -> wonder_bag "wonder bag"
!!	       with name "wonder" "magic" "bag",
!!	       capacity_volume 100,
!!	       weight [ w i;
!!	               w = 1;                        ! Base weight of bag is 1
!!	               objectloop (i in wonder_bag)
!!	                       w = w + WeightOf (i); ! Add up weights...
!!	               return (w/2);                 ! ...and return 50%
!!	       ],
!!	       has container;
!!
!!------------------------------------------------------------------------------

System_file;
Constant INFINITE_CAPACITY -1;		! (-1) simboliza una capacidad infinita

Class	FAKE_RECEPT					! Se definen las siete nuevas
 with	weight	0,					! propiedades sin usar nada de
		size	0,					! espacio del programa o de
		volume	0,					! runtime-memory
		capacity_weight	0,
		capacity_size	0,
		capacity_volume	0,
		capacity_number	0;

!!==============================================================================
!! Funciones para calcular el peso, volumen y tama�o de cualquier objeto dado
!!------------------------------------------------------------------------------

[ WeightOf obj		w i;
	if (obj provides weight) {
		if (metaclass(obj.weight) == Routine) return indirect (obj.weight);
		w = obj.weight;
	} else  w = 0;
	if ((obj has container) || (obj has supporter))
		! Add weight of child-objects
		objectloop (i in obj) w = w + WeightOf (i);
	return w;
];

[ SizeOf obj;
	if (obj provides size) {
		if (metaclass(obj.size) == Routine) return indirect (obj.size);
		return obj.size;
	}
	return 0;
];

[ VolumeOf obj;
	if (obj provides volume) {
		if (metaclass(obj.volume) == Routine) return indirect (obj.volume);
		return obj.volume;
	}
	return 0;
];

!!==============================================================================
!! Funciones para calcular la capacidad de cualquier contenedor o soporte dado
!!------------------------------------------------------------------------------

[ CapacityWeightOf obj;
	if (obj provides capacity_weight) {
		if (metaclass(obj.capacity_weight) == Routine)
			return indirect (obj.capacity_weight);
		return obj.capacity_weight;
	}
	return INFINITE_CAPACITY;	! Unlimited weight capacity
];

[ CapacityVolumeOf obj;
	if (obj provides capacity_volume) {
		if (metaclass(obj.capacity_volume) == Routine)
			return indirect (obj.capacity_volume);
		return obj.capacity_volume;
	}
	return INFINITE_CAPACITY;	! Unlimited volume capacity
];

[ CapacitySizeOf obj;
	if (obj provides capacity_size) {
		if (metaclass(obj.capacity_size) == Routine)
			return indirect (obj.capacity_size);
		return obj.capacity_size;
	}
	return INFINITE_CAPACITY;	! Unlimited size capacity
];

[ CapacityNumberOf obj;
	if (obj provides capacity_number) {
		if (metaclass(obj.capacity_number) == Routine)
			return indirect (obj.capacity_number);
		return obj.capacity_number;
	}
	return INFINITE_CAPACITY;	! Unlimited number of objects
];

!!==============================================================================
!! Funci�n auxiliar para comprobar si un contenedor o soporte tiene capacidad 
!! para soportar un objeto, dados ambos como argumentos. Retorna falso si el 
!! objeto no entra, verdadero en caso contrario.
!!------------------------------------------------------------------------------
[ CheckIfObjectFits receiver obj	s i;
	!! Se comprueba si el peso de obj est� dentro de los l�mites de receiver:
	if ( CapacityWeightOf( receiver ) ~= INFINITE_CAPACITY ) {
		if ( WeightOf(obj) > CapacityWeightOf( receiver ))
			return false;
		s = 0; objectloop (i in receiver) s = s + WeightOf(i);
		if ( (s + WeightOf(obj)) > CapacityWeightOf (receiver) )
			return false;
	}
	!! Se comprueba si el volumen de obj est� dentro de los l�mites de receiver:
	if ( CapacityVolumeOf (receiver) ~= INFINITE_CAPACITY ) {
		if ( VolumeOf(obj) > CapacityVolumeOf( receiver ))
			return false;
		s = 0; objectloop (i in receiver) s = s + VolumeOf(i);
		if ( (s + VolumeOf(obj)) > CapacityVolumeOf( receiver ))
			return false;
	}
	!! Se comprueba si el tama�o de obj est� dentro de los l�mites de receiver:
	if ( CapacitySizeOf (receiver) ~= INFINITE_CAPACITY ) {
		if ( SizeOf(obj) > CapacitySizeOf( receiver ))
			return false;
	}
	!! Se comprueba el l�mite de n�mero de objetos de receiver:
	if ( CapacityNumberOf (receiver) ~= INFINITE_CAPACITY ) {
		s = 0; objectloop (i in receiver) s++;
		if ( (s+1) > CapacityNumberOf( receiver ))
			return false;
	}
	!! El objeto encaja en el receptor:
	return true;
];

!!==============================================================================
!! Definici�n de la nueva clase Receptacle
!!------------------------------------------------------------------------------

Class	Receptacle
 with	before [ s i;
			Receive:
				!! Se comprueba el peso (si su capacidad no es infinita):
				if ( CapacityWeightOf( self ) ~= INFINITE_CAPACITY ) {
					if ( WeightOf(noun) > CapacityWeightOf( self ))
						return self.error_messages(1, noun);
					!! Se calcula el peso de todo el contenido
					s = 0; objectloop (i in self) s = s + WeightOf(i);
					if ( (s + WeightOf(noun)) > CapacityWeightOf (self) )
						return self.error_messages(2);
				}

				!! Se comprueba el volumen (si su capacidad no es infinita):
				if ( CapacityVolumeOf (self) ~= INFINITE_CAPACITY ) {
					if ( VolumeOf(noun) > CapacityVolumeOf( self ))
						return self.error_messages(3, noun);
					!! Se calcula el volumen de todo el contenido
					s = 0; objectloop (i in self) s = s + VolumeOf(i);
					if ( (s + VolumeOf(noun)) > CapacityVolumeOf( self ))
						return self.error_messages(4);
				}

				!! Se comprueba el tama�o (si su capacidad no es infinita):
				if ( CapacitySizeOf (self) ~= INFINITE_CAPACITY ) {
					if ( SizeOf(noun) > CapacitySizeOf( self ))
						return self.error_messages(5, noun);
				}

				!! Se comprueba el n�mero (si su capacidad no es infinita):
				if ( CapacityNumberOf (self) ~= INFINITE_CAPACITY ) {
					s = 0; objectloop (i in self) s++;
					if ( (s+1) > CapacityNumberOf( self ))
						return self.error_messages(6);
				}

				return false;
		],
		!! Mensajes de error. Se producen al intentar meter en el recept�culo 
		!! un objeto que viola alguno de los l�mites de capacidad definidos
		error_messages [ id obj;
			switch (id) {
				1:
					!! Intentar meter en el bolsillo un objeto cuyo peso supera 
					!! el l�mite permitido
					print_ret (The) obj, " ", (es) obj, " demasiado pesad", 
					(o) obj ," para ", (the) obj ,".";
				2:
					!! Intentar meter en el bolsillo un objeto cuyo peso, unido 
					!! al del resto de objetos que hay ya en el bolsillo, 
					!! superan el l�mite permitido
					print_ret (The) self ," ha", (n) self ," alcanzado su 
					capacidad m�xima.";
				3:
					!! Intentar meter en el bolsillo un objeto cuyo volumen 
					!! supera el l�mite permitido
					print_ret (The) obj ," no encaja", (n) obj ," dentro de ", 
					(the) self ,".";
				4:
					!! Intentar meter en el bolsillo un objeto cuyo volumen, 
					!! unido al del resto de objetos que hay ya en el bolsillo, 
					!! superan el l�mite permitido
					print_ret (The) self ," ha", (n) self ," alcanzado su 
					capacidad m�xima.";
				5:
					!! Intentar meter en el bolsillo un objeto cuyo tama�o 
					!! supera el l�mite permitido
					print_ret (The) obj ," ", (es) obj ," demasiado grande", 
					(s) obj ," para ", (the) self ,".";
				6:
					!! Intentar meter en el bolsillo un objeto despu�s de haber 
					!! alcanzado el l�mite de objetos introducidos permitido
					"En ", (the) self ," no entran ya m�s cosas.";
			}
		];

!!==============================================================================
!! Verbos de depuraci�n
!!------------------------------------------------------------------------------

#ifdef DEBUG;

Verb meta 'peso' * noun			-> MetaWeigh;
Verb meta 'dimensiones' * noun	-> MetaMeasure;
Verb meta 'capacidad' * noun	-> MetaCapacity;

[ MetaWeighSub;
	print (The) noun, " pesa", (n) noun ," ",
	WeightOf (noun), " unidad";
	if ( WeightOf( noun ) ~= 1 ) print "es";
	new_line;
	return true;
];

[ MetaMeasureSub;
	print (The) noun, ":^";
	spaces(6);	print "Peso: ",		WeightOf(noun),	" unidades^";
	spaces(6);	print "Tama�o: ",	SizeOf(noun),	" unidades^";
	spaces(6);	print "Volumen: ",	VolumeOf(noun),	" unidades^";
	return true;
];

[ MetaCapacitySub;
	print (The) noun, ":^";

	spaces(6); print "Capacidad (peso): ";
	if (CapacityWeightOf(noun) == INFINITE_CAPACITY) print "infinita^";
	else print CapacityWeightOf(noun), " unidades^";

	spaces(6); print "Capacidad (tama�o): ";
	if (CapacitySizeOf(noun) == INFINITE_CAPACITY) print "infinita^";
	else print CapacitySizeOf(noun), " unidades^";

	spaces(6); print "Capacidad (volumen): ";
	if (CapacityVolumeOf(noun) == INFINITE_CAPACITY) print "infinita^";
	else print CapacityVolumeOf(noun), " unidades^";

	spaces(6); print "Capacidad (n�mero de objetos): ";
	if (CapacityNumberOf(noun) == INFINITE_CAPACITY) print "infinita^";
	else print CapacityNumberOf(noun), "^";

	return true;
];

#endif;



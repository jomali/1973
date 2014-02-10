

!!==============================================================================
!!
!!	TOPIC INVENTORY
!!	Sistema de conversaci�n con inventario de temas
!!
!!==============================================================================
!!
!!	File:			topicInventory.h
!!	Author(s):		J. Francisco Mart�n <jfm.lisaso@gmail.com>
!!					Mastodon
!!	Language:		ES (Castellano)
!!	System:			Inform-INFSP 6
!!	Platform:		Z-Machine / Glulx
!!	Version:		2.0
!!	Released:		2014/02/10
!!
!!------------------------------------------------------------------------------
!!
!!	# HISTORIAL DE VERSIONES
!!
!!	2.0: 2014/02/10	Mejor gesti�n en el cambio de conversaciones activas en el 
!!					gestor y a�adidos temas con presencia temporizada.
!!	1.8: 2014/02/05	Cambiado el nombre de la clase *ConversationEntry* por 
!!					*ConversationTopic* y otras peque�as correcciones.
!!	1.7: 2014/01/29	Cambiados los nombres de las clases y peque�as correcciones.
!!	1.4: 2013/04/13	Modificaci�n de la propiedad *show_topic_inventory* del 
!!					objeto gestor.
!!	1.3: 2013/04/01 A�adidas nuevas propiedades a la clase *ConversationTopic* 
!!					y modificaci�n de la rutina *NPCTalkSub()*.
!!	1.2: 2013/03/30	A�adidos a la documentaci�n.
!!	1.1: 2013/03/30 A�adidas nuevas propiedades al objeto *ConversationManager* 
!!					y modificaci�n de la rutina *NPCTalkSub()*.
!!	1.0: 2013/03/30	Primera versi�n de la extensi�n.
!!	0.1: 2013/03/28	Versi�n preliminar.
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2009, Mastodon
!!	Copyright (c) 2013, 2014, J. Francisco Mart�n
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
!!	# INTRODUCCI�N
!!
!!	*TOPIC INVENTORY* es una extensi�n para implementar sistemas de 
!!	conversaci�n utilizando inventario de temas y an�lisis no estricto de la 
!!	entrada de usuario para el reconocimiento de patrones. Est� basada en las 
!!	notas de Eric Eve sobre sistemas de conversaci�n:
!!	<http://www.tads.org/howto/convbkg.htm>, y construida sobre la extensi�n 
!!	*NPC_Conversacion* v1.0 de Mastodon.
!!
!!
!!	# CONVERSACIONES CON INVENTARIO DE TEMAS
!!
!!	El sistema de conversaci�n con invantario de temas est� ideado con el 
!!	objetivo de esquivar los problemas que suelen presentar los sistemas de 
!!	conversaci�n m�s habituales en los relatos interactivos; dificultades para 
!!	adivinar la palabra clave en sistemas basados en acciones ASK/TELL, 
!!	simplificaci�n excesiva en sistemas basados en la acci�n TALK TO, o falta 
!!	de libertad y ruptura de la interfaz textual en sistemas de men�s. 
!!
!!	Al utilizar un inventario de temas, en esencia, se presenta al usuario una 
!!	lista de temas de conversaci�n sugeridos bas�ndose en el conocimiento del 
!!	personaje protagonista. De esta forma, el usuario puede hacer referencia a 
!!	los temas que quiera lanzar utilizando una interfaz similar a la de los 
!!	sistemas ASK/TELL, pero sin necesidad de tener que adivinar por s� mismo la 
!!	palabra clave que lanza el tema.
!!
!!	En concreto, el sistema se presentar�a de forma similar a la siguiente:
!!
!!		> hablar con Bob
!!		---Disculpe...
!!		Bob levanta la vista de su libro y se quita las gafas- ---�Si?
!!
!!		(Puedes preguntar por la antena, hablarle sobre el faro o hablarle 
!!		del tema que nunca se acaba.)
!!
!!		> hablar sobre el faro
!!		---He pasado junto al faro esta ma�ana ---empiezas.
!!		Bob se pone muy nervioso de pronto. ---�De qu� est�s hablando? Esa cosa 
!!		se vino abajo hace a�os. Despu�s de aquel... asunto.
!!
!!		> hablar con Bob
!!		Bob simula limpiar las gafas, inquieto. 
!!
!!		(Puedes preguntar por la antena, preguntar por el asunto o hablarle 
!!		del tema que nunca se acaba.)
!!
!!		> asunto
!!		---�A qu� "asunto" te refieres?
!!		Bob se coloca las gafas y vuelve de nuevo su atenci�n al libro, 
!!		claramente agitado. ---T-Tengo mucho trabajo que hacer. Si me 
!!		disculpas...
!!
!!		(Puedes insistir con el "asunto", preguntar por la antena o hablarle 
!!		del tema que nunca se acaba.)
!!
!!		> tema interminable
!!		---T�o, �d�nde est� mi coche?
!!		---�Y tu coche, t�o?
!!
!!		(Puedes insistir con el "asunto", preguntar por la antena o hablarle 
!!		del tema que nunca se acaba.)
!!
!!	El reconocimiento de patrones entre la entrada de usuario y las claves de 
!!	cada tema se lleva a cabo por medio de an�lisis no estricto; si entre todas 
!!	las palabras introducidas se reconocen claves de uno o m�s temas, se lanza 
!!	aquel tema con m�s coincidencias ignorando el resto de la entrada.
!!
!!
!!	# SOBRE LOS MENSAJES
!!
!!	El autor puede definir hasta 7 constantes para modificar los textos por 
!!	defecto de la extensi�n:
!!
!!	 *	CONVERSATION_STYLE: (0-3) Define el estilo con el que se imprimen los 
!!		textos de la extensi�n: 0-romana, 1-it�lica, 2-negrita, 3-monoespaciada.
!!	 *	CONVERSATION_PREFIX: Cadena de texto previa a cualquier otro mensaje.
!!	 *	CONVERSATION_SUFIX: Cadena de texto posterior a cualquier otro mensaje.
!!	 *	CONVERSATION_MSG1: Antecede a la lista de temas del inventario.
!!	 *	CONVERSATION_MSG2: Si hay m�s de un tema en el inventario, se imprime 
!!		entre CONVERSATION_MSG1 y la lista de temas.
!!	 *	CONVERSATION_OR: Separador de los dos �ltimos temas del inventario.
!!	 *	CONVERSATION_NO_MSG: Mensaje cuando no hay temas en el inventario.
!!
!!
!!	# UTILIZACI�N
!!
!!	Para usar la librer�a �nicamente hay que incluir la siguiente l�nea en la 
!!	rutina BeforeParsing (crearla si no existe):
!!
!!		ConversationManager.try();
!!
!!
!!	# LIMITACIONES Y POSIBLES MEJORAS
!!
!!	Podr�a ser interesante contemplar la posibilidad de crear temas ocultos que 
!!	no aparezcan sugeridos autom�ticamente al imprimir el inventario de temas 
!!	disponibles.
!!
!!------------------------------------------------------------------------------
System_file;

!! Descomentar para obtener informaci�n de depuraci�n:
!Constant DEBUG_TOPICINVENTORY;

!! Vector para guardar palabras temporalmente:
Array tmp_text -> 64;

Default CONVERSATION_STYLE	1; ! (0-3)
Default CONVERSATION_PREFIX	"(";
Default CONVERSATION_SUFIX	")";
Default CONVERSATION_MSG1	"Puedes ";
Default CONVERSATION_MSG2	"escoger entre ";
Default CONVERSATION_OR		" o ";
Default CONVERSATION_NO_MSG	"No hay temas que tratar";

!!==============================================================================
!!	Funciones de depuraci�n
!!------------------------------------------------------------------------------

#Ifdef DEBUG_TOPICINVENTORY;

!! Funci�n para pintar un String Array
[ PrintStringArray the_array i;
	print "(", the_array-->0, ")";
	for (i = WORDSIZE : i < (the_array-->0) + WORDSIZE : i++) 
		print (char) the_array->i;
];

!! Funci�n para pintar una palabra del Prompt de entrada del jugador
[ PrintPromptWord num_word dir i;
	dir = WordAddress(num_word);
	for (i = 0 : i < WordLength(num_word) : i++)
		print (char) dir->i;
];

#Endif; ! DEBUG_TOPICINVENTORY;

!!==============================================================================
!!	Compara una palabra de la entrada del usuario con una de las palabras de 
!!	diccionario. La palabra de entrada se pasa a la funci�n a trav�s de 
!!	'num_word_prompt', un n�mero que indica el orden de la palabra en el vector 
!!	de entrada, y la palabra de diccionario se pasa a trav�s de 'dictword' 
!!	(hay que volcarla en un vector antes de hacer la comprobaci�n).
!!
!!	Se retorna 1 si las palabras son iguales, o 0 si son diferentes
!!------------------------------------------------------------------------------

[ CompareWord num_word_prompt dictword i len;

	!! A) Se vuelca la palabra de diccionario a un array

	#Ifdef TARGET_ZCODE;
	@output_stream 3 tmp_text;
	print (address)dictword;
	@output_stream -3;
	#Ifnot;	! TARGET_GLULX;
	tmp_text->(WORDSIZE-1) = PrintAnyToArray(tmp_text+WORDSIZE, 60, dictword);
	#Endif; ! TARGET_

	len = tmp_text->(WORDSIZE-1); 

	!! B) Si el ultimo car�cter es una coma, se elimina para evitar conflictos 
	!! con la conversi�n de infitivos y los diccionarios en informATE --> NO 
	!! DEBE HABER NUNCA PALABRAS EN INFINITIVO EN EL DICCIONARIO. No vale para 
	!! palabras que antes de ponerles la coma tengan 9 o m�s caracteres 
	!! (limitaci�n de Inform)

	if (tmp_text->(len+WORDSIZE-1) == ',') {
		tmp_text->(len+WORDSIZE-1) = 0;	! Se elimina el caracter del buffer
		(tmp_text->(WORDSIZE-1))--;		! Se reducen las dimensiones
		len = tmp_text->(WORDSIZE-1);	! Se actualiza el valor de 'len'
	}
	
	#Ifdef DEBUG_TOPICINVENTORY;
	print "Comparando prompt: <", (PrintPromptWord) num_word_prompt, 
	"> con palabra de diccionario:<", (PrintStringArray) tmp_text, ">^";
	#Endif; ! DEBUG_TOPICINVENTORY;

	!! Si la longitud de las palabras no es igual, se retorna NO coincidente. 
	!! (NOTA: Hay que contemplar el caso especial de palabras de m�s de 9 
	!! caracteres por las limitaciones de Inform)
	if (WordLength(num_word_prompt) ~= len && 
			~~(WordLength(num_word_prompt) > 9 && len == 9)) 
		return 0;

	!! Si las palabras tienen la misma longitud, se comparan caracter a 
	!! caracter y se retorna NO coincidente si se encuentra una diferencia
	for (i = 0: i < len: i++) {
		if (WordAddress(num_word_prompt)->i ~= tmp_text->(i+WORDSIZE))
			return 0;
	}

	!! Las palabras son iguales
	return 1;
];

!!==============================================================================
!!	Representa un tema sobre el que se puede hablar en una conversaci�n. Cuando 
!!	el gestor trata un tema, autom�ticamente le da el atributo "visited" para 
!!	marcarlo como tema tratado y al final lo elimina de la conversaci�n. Se 
!!	puede forzar, sin embargo, que un tema est� siempre disponible si en la 
!!	funci�n reaction() se le quita el atributo "visited". Ej:
!!
!!		Object	topic
!!		 class	ConversationTopic
!!		 with	[...]
!!				reaction [; give self ~visited; ];
!!------------------------------------------------------------------------------

Class	ConversationTopic
 with	compare_prompt [i j;
			self.hits = 0;
			for (i = 0 : i < self.#keys/WORDSIZE : i++) {
				for (j = 1 : j <= num_words : j++) {
					if (CompareWord(j, self.&keys-->i)) {
						self.hits++;
					}
				}
			}
		],
		!! N�mero de coincidencias de la entrada del usuario con el tema
		hits 0,
		!! Descripci�n del tema
		entry 0, 
		!! Desarrollo del tema
		reply 0, 
		!! Acciones a ejecutar despu�s de tratar el tema
		reaction 0,
		!! Lista de subtemas que a�adir a la conversacion tras tratar este tema
		subtopics 0, 
		!! Si est� establecido a 'true' se fuerza que el turno en que se trata 
		!! este tema finalice mostrando el inventario de temas disponibles
		append_topic_inventory true;

!!==============================================================================
!!	Representa una conversaci�n que tiene una lista de temas que pueden ser 
!!	tratados a elecci�n del usuario. Implementa las siguientes funciones:
!!
!!	 *	add_topic(topic:ConversationTopic) - A�ade un tema a la conversaci�n (a 
!!		menos que el tema pasado como par�metro est� marcado como ya tratado). 
!!
!!	 *	add_subtopics(topic:ConversationTopic) - A�ade a la conversaci�n los 
!!		subtemas del tema pasado como par�metro (si tiene alguno).
!!
!!	 *	remove_topic(topic:ConversationTopic, visited_flag:boolean) - Elimina 
!!		un tema de la conversaci�n. Si se invoca con *visited_flag* verdadero, 
!!		adem�s se fuerza que el tema quede marcado como tratado aunque no haya 
!!		llegado a mostrarse en pantalla en realidad.
!!
!!	 *	remove_temporal_topics() - Elimina todos los temas temporales de la 
!!		conversaci�n (si es que hay alguno).
!!
!!	 *	topic_inventory_size() - Retorna el n�mero de temas de la conversaci�n.
!!
!!	 *	show_topic_inventory() - Imprime el inventario de temas.
!!
!!	 *	initiate() - Inicializa el inventario de temas de la conversaci�n con 
!!		aquellos temas presentes en las propiedades *topics* y 
!!		*temporal_topics* del objeto.
!!------------------------------------------------------------------------------

Class	Conversation
 with	add_topic [ topic;
			!! Se comprueba que el tema pasado sea v�lido:
			if (topic == 0) return false;
			if (~~(topic ofclass ConversationTopic)) return false;
			if (topic has visited) return false;
			!! Se a�ade el tema a la conversaci�n:
			move topic to self;
			return true;
		],
		add_subtopics [ topic i;
			if ((topic == 0) || ~~(topic provides subtopics)) return false;
			if (topic.subtopics ~= 0) {
				for (i=0 : i<(topic.#subtopics)/WORDSIZE : i++) {
					self.add_topic(topic.&subtopics-->i);
				}
			}
			return true;
		],
		remove_topic [ topic visited_flag;
			if (parent(topic) ~= self) return false;
			if (visited_flag) give topic visited;
			remove topic;
			return true;
		], 
		remove_temporal_topics [ i;
			if (self.temporal_topics ~= 0)
				for (i = 0 : i < (self.#temporal_topics)/WORDSIZE : i++)
					self.remove_topic(self.&temporal_topics-->i);
		],
		topic_inventory_size [ topic size;
			topic = 0; ! para evitar un desconcertante warning del compilador
			objectloop(topic in self) size++;
			return size;
		],
		show_topic_inventory [ topic size;
			!! XXX - Puesto que el temporizador comprueba si se deben eliminar 
			!! los temas temporales al finalizar el turno (es decir, despu�s de 
			!! de mostrar el inventario de temas), puede ocurrir que algunos de 
			!! los temas del listado no est�n en realidad disponibles. Por 
			!! ello, antes de imprimir el inventario, si procede se eliminan 
			!! los temas temporales.
			if (self.time_left == 0) self.remove_temporal_topics();

			if (self.talker ~= 0) {
				switch (metaclass(self.talker)) {
					Object:		print (The) self.talker;
					String:		print (string) self.talker;
					Routine:	indirect(self.talker);
				}
				print ": ";
			}
			size = self.topic_inventory_size();
			if ((size == 0) && (CONVERSATION_NO_MSG ~= 0))
				print (string) CONVERSATION_NO_MSG;
			if ((size > 0) && (CONVERSATION_MSG1 ~= 0))
				print (string) CONVERSATION_MSG1;
			if ((size > 1) && (CONVERSATION_MSG2 ~= 0))
				print (string) CONVERSATION_MSG2;
			!! imprime el listado de temas
			for (topic=child(self) : topic~=nothing : topic=sibling(topic)) {
				PrintOrRun(topic, entry, true);
				if (sibling(topic) ~= nothing) {
					if (sibling(sibling(topic)) == nothing) {
						if (CONVERSATION_OR ~= 0) {
							print (string) CONVERSATION_OR;
						}
					} else print ", ";
				}
			}
			print ".";
			return true;
		],
		initiate [ i;
			if (self.topics ~= 0)
				for (i=0 : i < (self.#topics)/WORDSIZE : i++)
					self.add_topic(self.&topics-->i);
			if (self.temporal_topics ~= 0) {
				for (i=0 : i < (self.#temporal_topics)/WORDSIZE : i++)
					self.add_topic(self.&temporal_topics-->i);
				StartTimer(self, 1);
			}
			return true;
		],
		!! Temas con los que se inicializa la conversaci�n.
		topics 0,
		!! Temas temporales con los que se inicializa la conversaci�n.
		temporal_topics 0,
		!! Permite indicar con qui�n se lleva a cabo la conversaci�n. Puede ser 
		!! ser objeto, string o rutina.
		talker 0,
		!! Mensaje al activar una conversaci�n.
		initial_message 0,
		!! Mensaje al intentar iniciar una conversaci�n ya activada.
		inter_message 0,
		!! Mensaje al intentar iniciar una conversaci�n agotada.
		final_message 0,
		!! Temporizadores.
		time_left -1,
		time_out [;
			self.time_left = -1;
			self.remove_temporal_topics();
		];


!!==============================================================================
!!	Objeto gestor del sistema de conversaci�n. Cuenta con el siguiente conjunto 
!!	de funciones que pueden ser utilizadas por un autor de relatos interactivos 
!!	para manejar conversaciones:
!!
!!	 *	start(conv:Conversation) - Inicia y deja activa en el gestor la 
!!		conversaci�n pasada como par�metro. Retorna verdadero si la 
!!		conversaci�n se activa correctamente, falso si la conversaci�n no es 
!!		v�lida o no tiene temas que tratar.
!!
!!	 *	end() - Quita del gestor la conversaci�n activa.
!!
!!	 *	is_running(conv:Conversation) - Retorna verdadero si la conversaci�n 
!!		pasada como par�metro est� activada en el gestor. Si no se pasan 
!!		par�metros, retorna verdadero si hay una conversaci�n cualquiera 
!!		activada. Retorna falso en otro caso.
!!
!!	 *	topic_inventory_size() - Retorna el n�mero de temas de la conversaci�n 
!!		activa. 
!!
!!	 *	show_topic_inventory() - Muestra el inventario de temas de la 
!!		conversaci�n activa.
!!
!!	 *	try() - Funci�n principal del gestor. Comprueba si la entrada de 
!!		usuario se refiere a alguno de los temas disponibles en la conversaci�n 
!!		activa  y lanza la acci�n adecuada para tratarlo si es as�. Debe 
!!		invocarse desde el punto de entrada *BeforeParsing()*.
!!------------------------------------------------------------------------------

Object ConversationManager "(Conversation Manager)"
 with	start [ conv;
			!! Se comprueba que la conversaci�n pasada sea v�lida:
			if ((conv == 0) || ~~(conv ofclass Conversation)) {
				#Ifdef DEBUG_TOPICINVENTORY;
				print "ERROR. La conversaci�n introducida no es v�lida.^";
				#Endif;
				return false;
			}
			if (self.is_running(conv)) {
				if (conv.inter_message ~= 0) {
					switch (metaclass(conv.inter_message)) {
						String:		print (string) conv.inter_message, "^";
									new_line;
						Routine:	indirect(conv.inter_message);
									new_line;
					}
				}
			} else {
				!! Quita cualquier conversaci�n activa en el gestor:
				self.current_conversation = 0;
				!! Si la conversaci�n no tiene temas, imprime un mensaje de 
				!! final de conversaci�n (si est� definido) y retorna:
				conv.initiate();
				if (~~(child(conv))) {
					if (conv.final_message ~= 0) {
						switch (metaclass(conv.final_message)) {
							String:		print (string) conv.final_message, "^";
										new_line;
							Routine:	indirect(conv.final_message);
										new_line;
						}
					}
					return false;
				}
				!! En este punto se sabe que la conversaci�n tiene temas. Se 
				!! imprime un mensaje de inicio de conversaci�n (si est� 
				!! definido):
				if (conv.initial_message ~= 0) {
					switch (metaclass(conv.initial_message)) {
						String:		print (string) conv.initial_message, "^";
									new_line;
						Routine:	indirect(conv.initial_message);
									new_line;
					}
				}
				!! Activa la conversaci�n en el gestor:
				self.current_conversation = conv;
			}
			!! Imprime el inventario de temas de la conversaci�n activa:
			self.show_topic_inventory();
			return true;
		], 
		end [;
			self.current_conversation = 0;
			return true;
		], 
		is_running [ conv;
			if (conv == nothing)
				return self.current_conversation ~= nothing;
			else return self.current_conversation == conv;
		], 
		topic_inventory_size [;
			return self.current_conversation.topic_inventory_size();
		],
		!! XXX - Requiere la extensi�n types.h v4.X o superior. Se puede 
		!! cambiar por la versi�n alternativa que no hace uso de la extensi�n.
		show_topic_inventory [;
			start_parser_style();
			self.current_conversation.show_topic_inventory();
			end_parser_style();
			new_line;
			return true;
		],
		!! XXX - Versi�n alternativa de la propiedad show_topic_inventory. 
		!! Descomentar si no se quiere utilizar la extensi�n types.h.
!		show_topic_inventory [;
!			switch (CONVERSATION_STYLE) {
!			0:	!! Estilo: Romana
!				#Ifdef TARGET_ZCODE;
!				font_on; style roman;
!				#Ifnot; ! TARGET_GLULX;
!				glk($0086, 0);
!				#Endif;	! TARGET_
!			1:	!! Estilo: It�lica
!				#Ifdef	TARGET_ZCODE;
!				font on; style underline;
!				#Ifnot;	! TARGET_GLULX;
!				glk($0086, 1);
!				#Endif;	! TARGET_
!			2:	!! Estilo: Negrita
!				#Ifdef	TARGET_ZCODE;
!				font on; style bold;
!				#Ifnot;	! TARGET_GLULX;
!				glk($0086, 3);
!				#Endif;	! TARGET_
!			3:	!! Estilo: Monoespaciada
!				#Ifdef	TARGET_ZCODE;
!				font off;
!				#Ifnot;	! TARGET_GLULX;
!				glk($0086, 2);
!				#Endif;	! TARGET_
!			}
!			if (CONVERSATION_PREFIX ~= 0)
!				print (string) CONVERSATION_PREFIX;
!			self.current_conversation.show_topic_inventory();
!			if (CONVERSATION_SUFIX ~= 0)
!				print (string) CONVERSATION_SUFIX;
!			#Ifdef	TARGET_ZCODE;		!!
!			font on; style roman;		!!
!			#Ifnot;	! TARGET_GLULX;		!! Romana
!			glk($0086, 0);				!!
!			#Endif; ! TARGET_			!!
!			new_line;
!			return true;
!		],
		try [ o o_tmp_hits;
			if (self.current_conversation) {

				!! A) Inicializaciones del m�todo
				self.topic_inventory_flag = false; 
				self.hits = 0;
				self.topic = 0;

				!! B) Da un repaso a los temas actuales comprobando si alguno 
				!! encaja con la entrada de usuario
				objectloop (o in self.current_conversation) {
					#Ifdef DEBUG_TOPICINVENTORY;
					print "Probando: ", (string) o.entry, "... ";
					#Endif; ! DEBUG_TOPICINVENTORY;

					!! Se calcula el n�mero de coincidencias del tema en 
					!! relaci�n al total de palabras (%)
					o.compare_prompt();
					o_tmp_hits = (o.hits*100) / num_words; 
					
					#Ifdef DEBUG_TOPICINVENTORY;
					print "Ajuste de ", o.hits, " sobre ", 
					o.#keys / WORDSIZE, " palabras: ", o_tmp_hits, "%^";
					#Endif; ! DEBUG_TOPICINVENTORY;

					!! Si coincide m�s que el m�ximo actual:
					if (o_tmp_hits > self.hits) {
						self.hits = o_tmp_hits;
						self.topic = o;
					}
				}

				!! C) Se muestra la respuesta al tema elegido, si lo hay, se 
				!! quita de la conversaci�n y se a�aden los posibles subtemas
				if (self.hits) {
					#Ifdef DEBUG_TOPICINVENTORY;
					print "Tema seleccionado: ", (string) self.topic.entry;
					new_line;
					#Endif; ! DEBUG_TOPICINVENTORY;

					!! El tema queda marcado como tratado
					give self.topic visited;

					PrintOrRun(self.topic, reply);

					!! Se establece la propiedad 'topic_inventory_flag' del 
					!! sistema en funci�n de las propiedades del tema
					self.topic_inventory_flag
						= self.topic.append_topic_inventory;

					!! Se modifica la entrada de usuario para que la librer�a 
					!! lance la acci�n de apoyo ##NPCTalk (referenciada con la 
					!! palabra clave 'npc.talk')
					parse-->1 = 'npc.talk';
					num_words = 1;

					!! Se elimina el tema seleccionado si est� agotado y se 
					!! a�aden a la conversaci�n todos sus subtemas, si los hay
					if (self.topic has visited)
						self.current_conversation.remove_topic(self.topic);
					self.current_conversation.add_subtopics(self.topic);

					PrintOrRun(self.topic, reaction);

					!! A partir de este punto, la librer�a lanzar� la acci�n 
					!! ##NPCTalk, que imprimir� el inventario de temas 
					!! disponibles (si es preciso) o pondr� fin a la 
					!! conversaci�n actual
				}
			}

			!! Retorna de la funci�n sin hacer nada -> tratamiento normal de 
			!! la entrada del usuario
			return false;
		], 
		get_topic_inventory_flag [;
			return self.topic_inventory_flag;
		],
 private
		!! Tema con mayor porcentaje de coincidencias hasta el momento
		topic 0,
		!! N�mero de coincidencias del tema con m�s coincidencias de la 
		!! conversaci�n (en tanto por 100 sobre el n�mero de palabras clave)
		hits 0, 
		!! Conversaci�n actual activa en el gestor
		current_conversation 0, 
		!! Indica si hay que mostrar el inventario de temas al terminar de 
		!! desarrollar uno de los temas de la conversaci�n actual
		topic_inventory_flag false;

!!==============================================================================
!!	Definici�n de la acci�n de apoyo (y su gram�tica) que se lanza si se 
!!	reconoce en la entrada de usuario un intento de seguir con alguno de los 
!!	temas de la conversaci�n activa en el gestor.
!!------------------------------------------------------------------------------

Verb	'npc.talk'
	*								-> NPCTalk
	* topic							-> NPCTalk
;

[ NPCTalkSub;
	if (ConversationManager.is_running()) {
		if (ConversationManager.topic_inventory_size() > 0) {
			if (ConversationManager.get_topic_inventory_flag()) {
				new_line;
				ConversationManager.show_topic_inventory();
			}
		} else ConversationManager.end();
	}
	return true;
];



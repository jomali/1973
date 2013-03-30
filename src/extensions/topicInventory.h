

!!==============================================================================
!!
!!	TOPIC INVENTORY
!!	Sistema de conversación con inventario de temas
!!
!!==============================================================================
!!
!!	File:			topicInventory.h
!!	Author(s):		J. Francisco Martín (jfm.lisaso@gmail.com)
!!					Mastodon
!!	Language:		ES (Castellano)
!!	System:			Inform-INFSP 6
!!	Platform:		Z-Machine / Glulx
!!	Version:		1.2
!!	Released:		2013/03/30
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2009, Mastodon
!!	Copyright (c) 2013, J. Francisco Martín
!!
!!	Este programa es software libre: usted puede redistribuirlo y/o 
!!	modificarlo bajo los términos de la Licencia Pública General GNU 
!!	publicada por la Fundación para el Software Libre, ya sea la versión 
!!	3 de la Licencia, o (a su elección) cualquier versión posterior.
!!
!!	Este programa se distribuye con la esperanza de que sea útil, pero 
!!	SIN GARANTÍA ALGUNA; ni siquiera la garantía implícita MERCANTIL o 
!!	de APTITUD PARA UN PROPÓSITO DETERMINADO. Consulte los detalles de 
!!	la Licencia Pública General GNU para más información.
!!
!!	Debería haber recibido una copia de la Licencia Pública General GNU 
!!	junto a este programa. En caso contrario, consulte
!!	<http://www.gnu.org/licenses/>.
!!
!!------------------------------------------------------------------------------
!!
!!	# INTRODUCCIÓN
!!
!!	*TOPIC INVENTORY* es una extensión para implementar sistemas de 
!!	conversación utilizando inventario de temas y análisis no estricto de la 
!!	entrada de usuario para el reconocimiento de patrones. Está basada en las 
!!	notas de Eric Eve sobre sistemas de conversación:
!!	<http://www.tads.org/howto/convbkg.htm>, y construida sobre la extensión 
!!	*NPC_Conversacion* v1.0 de Mastodon.
!!
!!
!!	# CONVERSACIONES CON INVENTARIO DE TEMAS
!!
!!	El sistema de conversación con invantario de temas está ideado con el 
!!	objetivo de esquivar los problemas que suelen presentar los sistemas de 
!!	conversación más habituales en los relatos interactivos; dificultades para 
!!	adivinar la palabra clave en sistemas basados en acciones ASK/TELL, 
!!	simplificación excesiva en sistemas basados en la acción TALK TO, o falta 
!!	de libertad y ruptura de la interfaz textual en sistemas de menús. 
!!
!!	Al utilizar un inventario de temas, en esencia, se presenta al usuario una 
!!	lista de temas de conversación sugeridos basándose en el conocimiento del 
!!	personaje protagonista. De esta forma, el usuario puede hacer referencia a 
!!	los temas que quiera lanzar utilizando una interfaz similar a la de los 
!!	sistemas ASK/TELL, pero sin necesidad de tener que adivinar por sí mismo la 
!!	palabra clave que lanza el tema.
!!
!!	En concreto, el sistema se presentaría de forma similar a la siguiente:
!!
!!		> hablar con Bob
!!		---Disculpe...
!!		Bob levanta la vista de su libro y se quita las gafas- ---¿Si?
!!
!!		(Puedes preguntar por la antena, hablarle sobre el faro o hablarle 
!!		del tema que nunca se acaba.)
!!
!!		> hablar sobre el faro
!!		---He pasado junto al faro esta mañana ---empiezas.
!!		Bob se pone muy nervioso de pronto. ---¿De qué estás hablando? Esa cosa 
!!		se vino abajo hace años. Después de aquel... asunto.
!!
!!		> hablar con Bob
!!		Bob simula limpiar las gafas, inquieto. 
!!
!!		(Puedes preguntar por la antena, preguntar por el asunto o hablarle 
!!		del tema que nunca se acaba.)
!!
!!		> asunto
!!		---¿A qué "asunto" te refieres?
!!		Bob se coloca las gafas y vuelve de nuevo su atención al libro, 
!!		claramente agitado. ---T-Tengo mucho trabajo que hacer. Si me 
!!		disculpas...
!!
!!		(Puedes insistir con el "asunto", preguntar por la antena o hablarle 
!!		del tema que nunca se acaba.)
!!
!!		> tema interminable
!!		---Tío, ¿dónde está mi coche?
!!		---¿Y tu coche, tío?
!!
!!		(Puedes insistir con el "asunto", preguntar por la antena o hablarle 
!!		del tema que nunca se acaba.)
!!
!!	El reconocimiento de patrones entre la entrada de usuario y las claves de 
!!	cada tema se lleva a cabo por medio de análisis no estricto; si entre todas 
!!	las palabras introducidas se reconocen claves de uno o más temas, se lanza 
!!	aquel tema con más coincidencias ignorando el resto de la entrada.
!!
!!
!!	# SOBRE LOS MENSAJES
!!
!!	Se puede definir una variable GRAMMATICAL_INFLECTION en el archivo de 
!!	juego que puede tomar los valores: 1 (PRESENTE 1ª PERSONA), 2 (PRESENTE 2ª 
!!	PERSONA), 3 (PRESENTE 3ª PERSONA), 4 (PASADO 1ª PERSONA), 5 (PASADO 2ª 
!!	PERSONA) o 6 (PASADO 3ª PERSONA) para modificar la conjugación gramatical 
!!	de los mensajes de la librería. Si no se define esta variable, la librería 
!!	interpreta que se usa el valor 2 con lo que los mensajes se imprimen en 
!!	presente y segunda persona.
!!
!!
!!	# UTILIZACIÓN
!!
!!	Para usar la librería únicamente hay que incluir la siguiente línea en la 
!!	rutina BeforeParsing (crearla si no existe):
!!
!!		ConversationManager.try();
!!
!!
!!	# LIMITACIONES Y POSIBLES MEJORAS
!!
!!	Podría ser interesante contemplar la posibilidad de crear temas que no 
!!	aparezcan sugeridos automáticamente al imprimir el inventario de temas 
!!	disponibles.
!!
!!------------------------------------------------------------------------------
System_file;

!! Descomentar para obtener información de depuración:
!Constant DEBUG_CONVERSATION;

!! Vector para guardar palabras temporalmente:
Array tmp_text -> 64;

!! Flexión gramatical utilizada por defecto: Presente 2ª persona
Default GRAMMATICAL_INFLECTION 2;

!!==============================================================================
!!	Funciones de depuración
!!------------------------------------------------------------------------------

#Ifdef DEBUG_CONVERSATION;

!! Función para pintar un String Array
[ PrintStringArray the_array i;
	print "(", the_array-->0, ")";
	for (i = WORDSIZE : i < (the_array-->0) + WORDSIZE : i++) 
		print (char) the_array->i;
];

!! Función para pintar una palabra del Prompt de entrada del jugador
[ PrintPromptWord num_word dir i;
	dir = WordAddress(num_word);
	for (i = 0 : i < WordLength(num_word) : i++)
		print (char) dir->i;
];

#Endif; ! DEBUG_CONVERSATION;

!!==============================================================================
!!	Compara una palabra de la entrada del usuario con una de las palabras de 
!!	diccionario. La palabra de entrada se pasa a la función a través de 
!!	'num_word_prompt', un número que indica el orden de la palabra en el vector 
!!	de entrada, y la palabra de diccionario se pasa a través de 'dictword' 
!!	(hay que volcarla en un vector antes de hacer la comprobación).
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

	!! B) Si el ultimo carácter es una coma, se elimina para evitar conflictos 
	!! con la conversión de infitivos y los diccionarios en informATE --> NO 
	!! DEBE HABER NUNCA PALABRAS EN INFINITIVO EN EL DICCIONARIO. No vale para 
	!! palabras que antes de ponerles la coma tengan 9 o más caracteres 
	!! (limitación de Inform)

	if (tmp_text->(len+WORDSIZE-1) == ',') {
		tmp_text->(len+WORDSIZE-1) = 0;	! Se elimina el caracter del buffer
		(tmp_text->(WORDSIZE-1))--;		! Se reducen las dimensiones
		len = tmp_text->(WORDSIZE-1);	! Se actualiza el valor de 'len'
	}
	
	#Ifdef DEBUG_CONVERSATION;
	print "Comparando prompt: <", (PrintPromptWord) num_word_prompt, 
	"> con palabra de diccionario:<", (PrintStringArray) tmp_text, ">^";
	#Endif; ! DEBUG_CONVERSATION;

	!! Si la longitud de las palabras no es igual, se retorna NO coincidente. 
	!! (NOTA: Hay que contemplar el caso especial de palabras de más de 9 
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
!!	Representa un tema sobre el que se puede hablar en una conversación. Cuando 
!!	el gestor trata un tema, automáticamente le da el atributo "visited" para 
!!	marcarlo como tema tratado y al final lo elimina de la conversación. Se 
!!	puede forzar, sin embargo, que un tema esté siempre disponible si en la 
!!	función reaction() se le quita el atributo "visited". Ej:
!!
!!		Object	topic
!!		 class	TI_Topic
!!		 with
!!				[...]
!!				reaction [; give self ~visited; ];
!!------------------------------------------------------------------------------

Class TI_Topic
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
		!! Número de coincidencias de la entrada del usuario con el tema
		hits 0,
		!! Acciones a ejecutar después de tratar el tema
		reaction 0,
		!! Establecer a 'true' para forzar que el turno en que se trata este 
		!! tema finalice mostrando el inventario de temas disponibles
		append_topic_inventory false,
;

!!==============================================================================
!!	Representa una conversación que tiene una lista de temas que pueden ser 
!!	tratados a elección del usuario. Implementa métodos para añadir, eliminar y 
!!	manipular la lista de temas disponible
!!------------------------------------------------------------------------------

Class	TI_Conversation
 with	!! Mueve un tema a la conversación (si no está marcado como ya tratado)
		add_topic [ topic;
			if (topic hasnt visited)
				move topic to self;
		],
		!! Mueve a la conversación los subtemas de 'topic'
		add_subtopics [ topic i;
			for (i = 0 : i < topic.#subtopics/WORDSIZE : i++)
				self.add_topic(topic.&subtopics-->i);
		],
		!! Elimina un tema de la conversación
		remove_topic [ topic;
			remove topic;
		], 
		!! Imprime la lista de temas de la conversación
		show_topic_list [ topic;
			for (topic=child(self) : topic~=nothing : topic=sibling(topic)) {
				PrintOrRun(topic, entry, true);
				if (sibling(topic) ~= nothing) 
					if (sibling(sibling(topic)) == nothing)
						print " o ";
					else
						print ", ";	
			}
		],
		!! Se inicializa la lista de temas de la conversación con aquellos 
		!! temas presentes en 'topics' que no hayan sido tratados previamente
		initiate [ topic i;
			!! primero se asegura que no haya temas en la conversación:
			objectloop (topic in self)
				self.remove_topic(topic);
			!! se inicializa la lista de temas de la conversación:
			for (i = 0 : i < self.#topics/WORDSIZE : i++)
				self.add_topic(self.&topics-->i);
		],
;

!!==============================================================================
!!	Objeto gestor del sistema de conversación. Cuenta con el siguiente conjunto 
!!	de funciones que pueden ser utilizadas por un programador de relatos 
!!	interactivos para manejar conversaciones:
!!
!!	*	start(conversation) : Activa en el gestor la conversación pasada como 
!!		argumento.
!!	*	end() : Quita la conversación activa del gestor.
!!	*	is_running() : Indica si hay una conversación activa en el gestor. 
!!		Retorna verdadero si es así, o falso en caso contrario.
!!	*	topic_inventory_size() : Retorna el número de temas disponibles en la 
!!		conversación activa.
!!	*	show_topic_inventory() : Muestra el inventario de temas disponibles.
!!	*	try() : Función principal del gestor. Comprueba si la entrada de 
!!		usuario se refiere a alguno de los temas disponibles y lanza la acción 
!!		adecuada para tratarlo si es así. Debe invocarse desde el punto de 
!!		entrada "BeforeParsing()" del relato.
!!------------------------------------------------------------------------------

Object ConversationManager
 with		!! INTERFAZ DEL GESTOR
		start [ conversation;
			self.current_conversation = conversation;
			self.current_conversation.initiate();
			self.show_topic_inventory();
		], 
		end [;
			self.current_conversation = 0;
		], 
		is_running [;
			return self.current_conversation ~= nothing;
		], 
		topic_inventory_size [ o size;
			objectloop (o in self.current_conversation)
				size++;
			return size;
		], 
		show_topic_inventory [;
			#Ifdef	TARGET_ZCODE;		!!
			font on; style underline;	!!
			#Ifnot;	! TARGET_GLULX;		!! Itálica
			glk($0086, 1);				!!
			#Endif; ! TARGET_			!!
			switch (GRAMMATICAL_INFLECTION) {
			1:	print "(Puedo ";
			2:	print "(Puedes ";
			3:	print "(Puede ";
			4:	print "(Podía ";
			5:	print "(Podías ";
			6:	print "(Podía ";
			}
			self.current_conversation.show_topic_list();
			print ".)^";
			#Ifdef	TARGET_ZCODE;		!!
			font on; style roman;		!!
			#Ifnot;	! TARGET_GLULX;		!! Romana
			glk($0086, 0);				!!
			#Endif; ! TARGET_			!!
			return true;
		], 
		try [ o o_tmp_hits;
			if (self.current_conversation) {

				!! A) Inicializaciones del método
				self.topic_inventory_flag = false; 
				self.hits = 0;
				self.topic = 0;

				!! B) Da un repaso a los temas actuales comprobando si alguno 
				!! encaja con la entrada de usuario
				objectloop (o in self.current_conversation) {
					#Ifdef DEBUG_CONVERSATION;
					print "Probando: ", (string) o.entry, "... ";
					#Endif; ! DEBUG_CONVERSATION;

					!! Se calcula el número de coincidencias del tema en 
					!! relación al total de palabras (%)
					o.compare_prompt();
					o_tmp_hits = (o.hits*100) / num_words; 
					
					#Ifdef DEBUG_CONVERSATION;
					print "Ajuste de ", o.hits, " sobre ", 
					o.#keys / WORDSIZE, " palabras: ", o_tmp_hits, "%^";
					#Endif; ! DEBUG_CONVERSATION;

					!! Si coincide más que el máximo actual:
					if (o_tmp_hits > self.hits) {
						self.hits = o_tmp_hits;
						self.topic = o;
					}
				}

				!! C) Se muestra la respuesta al tema elegido, si lo hay, se 
				!! quita de la conversación y se añaden los posibles subtemas
				if (self.hits) {
					#Ifdef DEBUG_CONVERSATION;
					print "Tema seleccionado: ", (string) self.topic.entry;
					new_line;
					#Endif; ! DEBUG_CONVERSATION;

					!! El tema queda marcado como tratado
					give self.topic visited;

					PrintOrRun(self.topic, reply);
					PrintOrRun(self.topic, reaction);

					!! Se establece la propiedad 'topic_inventory_flag' del 
					!! sistema en función de las propiedades del tema
					self.topic_inventory_flag
						= self.topic.append_topic_inventory;

					!! Se modifica la entrada de usuario para que la librería 
					!! lance la acción de apoyo ##NPCTalk (referenciada con la 
					!! palabra clave 'npc.talk')
					parse-->1 = 'npc.talk';
					num_words = 1;

					!! Se elimina el tema seleccionado si está agotado y se 
					!! añaden a la conversación todos sus subtemas, si los hay
					if (self.topic has visited)
						self.current_conversation.remove_topic(self.topic);
					self.current_conversation.add_subtopics(self.topic);

					!! A partir de este punto, la librería lanzará la acción 
					!! ##NPCTalk, que imprimirá el inventario de temas 
					!! disponibles (si es preciso) o pondrá fin a la 
					!! conversación actual
				}
			}

			!! Retorna de la función sin hacer nada -> tratamiento normal de 
			!! la entrada del usuario
			return false;
		], 
			!! OTRAS FUNCIONES DE APOYO
		get_topic_inventory_flag [;
			return self.topic_inventory_flag;
		],
 private	!! PROPIEDADES PRIVADAS
		!! Tema con mayor porcentaje de coincidencias hasta el momento
		topic 0,
		!! Número de coincidencias del tema con más coincidencias de la 
		!! conversación (en tanto por 100 sobre el número de palabras clave)
		hits 0, 
		!! Conversación actual activa en el gestor
		current_conversation 0, 
		!! Indica si hay que mostrar el inventario de temas al terminar de 
		!! desarrollar uno de los temas de la conversación actual
		topic_inventory_flag false, 
;

!!==============================================================================
!!	Definición de la acción de apoyo (y su gramática) que se lanza si se 
!!	reconoce en la entrada de usuario un intento de seguir con alguno de los 
!!	temas de la conversación activa en el gestor
!!------------------------------------------------------------------------------

Verb	'npc.talk'
	*								-> NPCTalk
	* topic							-> NPCTalk
;

[ NPCTalkSub;
	if (ConversationManager.is_running()) {
		if (ConversationManager.get_topic_inventory_flag()) {
			new_line;
			ConversationManager.show_topic_inventory();
		}
	} else
		ConversationManager.end();
	return true;
];



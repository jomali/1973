

!!==============================================================================
!!
!!	TOPIC INVENTORY
!!	Sistema de conversaci�n con inventario de temas
!!
!!==============================================================================
!!
!!	File:			topicInventory.h
!!	Author(s):		J. Francisco Mart�n (jfm.lisaso@gmail.com)
!!					Mastodon
!!	Language:		ES (Castellano)
!!	System:			Inform-INFSP 6
!!	Platform:		Z-Machine / Glulx
!!	Version:		0.2
!!	Released:		2013/03/28
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2009, Mastodon
!!	Copyright (c) 2013, J. Francisco Mart�n
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
!!	<topicInventory> es una extensi�n para implementar sistemas de conversaci�n 
!!	utilizando inventario de temas y an�lisis ligero de la entrada de usuario 
!!	para el reconocimiento de patrones. Est� basada en las notas de Eric Eve 
!!	sobre sistemas de conversaci�n: <http://www.tads.org/howto/convbkg.htm> y 
!!	construida sobre la extensi�n <NPC_Conversacion> v1.0 de Mastodon.
!!
!!
!!	# CONVERSACIONES CON INVENTARIO DE TEMAS
!!
!!	El sistema de conversaci�n con invantario de temas est� ideado con el 
!!	objetivo de esquivar los problemas que suelen presentar los sistemas de 
!!	conversaci�n m�s habituales en los relatos interactivos; los problemas de 
!!	adivinar la palabra clave en sistemas basados en acciones ASK/TELL, la 
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
!!		sobre la rata gigante.)
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
!!		sobre la rata gigante.)
!!
!!		> asunto
!!		---�A qu� "asunto" te refieres?
!!		Bob se coloca las gafas y vuelve de nuevo su atenci�n al libro, 
!!		claramente agitado. ---T-Tengo mucho trabajo que hacer. Si me 
!!		disculpas...
!!
!!		(Puedes insistir en el tema del asunto, preguntar por la antena o 
!!		preguntarle por su trabajo.)
!!
!!	El reconocimiento de patrones entre la entrada de usuario y las claves de 
!!	cada tema se lleva a cabo por medio de an�lisis ligero; si entre todas las 
!!	palabras introducidas se reconocen claves de uno o m�s temas, se lanza 
!!	aquel tema con m�s coincidencias ignorando el resto de la entrada.
!!
!!
!!	# SOBRE LOS MENSAJES
!!
!!	Se puede definir una variable GRAMMATICAL_INFLECTION en el archivo de 
!!	juego que puede tomar los valores: 1 (PRESENTE 1� PERSONA), 2 (PRESENTE 2� 
!!	PERSONA), 3 (PRESENTE 3� PERSONA), 4 (PASADO 1� PERSONA), 5 (PASADO 2� 
!!	PERSONA) o 6 (PASADO 3� PERSONA) para modificar la conjugaci�n gramatical 
!!	de los mensajes de la librer�a. Si no se define esta variable, la librer�a 
!!	interpreta que se usa el valor 2 con lo que los mensajes se imprimen en 
!!	presente y segunda persona.
!!
!!
!!	# UTILIZACI�N
!!
!!	Para usar la librer�a �nicamente hay que incluir la siguiente l�nea en la 
!!	rutina BeforeParsing (crearla si no existe):
!!
!!		ConversationManager.try();
!!
!!------------------------------------------------------------------------------
System_file;

!! Flexi�n gramatical utilizada por defecto: Presente 2� persona
Default GRAMMATICAL_INFLECTION 2;

!! Descomentar para obtener informaci�n de depuraci�n:
!Constant DEBUG_CONVERSATION;

!! Vector para guardar palabras temporalmente:
Array tmp_text1 -> 64;

!!==============================================================================
!!	Funciones de depuraci�n
!!------------------------------------------------------------------------------

#Ifdef DEBUG_CONVERSATION;

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

#Endif; ! DEBUG_CONVERSATION;

!!==============================================================================
!!	Compara una entrada en array (del prompt, indicada por el n�mero de orden 
!!	de la palabra) con una palabra de diccionario.
!!	Devuelve 1 si coinciden, 0 si diferentes
!!------------------------------------------------------------------------------

[ CompareWord num_word_prompt dictword i len;

	!! A) Volcamos la palabra de diccionario a un array
	#Ifdef TARGET_ZCODE;
	@output_stream 3 tmp_text1;
	print (address)dictword; ! vuelca la palabra de diccionario en el array
	@output_stream -3;
	#Endif;

	#Ifdef TARGET_GLULX;
	tmp_text1->(WORDSIZE-1) = PrintAnyToArray(tmp_text1+WORDSIZE, 60, dictword);
	#Endif;

	len = tmp_text1->(WORDSIZE-1); 

	!! B) Si el ultimo car�cter es una coma, se elimina para evitar conflictos 
	!! con la conversi�n de infitivos y los diccionarios en informATE --> NO 
	!! DEBE HABER NUNCA PALABRAS EN INFINITIVO EN EL DICCIONARIO. No vale para 
	!! palabras que antes de ponerles la coma tengan 9 o m�s caracteres 
	!! (limitaci�n de inform)
	if (tmp_text1->(len+WORDSIZE-1) == ',') {
		tmp_text1->(len+WORDSIZE-1) = 0; ! Eliminamos el ; del buffer
		(tmp_text1->(WORDSIZE-1))--; ! Reducimos el tama�o indicado en el buffer
		len = tmp_text1->(WORDSIZE-1); ! Y actualizamos la long. al nuevo valor
	}
	
	#Ifdef DEBUG_CONVERSATION;
	print "Comparando prompt: <", (PrintPromptWord) num_word_prompt, 
	"> con palabra de diccionario:<", (PrintStringArray) tmp_text1, ">^";
	#Endif;
	
	!! Si tienen diferentes longitudes devuelve NO coincidente, tratando el 
	!! caso especial de la limitacion a 9 caracteres de las palabras de 
	!! diccionario en Inform
	if (WordLength(num_word_prompt) ~= len && 
			~~(WordLength(num_word_prompt) >9 && len == 9)) 
		return 0;
		
	!! Para la misma longitud, miramos letra a letra buscando diferencias
	for (i = 0: i < len: i++) {
		if (WordAddress(num_word_prompt)->i ~= tmp_text1->(i+WORDSIZE))
			return 0; ! Letra diferente, retorno que son diferentes
	}
	
	!! Son iguales
	return 1;
];

!!==============================================================================
!!	Representa un tema sobre el que se puede hablar en una conversaci�n. Puede 
!!	tener los atributos 'general' si el tema ya ha sido tratado, y 'concealed' 
!!	para evitar que aparezca listado en el inventario de temas
!!------------------------------------------------------------------------------

Class TCS_Topic
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
		!! Acci�n a ejecutar (opcional) tras haber desarrollado el tema
		reaction 0,
!! XXX
		!! Establecer a 'true' para forzar que el turno en que se trata este 
		!! tema finalice mostrando el inventario de temas disponibles
		append_topic_inventory false, 
;

!!==============================================================================
!!	Representa una conversaci�n que tiene una lista de temas que pueden ser 
!!	tratados a elecci�n del usuario. Implementa m�todos para a�adir, eliminar y 
!!	manipular la lista de temas disponible
!!------------------------------------------------------------------------------

Class	TCS_Conversation
 with	!! Se inicializa la lista de temas de la conversaci�n con aquellos 
		!! temas presentes en 'topics' que no hayan sido tratados previamente
		iniciar [ i;
			self.finalizar(); ! Borramos todo por si acaso
			for (i = 0 : i < self.#topics/WORDSIZE : i++) {
				if (self.&topics-->i hasnt general) {
					!print "Move:", (string)(self.&topics-->i).entry, "^";
					move self.&topics-->i to self;
				}
			}
		],
		!! Elimina todos los temas que hay en la conversaci�n
		finalizar [ topic;
			objectloop (topic in self) 
				remove topic;
		],
!! TODO: no mostrar temas con 'concealed' activado
		!! Imprime en pantalla la lista de temas de la conversaci�n
		show_topic_list [ o;
			for (o = child(self) : o ~= nothing : o = sibling(o)) {
				PrintOrRun(o,entry, true);
				if (sibling(o) ~= nothing) 
					if (sibling(sibling(o)) == nothing)
						print " o ";
					else
						print ", ";	
			}
		],
		!! Mueve el tema 'topic' a la conversaci�n
		add_topic [ topic; 
			move topic to self;
		],
		!! Mueve a la conversaci�n todos aquellos subtemas del tema 'topic' 
		!! pasado como argumento que no hayan sido tratados previamente
		add_subtopics [ topic i;
			for (i = 0 : i < topic.#subtopics/WORDSIZE : i++) {
				if (topic.&subtopics-->i hasnt general) {
					move topic.&subtopics-->i to self;
				}
			}
		],
		!! Elimina el tema 'topic'
		remove_topic [ topic;
			remove topic;
		], 
;

!!==============================================================================
!!	Objeto gestor del sistema de conversaci�n
!!------------------------------------------------------------------------------

Object ConversationManager
 with	start [ conversation;
			self.current_conversation = conversation;
			self.current_conversation.iniciar();
			self.ask();
		],
		end [;
			self.current_conversation = 0;
		],
		try [ o o_tmp_hits;
			if (self.current_conversation) {

				self.topic_inventory_flag = false;

				!! B) Da un repaso a los temas actuales comprobando si alguno 
				!! encaja con la entrada de usuario
				self.hits = 0; 
				self.topic = 0;
				objectloop (o in self.current_conversation) {
					#Ifdef DEBUG_CONVERSATION;
					print "Probando: ", (string) o.entry, "... ";
					#Endif;

					!! Se calcula el n�mero de coincidencias del tema en 
					!! relaci�n al total de palabras (%)
					o.compare_prompt();
					!o_tmp_hits = (o.hits*100)/(o.#keys*WORDSIZE); 
					o_tmp_hits = (o.hits*100) / num_words; 
					
					#Ifdef DEBUG_CONVERSATION;
					print "Ajuste de ", o.hits, " sobre ", 
					o.#keys / WORDSIZE, " palabras: ", o_tmp_hits, "%^";
					#Endif;

					!! Si coincide m�s que el m�ximo actual:
					if (o_tmp_hits > self.hits) {
						self.hits = o_tmp_hits;
						self.topic = o;
					}
				}

				!! C) Se muestra la respuesta al tema elegido, si lo hay, se 
				!! quita de la conversaci�n y se a�aden los posibles subtemas
				if (self.hits) {
					#Ifdef DEBUG_CONVERSATION;
					print "Tema seleccionado: ", (string) self.topic.entry;
					new_line;
					#Endif;

					!! Se le da el atributo general para indicar que se trata 
					!! de un tema ya tratado
					give self.topic general;

					PrintOrRun(self.topic, reply);
					PrintOrRun(self.topic, reaction);

!! XXX
					!! Se establece la propiedad 'topic_inventory_flag' del 
					!! sistema en funci�n de las propiedades del tema
					self.topic_inventory_flag
						= self.topic.append_topic_inventory;

					!! Se modifica la entrada de usuario para que la librer�a 
					!! lance la acci�n de apoyo ##NPCTalk (referenciada con la 
					!! palabra clave 'npc.talk')
					parse-->1 = 'npc.talk';
					num_words = 1;

					!! Se a�aden a la conversaci�n todos los posibles subtemas 
					!! del tema seleccionado y �ste se borra autom�ticamente si 
					!! es necesario
					self.current_conversation.remove_topic(self.topic);
					self.current_conversation.add_subtopics(self.topic);

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
		!! Tema con mayor porcentaje de coincidencias hasta el momento
		topic 0,
		!! N�mero de coincidencias del tema con m�s coincidencias de la 
		!! conversaci�n (en tanto por 100 sobre el n�mero de palabras clave)
		hits 0, 
		!! Conversaci�n actual activa en el gestor
		current_conversation 0, 

		!! XXX: 
		!! Indica si hay que mostrar el inventario de temas al terminar de 
		!! desarrollar uno de los temas de la conversaci�n actual
		topic_inventory_flag false, 

		ask [;
			#Ifdef	TARGET_ZCODE;		!!
			font on; style underline;	!!
			#Ifnot;	! TARGET_GLULX;		!! It�lica
			glk($0086, 1);				!!
			#Endif; ! TARGET_			!!
			switch (GRAMMATICAL_INFLECTION) {
			1:	print "(Puedo ";
			2:	print "(Puedes ";
			3:	print "(Puede ";
			4:	print "(Pod�a ";
			5:	print "(Pod�as ";
			6:	print "(Pod�a ";
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
;

!!==============================================================================
!!	Definici�n de la acci�n de apoyo (y su gram�tica) que se lanza si se 
!!	reconoce en la entrada de usuario un intento de seguir con alguno de los 
!!	temas de la conversaci�n activa en el gestor
!!------------------------------------------------------------------------------

Verb	'npc.talk'
	*								-> NPCTalk
	* topic							-> NPCTalk
;

[ NPCTalkSub;
	! Tras responder a una pregunta, vuelve a mostrar la lista de temas
	if (child(ConversationManager.current_conversation) ~= nothing) {
		if (ConversationManager.topic_inventory_flag) {
			new_line;
			ConversationManager.ask();
		}
	} else {
!		PrintOrRun(ConversationManager, no_topics);
		ConversationManager.end();
	}
	return true;
];



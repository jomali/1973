

!!==============================================================================
!!
!!	TAG CONVERSATION SYSTEM
!!	Sistema de conversaci�n por etiquetas
!!
!!==============================================================================
!!
!!	File:			tagConversationSystem.h
!!	Author(s):		J. Francisco Mart�n (jfm.lisaso@gmail.com)
!!					Mastodon
!!	Languague:		ES (Castellano)
!!	System:			Inform, INFSP 6
!!	Platform:		Z-Machine / GLULX
!!	Version:		3.1
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
!!	'tagConversationSystem' es una remodelaci�n de la extensi�n 
!!	'NPC_Conversacion' versi�n 1.0 (2010/04/19) escrita por Mastodon. Tan s�lo 
!!	presenta peque�as modificaciones con respecto a la original (altera ciertos 
!!	comportamientos y cambia el nombre de algunas funciones y objetos). A 
!!	continuaci�n se adjunta la descripci�n original de la librer�a:
!!
!!	Librer�a gen�rica para tratamiento de conversaciones al estilo de Eric Eve 
!!	(ver Shelter from the Storm). El resultado es igual a la cl�sica por men�s 
!!	de LucasArts, pero la forma de presentarla y de introducir las opciones es 
!!	diferente y m�s "natural". Transcripci�n de ejemplo:
!!
!!		> hablar con pirata
!!		Inicias una interesante conversaci�n con el pirata patapalo. 
!!		"Buenas, grumete, �qu� te trae por aqu�?"
!!		(Se te ocurre hablarle de la busqueda del tesoro, preguntarle por la 
!!		isla secreta, o cambiar de tema)
!!		> hablar del tesoro
!!		"Ver�s, pirata", le dices, "hace tiempo que estoy buscando el famoso 
!!		tesoro, �sabes algo de el?"
!!		"Mi querido marinero de agua dulce", te responde con sorna, "a ti te lo 
!!		voy a decir... salvo... quiz�s puedas hacerme un favor y entonces me 
!!		pensar�a ayudarte".
!!		(Consideras aceptar el trato, preguntarle por la isla secreta, o 
!!		cambiar de tema)
!!		> cambio de tema
!!		"Pues parece que va a llover por aqu�", cortas, mirando al cielo.
!!		"Ahh, veo que eres un chico listo, creo que vamos a hacer buenas migas."
!!		... etc ...
!!
!!	El reconocimiento de patrones se hace mediante parseado ligero y 
!!	comparaci�n entre la entrada del jugador y la definici�n de cada tema, de 
!!	forma que la librer�a sabe interpretar (casi siempre) la intenci�n del 
!!	jugador por una serie de palabras clave indicadas en los temas. Se ponen 
!!	directamente en el prompt sin necesidad de definir nuevas gram�ticas.
!!
!!	## USO 
!!
!!	Para usar la librer�a �nicamente hay que incluir la siguiente l�nea en la 
!!	rutina BeforeParsing (crearla si no existe):
!!
!!		TagConversationSystem.try();
!!
!!	## SOBRE LOS MENSAJES
!!
!!	Se puede definir una variable GRAMMATICAL_INFLECTION en el archivo de 
!!	juego que puede tomar los valores: 1 (PRESENTE 1� PERSONA), 2 (PRESENTE 2� 
!!	PERSONA), 3 (PRESENTE 3� PERSONA), 4 (PASADO 1� PERSONA), 5 (PASADO 2� 
!!	PERSONA) o 6 (PASADO 3� PERSONA) para modificar la conjugaci�n gramatical 
!!	de los mensajes de la librer�a. Si no se define esta variable, la librer�a 
!!	interpreta que se usa el valor 2 con lo que los mensajes se imprimen en 
!!	presente y segunda persona.
!!
!!------------------------------------------------------------------------------
System_file;

!! Flexi�n gramatical por defecto: Presente 2� persona
Default GRAMMATICAL_INFLECTION 2;

!! Descomentar para obtener informacion de depuraci�n:
!Constant DEBUG_CONVERSATION;

!! Array para guardar palabras temporalmente;
Array tmp_text1 -> 64;

!!------------------------------------------------------------------------------
!! Funciones de depuraci�n:
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
#Endif;

!!------------------------------------------------------------------------------
!! Compara una entrada en array (del prompt, indicada por el n�mero de orden de 
!! la palabra) con una palabra de diccionario.
!! Devuelve 1 si coinciden, 0 si diferentes
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

!!------------------------------------------------------------------------------

Class	TCS_Conversation
 with	iniciar [ i;
			self.finalizar(); ! Borramos todo por si acaso
			for (i = 0 : i < self.#temas/WORDSIZE : i++) {
				!! XXX
				if (self.&temas-->i hasnt general) {
					!print "Move:", (string)(self.&temas-->i).entry, "^";
					move self.&temas-->i to self;
				}
			}
		],
		finalizar [ o; ! Borra todos los item actuales en la conversaci�n
			objectloop (o in self) 
				remove o;
		],
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
		add_topic [ topic; 
			move topic to self;
		],
		add_subtopics [ topic i;
			for (i = 0 : i < topic.#temas/WORDSIZE : i++) {
				!! XXX
				if (topic.&temas-->i hasnt general) {
					move topic.&temas-->i to self;
				}
			}
		],
		remove_topic [ topic;
			remove topic;
		],
;

!!------------------------------------------------------------------------------

!! El atributo general de los temas se activa al ser tratados en una conv.
Class TCS_Topic
 with	compare_prompt [i j;
			self.hits = 0;
			for (i = 0 : i < self.#keys/WORDSIZE : i++) {
				for (j = 1 : j <= num_words : j++)
					if (CompareWord(j, self.&keys-->i)) 
						self.hits++;
			}
		],
		!! N�mero de palabras de este tema que coinciden con la entrada del 
		!! jugador
		hits 0, 
		!! Acci�n a ejecutar (opcional) tras haber le�do �ste tema
		reaction [; ],
;

!!------------------------------------------------------------------------------
!! TODO: eliminar accion falsa 'npc_talk' y gestionar todo el sistema desde 
!! el punto de entrada BeforeParsing

Object TagConversationSystem
 with	start [ conversacion;
			self.select(conversacion);
			self.current_conversation.iniciar();
!			PrintOrRun(self.current_conversation, inicio);
!			print "^";
			self.ask();
		],
		end [;
!			PrintOrRun(self.current_conversation, fin);
			self.current_conversation = 0;
		],
		select [ conversacion;
			self.current_conversation = conversacion;
		],
		try [ o o_tmp_hits;
			if (self.current_conversation) {

				!! A) Da un repaso a los temas actuales intentando encajar con 
				!! el prompt
				self.hits = 0; 
				self.topic = 0;
				objectloop (o in self.current_conversation) {
					#Ifdef DEBUG_CONVERSATION;
					print "Probando: ", (string) o.entry, "... ";
					#Endif;

					!! Calculamos el n�mero de coincidencias del tema en 
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

				!! B) Mostrar la respuesta al tema elegido, si lo hay, y a�adir 
				!! los subtemas
				if (self.hits) {
					#Ifdef DEBUG_CONVERSATION;
					print "Tema seleccionado: ", (string) self.topic.entry;
					new_line;
					#Endif;
					!! XXX
					give self.topic general;
					PrintOrRun(self.topic, reply);
					new_line;
					PrintOrRun(self.topic, reaction);

					!! Sustituimos la accion tecleada por una fake que no 
					!! se diga nada
					parse-->1 = 'npc_talk,';
					num_words = 1;

					!! Por �ltimo a�adimos los nuevos temas, si los hay, y 
					!! preguntamos de nuevo
					self.current_conversation.remove_topic(self.topic);
					self.current_conversation.add_subtopics(self.topic);
					
					!! Ahora la acci�n NPC_OK_Talk toma el control para mostrar 
					!! la nueva lista de temas

				}
			}

			!! Retorna de la funci�n sin hacer nada -> tratamiento normal de 
			!! la entrada del usuario
			return false;	
		],
		!! Hits del tema con m�s coincidencias (en tanto por 100 sobre el 
		!! n�mero de palabras del tema)
		hits 0,
		!! Tema con m�s porcentaje de coincidencia hasta el momento
		topic 0,
		current_conversation 0,
!		questions 
!			"Se te ocurre" "Piensas en" "Podr�as" "Ser�a posible" 
!			"No ser�a mala idea",
!		ask [ n;
!			n = random(self.#questions/WORDSIZE) - 1;
!			print "(", (string)self.&questions-->n, " ";
!			self.current_conversation.show_topic_list();
!			")";
!		],
		no_topics [;
			#Ifdef	TARGET_ZCODE;		!!
			font on; style underline;	!!
			#Ifnot;	! TARGET_GLULX;		!! It�lica
			glk($0086, 1);				!!
			#Endif; ! TARGET_			!!
			switch (GRAMMATICAL_INFLECTION) {
			1,2,3:	print "(Se han agotado los temas.)^";
			4,5,6:	print "(Se hab�an agotado los temas.)^";
			}
			#Ifdef	TARGET_ZCODE;		!!
			font on; style roman;		!!
			#Ifnot;	! TARGET_GLULX;		!! Romana
			glk($0086, 0);				!!
			#Endif; ! TARGET_			!!
			return true;
		],
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

!!------------------------------------------------------------------------------
!! Gram�tica de pega que se activa cuando se proporciona una de las opciones 
!! v�lidas (muestra la lista de temas pendientes)
!!------------------------------------------------------------------------------

Verb 'npc_talk,'
  *                                   -> NPC_Talk_OK
  * topic							  -> NPC_Talk_OK
;

[ NPC_Talk_OKSub;
	! Tras responder a una pregunta, vuelve a mostrar la lista de temas
	if (child(TagConversationSystem.current_conversation) ~= nothing)
		TagConversationSystem.ask();
	else {
		PrintOrRun(TagConversationSystem, no_topics);
		TagConversationSystem.end();
	}
	return true;
];



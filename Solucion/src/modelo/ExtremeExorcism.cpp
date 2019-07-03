#include "ExtremeExorcism.h"

ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init, list<Accion> acciones_fantasma,
        Contexto *ctx) : juego(h), ctx(*ctx) {
    vector<Evento> f = eventosFanInicial(acciones_fantasma, f_init);
    nuevoFanEspecial(f);
    iniciarJugadores(jugadores);
}

ExtremeExorcism::Juego::Juego(Habitacion h) :
    paso(0),
    ronda(0),
    mapa(h),
    mapaDisparos(vector<vector<PasoDisparo>> (h.tam(),vector<PasoDisparo>(h.tam(), PasoDisparo(0,0)))),
    infoFantasmaEspecial(list<InfoActualFan>().begin()) {}
//                                              disparosFanUltimoPaso(list<Pos>()),
//                                              infoJugadores(string_map<InfoPJ>()),
//                                              infoActualJugadoresVivosq(list<InfoActualPJ>()),
//                                              infoJugadoresVivos(list<InfoPJ*>()),
//                                              infoFantasmas(list<InfoFan>()),
//                                              infoActualFantasmasVivos(list<InfoActualFan>()),
//                                              infoFantasmasVivos(list<list<InfoFan>::iterator>()),
// infoFantasmaEspecial(list<InfoActualFan>::iterator infoActualFantasmasVivos)

ExtremeExorcism::PasoDisparo::PasoDisparo(int pj, int fan) : pj(pj), fan(fan) {}

void ExtremeExorcism::pasar() {
    // Incremento el paso
    juego.paso++; // O(1)

    // Reinicio los disparos de los fantasmas
    reiniciarDisparosFan();         // O(1)

    // Actualizo las acciones de los fantasmas, actualizando el mapa de
    // disparos si es que disparan
    actualizarFantasmas();          // O(#fv * m)

    // Veo que jugadores mueren
    chequearMuerteJugadores();      // O(#jv)

    // Agrego los 'pasar' faltantes
    agregarPasarFaltantes();        // O(#jv)
}

void ExtremeExorcism::ejecutarAccion(const Jugador& j, Accion a) {
    // Incremento el paso
    juego.paso++; // O(1)

    // Actualizo la información del jugador con la nueva acción, y me guardo
    // una referencia a su info modificada
    InfoPJ infoPJ = actualizarPJ(j, a);                 // O(|j|)
    Evento eventoPJ = eventoActualPJ(infoPJ);           // O(1)

    // Reinicio los disparos de fantasmas
    reiniciarDisparosFan();                             // O(1)

    // Modifico el mapa de disparos (solo si el pj dispara)
    actualizarMapaDisparosConPJ(eventoPJ);            // O(m)

    // Veo que fantasmas mueren, guardaandome si murió el fantasma especial
    bool murioFanEspecial = chequearMuerteFantasmas();  // O(#fv)

    // Si murió, cambio de ronda
    if(murioFanEspecial) {
        nuevaRonda(infoPJ);   // O(m^2 + #f + locJugadores + #j * (|pjMasLargo| + long(maxEv)))
    } else {
        // Sigo en la misma ronda

        // Actualizo las acciones de los fantasmas, actualizando el mapa de
        // disparos si disparan
        actualizarFantasmas();      // O(#fv * m)

        // Veo que jugadores mueren
        chequearMuerteJugadores();  // O(#jv)

        // Agrego los 'pasar' faltantes
        agregarPasarFaltantes();    // O(#jv)
    }
}

void ExtremeExorcism::iniciarJugadores(const set<Jugador>& jugadores) {
    // Obtengo las posiciones y direcciones de los jugadores
    map<Jugador, PosYDir> localPJs = ctx.localizar_jugadores(jugadores, fantasmas(), juego.mapa); // O(locJugadores)

    // Lleno las estructuras de los jugadores
    for(auto& pair : localPJs) {                                                    // O(#pjs * |maxPJ|)
        Jugador pj = pair.first;
        PosYDir localizacion = pair.second;

        // Creo la infoActual y la agrego a su conjunto
        InfoActualPJ infoActual = InfoActualPJ{pj, localizacion};                   // O(1)
        auto itInfoActual = juego.infoActualJugadoresVivos.insert(
                juego.infoActualJugadoresVivos.begin(),
                infoActual
        ); // O(1)

        // Creo la infoPJ con la actual
        InfoPJ info = nuevaInfoPJ(localizacion, itInfoActual);                      // O(1)
        // La agrego al trie y me guardo un puntero a la info guardada
        InfoPJ* infoPtr = &juego.infoJugadores[pj];                                 // O(|pj|) // TODO: Cambiar por el nombre real de la función
        *infoPtr = info;

        // Agrego al conjunto de jugadores vivos el puntero a la info del PJ
        juego.infoJugadoresVivos.insert(
                juego.infoJugadoresVivos.begin(),
                infoPtr
        );                              // O(1)
    }
}

ExtremeExorcism::InfoPJ ExtremeExorcism::nuevaInfoPJ(PosYDir localizacion,
                                                     list<InfoActualPJ>::iterator itInfoActual) {
    // La armo
    return InfoPJ(
        crearEventosConLocalizacion(localizacion),
        true,
        itInfoActual
    );
}

void ExtremeExorcism::nuevoFanEspecial(const vector<Evento>& eventosFan) {
    // Creo la infoActual y la agrego a su conjunto
    InfoActualFan infoActual = PosYDir(eventosFan[0].pos, eventosFan[0].dir);   // O(1)
    auto itInfoActualFan = juego.infoActualFantasmasVivos.insert(
            juego.infoActualFantasmasVivos.begin(),
            infoActual
    );   // O(1)

    // Hago que este sea el fantasma especial
    juego.infoFantasmaEspecial = itInfoActualFan;   // O(1)

    // Le doy forma al vector de eventos
    vector<Evento> nuevosEventosFan = inversa(eventosFan);  // O(long(eventosFan)^2)

    // Creo la infoFan con la actual
    InfoFan infoFan = InfoFan(nuevosEventosFan, true, itInfoActualFan); // O(1)

    // La agrego al conjunto de info de fantasmas, y me guardo el iterador
    auto itInfoFan = juego.infoFantasmas.insert(
            juego.infoFantasmas.begin(),
            infoFan
    );  // O(1)

    // Agrego al conjunto de fantasmas vivos el iterador
    juego.infoFantasmasVivos.insert(
            juego.infoFantasmasVivos.begin(),
            itInfoFan
    );    // O(1)
}

vector<Evento> ExtremeExorcism::eventosFanInicial(const list<Accion>& l, PosYDir pd) {
    vector<Evento> f;
    Evento anterior = Evento(pd.pos, pd.dir, false); // OJO, ESTO PODRÍA NO IR Y QUE EL INICIAL SEA APLICAR DE LA PRIMER ACCIÓN
    f.push_back(anterior);
    for(Accion a : l){
        f.push_back(aplicar(a, anterior));
    }
    return f;
}

ExtremeExorcism::PasoDisparo& ExtremeExorcism::pasoDisparoEn(Pos pos) {
    return juego.mapaDisparos[pos.first][pos.second];
}

ExtremeExorcism::InfoPJ& ExtremeExorcism::actualizarPJ(const Jugador& j, Accion a) {
    // Busco la información del PJ
    InfoPJ& infoPJ = juego.infoJugadores.at(j);

    // Genero un evento con la acción y el evento anterior
    Evento eventoPJ = aplicar(a, eventoActualPJ(infoPJ));
    // Agrego el evento al jugador
    infoPJ.eventos.push_back(eventoPJ);

    // Actualizo su info actual

    /* infoPJ.infoActual->local = PosYDir(eventoPJ.pos, eventoPJ.dir);
     * Lo de arriba no vale porque -> devuelve un puntero const, es más bizarro. El * devuelve una referencia al valor apuntado.
     */
    InfoActualPJ iap = *infoPJ.infoActual;
    iap.local = PosYDir(eventoPJ.pos, eventoPJ.dir);


    // Devuelvo la info
    return infoPJ;
}

Evento ExtremeExorcism::eventoActualPJ(InfoPJ infoPJ) const {
    return infoPJ.eventos.back();   // O(1)
}

Evento ExtremeExorcism::eventoActualFan(ExtremeExorcism::InfoFan info, int paso) const {
    return info.eventos[paso % info.eventos.size()];
}

void ExtremeExorcism::reiniciarDisparosFan() {
    // Vacío la lista de disparos del ultimo paso
    juego.disparosFanUltimoPaso.clear(); // TODO: Como hacer clear de list?
}

void ExtremeExorcism::actualizarMapaDisparosConPJ(Evento eventoPJ) {
    // Si dispara, agrego las posiciones afectadas por su disparo
    // al mapa de disparos
    if (eventoPJ.dispara){
        agregarDisparo(eventoPJ.pos, eventoPJ.dir, false);  // O(m)
    }
}

void ExtremeExorcism::agregarDisparo(Pos pos, Dir dir, bool esFan) {
    // Me creo una copia de pos
    Pos posC = pos;

    // Parado desde posC en el mapa de disparos, recorro hacia dir
    // hasta que me choco con un obstaculo o con la pared.
    while(juego.mapa.valida(posC) && !juego.mapa.ocupado(pos)) {    // O(m)
        // Me guardo una referencia al paso del disparo correcto
        PasoDisparo disp = pasoDisparoEn(pos);
        int& pasoDisparo = esFan? disp.fan : disp.pj;

        // Si no pasé por esta posición con otro,
        // (i.e si en el mapa de disparos no está ya el paso actual)
        if (pasoDisparo != juego.paso) {
            // Le pongo el paso actual al paso en el que hubo un disparo
            pasoDisparo = juego.paso; // TODO: Se setea por ref?

            // Si es un fantasma, agrego la posición al conjunto de disparos de fantasmas
            if (esFan) {
                juego.disparosFanUltimoPaso.push_back(posC);
            }
        }

        // Avanzo la posición en la dirección
        posC = avanzar(pos, dir);
    }
}

bool ExtremeExorcism::chequearMuerteFantasmas() {
    // Me guardo si el fantasma especial muere
    bool muereFanEspecial = false;

    // Recorro los fantasmas vivos con un iterador
    for(auto itFanVivos = juego.infoFantasmasVivos.begin();
        itFanVivos != juego.infoFantasmasVivos.end();
        ++itFanVivos) {   // O()
        // Obtengo su info
        InfoFan infoFan = **itFanVivos;

        // Obtengo su evento actual
        Evento eventoActual = eventoActualFan(infoFan, juego.paso);

        if (fanAfectadoPorDisparo(eventoActual.pos)) {
            muereFanEspecial = muereFan(itFanVivos);
        }
    }

    return muereFanEspecial;
}

bool ExtremeExorcism::fanAfectadoPorDisparo(Pos pos) {
    // El fantasma estará afectado si en la posición en la que está hay un
    // disparo de un PJ en el paso actual.

    int pasoDispPJ = pasoDisparoEn(pos).pj;  // O(1)

    bool afectado = (pasoDispPJ == juego.paso);                     // O(1)

    return afectado;
}

bool ExtremeExorcism::muereFan(list<list<InfoFan>::iterator>::iterator itFanVivos) {
    // Obtengo la info
    InfoFan infoFan = **itFanVivos;     // O(1)

    // Lo seteo como muerto
    infoFan.vivo = false;               // O(1)

    // Obtengo su info actual
    list<InfoActualFan>::iterator itInfoActual = infoFan.infoActual;      // O(1)

    // Veo si era el especial
    InfoActualFan info = *(juego.infoFantasmaEspecial);
    bool eraEspecial = (*itInfoActual == info);    // O(1)

    // Lo borro de infoActualFantasmasVivos
    juego.infoActualFantasmasVivos.erase(itInfoActual);                 // O(1)

    // Lo borro de infoFantasmasVivos
    juego.infoFantasmasVivos.erase(itFanVivos); // TODO: No provee eliminar con iterador?

    // Retorno si era especial
    return eraEspecial;
}

void ExtremeExorcism::nuevaRonda(const ExtremeExorcism::InfoPJ& pjMatoFanEspecial) {
    // Incremento la ronda
    juego.ronda++;  // O(1)

    // Reinicio el paso
    juego.paso = 0; // O(1)

    // Reinicio el mapa de disparos y los disparos de los fantasmas
    reiniciarMapaDisparos();    // O(m^2)
    reiniciarDisparosFan();     // O(1)

    // Reinicio los fantasmas
    reiniciarFantasmas();       // O(#f)

    // Agrego el nuevo fantasma especial luego de convertir la lista en un vector
    vector<Evento> nuevoFan = vectorizar(pjMatoFanEspecial.eventos);
    nuevoFanEspecial(nuevoFan);

    // Reinicio los jugadores
    reiniciarJugadores();        // O(locJugadores + #j * (|maxPJ| + long(maxEvt))
}


void ExtremeExorcism::reiniciarMapaDisparos() {
    // Recorro todas las posiciones y las reinicio
    for (vector<PasoDisparo> row : juego.mapaDisparos) {
        for (PasoDisparo pos : row) {
            pos.pj = 0;
            pos.fan = 0;
        }
    }
}

void ExtremeExorcism::reiniciarFantasmas() {
    // Vacío la información de los fantasmas vivos
    juego.infoFantasmasVivos.clear(); // TODO: Como hacer clear de list?
    juego.infoActualFantasmasVivos.clear(); // TODO: Como hacer clear de list?

    // Recorro infoFantasmas con un iterador
    for (list<InfoFan>::iterator itInfoFan = juego.infoFantasmas.begin();
         itInfoFan != juego.infoFantasmas.end();
         ++itInfoFan){  // O(#f)
        // Obtengo su info
        InfoFan info = *itInfoFan;

        // Lo seteo como vivo;
        info.vivo = true;

        // Creo su infoActual y la agrego a infoActualFantasmasVivos,
        // guardandome el iterador
        PosYDir infoActualFan = PosYDir(info.eventos[0].pos, info.eventos[0].dir);
        auto itInfoActualFan = juego.infoActualFantasmasVivos.insert(  // O(1)
                juego.infoActualFantasmasVivos.begin(),
                infoActualFan
        );

        // Le seteo el iterador a la info actual
        info.infoActual = itInfoActualFan;

        // Agrego un iterador a su info a infoFantasmasVivos
        juego.infoFantasmasVivos.insert(
                juego.infoFantasmasVivos.begin(),
                itInfoFan
        );
    }
}

void ExtremeExorcism::reiniciarJugadores() {
    // Vacío las estructuras de vivos
    juego.infoActualJugadoresVivos.clear(); // TODO: Como hacer clear de list?
    juego.infoJugadoresVivos.clear(); // TODO: Como hacer clear de list?

    // Obtengo sus localizaciones
    map<Jugador, PosYDir> localPJs = ctx.localizar_jugadores(jugadores(), fantasmas(), juego.mapa);

    for(auto& pair : localPJs) {
        Jugador pj = pair.first;
        PosYDir localizacion = pair.second;

        // Obtengo infoPJ del trie
        InfoPJ& infoPJ = juego.infoJugadores.at(pj);

        // Vacío los eventos
        infoPJ.eventos.clear();

        // Creo una lista de eventos con la localización
        infoPJ.eventos = crearEventosConLocalizacion(localizacion);

        // Lo seteo como vivo
        infoPJ.vivo = true;

        // Creo una info actual
        InfoActualPJ infoActualPJ = InfoActualPJ{pj, localizacion};

        // La agrego a infoActualJugadoresVivos y me guardo el iterador
        auto itInfoActual = juego.infoActualJugadoresVivos.insert(
                juego.infoActualJugadoresVivos.begin(),
                infoActualPJ
        );

        // Le seteo la info actual
        infoPJ.infoActual = itInfoActual;

        // Agrego un puntero a la infoPJ a infoJugadoresvivos
        juego.infoJugadoresVivos.insert(
                juego.infoJugadoresVivos.begin(),
                &infoPJ
        );
    }
}

list<Evento> ExtremeExorcism::crearEventosConLocalizacion(PosYDir localizacion) {
    // Creo el evento
    Evento evt = Evento(localizacion.pos, localizacion.dir, false);
    // Creo una lista con el
    list<Evento> eventos;
    eventos.push_back(evt);

    return eventos;
}

void ExtremeExorcism::actualizarFantasmas() {
    // Recorro los fantasmas vivos
    for (auto& itInfoFan : juego.infoFantasmasVivos) {   // O(#f * m)
        // Obtengo la info del fantasma
        InfoFan info = *itInfoFan;

        // Actualizo su info actual, obteniendola del evento actual
        Evento eventoActual = actualizarFan(info);

        // Si dispara, agrego su disparo a los del paso
        if (eventoActual.dispara) {
            agregarDisparo(eventoActual.pos, eventoActual.dir, true);   // O(m)
        }
    }
}

Evento ExtremeExorcism::actualizarFan(InfoFan& info) {
    // Obtengo el evento actual
    Evento evtActual = eventoActualFan(info, juego.paso);

    // Actualizo la info actual con el evento actual
    *(info.infoActual) = PosYDir(evtActual.pos, evtActual.dir);

    // Devuelvo el evento actualposicionJugador
    return evtActual;
}

void ExtremeExorcism::chequearMuerteJugadores() {
    // Recorro los jugadores vivos
    for (auto itPJVivos = juego.infoJugadoresVivos.begin();
         itPJVivos != juego.infoJugadoresVivos.end();
         ++itPJVivos) {
        // Obtengo su evento actual
        InfoPJ* ptrInfoPJ = *itPJVivos;
        Evento eventoActual = eventoActualPJ(*ptrInfoPJ);

        if (pjAfectadoPorDisparo(eventoActual.pos)) {
            muerePJ(itPJVivos);
        }
    }
}

bool ExtremeExorcism::pjAfectadoPorDisparo(Pos pos) {
    // El PJ estará afectado si en la posición en la que está hay un disparo de un fantasma
    // Indexo por su posición en el mapa de disparos para obtener el paso en
    // el que hubo un disparo del fantasma.

    int pasoDispFan = pasoDisparoEn(pos).fan;
    // Estará afectado si el paso del disparo es igual al actual
    bool afectado = (pasoDispFan == juego.paso);

    return afectado;

}

void ExtremeExorcism::muerePJ(list<InfoPJ *>::iterator itPJVivos) {
    // Obtengo una ref a su info
    InfoPJ& info = **itPJVivos;

    // Lo seteo como muerto
    info.vivo = false;

    // Lo borro del conjunto infoActualJugadoresVivos
    juego.infoActualJugadoresVivos.erase(info.infoActual); // TODO: No se puede borrar con iterador?

    // Lo borro del conjunto infoJugadoresVivos
    juego.infoJugadoresVivos.erase(itPJVivos); // TODO: No se puede borrar con iterador?
}

void ExtremeExorcism::agregarPasarFaltantes() {
    // Recorro los jugadores vivos, y le agrego un pasar a los jugadores que
    // no se movieron en este paso

    for (InfoPJ* ptrInfoPJ: juego.infoJugadoresVivos) {
        // Si su lista de acciones no es de la misma longitud que el paso actual + 1,
        // entonces es necesario agregarle una accion pasar.

        list<Evento>& eventos = ptrInfoPJ->eventos;
        if (eventos.size() != juego.paso + 1){
            // Genero el evento pasar
            Evento pasar = Evento(eventos.back().pos, eventos.back().dir, false);

            // Lo agrego al final
            eventos.push_back(pasar);
        }
    }
}






list<pair<Jugador, PosYDir>> ExtremeExorcism::posicionJugadores() const {
    /*Observación:
     * En el TP2 devolvemos el conjLineal por referencia pero como aquí piden devolver una lista, hacemos la conversión en O(n).
     * (Esto fue consultado con Lean y dijo que estaba bien)
     */
    list<pair<Jugador, PosYDir>> res;
    for(const InfoActualPJ& infoPJ : juego.infoActualJugadoresVivos){
        res.emplace_back(make_pair(infoPJ.id, infoPJ.local));
    }
    return res;
}

list<PosYDir> ExtremeExorcism::posicionFantasmas() const {
    // Observación: Es necesario realizar la conversión a list, pero se podría
    //              devolver en O(1).

    list<PosYDir> ps;

    for(PosYDir p : juego.infoActualFantasmasVivos) { // O(n)
        ps.push_back(p);
    }

    return ps;
}

PosYDir ExtremeExorcism::posicionEspecial() const {
    return *juego.infoFantasmaEspecial;
}

list<PosYDir> ExtremeExorcism::disparosFantasmas() const {
    list<PosYDir> ps;
    for (auto itInfoFan : juego.infoFantasmasVivos) { // O(#fv)
        InfoFan infoFan = *itInfoFan;
        Evento e = eventoActualFan(infoFan, juego.paso);
        if (e.dispara) {
            ps.push_back(PosYDir(e.pos, e.dir));
        }
    }

    return ps;
}

set<Pos> ExtremeExorcism::posicionesDisparadas() const {
    /*Observación:
 *  En el TP2 devolvemos el conjLineal por referencia pero como aquí piden devolver un set, hacemos la conversión en O(n).
 *  (Esto fue consultado con Lean y dijo que estaba bien)
 */
    set<Pos> res;
    for(Pos p : juego.disparosFanUltimoPaso){
        res.insert(p);
    }
    return res;
}

bool ExtremeExorcism::jugadorVivo(Jugador j) const {
    InfoPJ iPj = juego.infoJugadores.at(j); // O(|j|)
    return iPj.vivo;
}

const Habitacion& ExtremeExorcism::habitacion() const {
    return juego.mapa; // O(1)
}

PosYDir ExtremeExorcism::posicionJugador(const Jugador& j) const {
    InfoPJ iPj = juego.infoJugadores.at(j); // O(|j|)
    return iPj.infoActual->local; // O(1)
}

const set<Jugador>& ExtremeExorcism::jugadores() const {
    return juego.infoJugadores.claves();
}

const list<Fantasma> ExtremeExorcism::fantasmas() const {
    list<Fantasma> l; // O(1)
    auto it = l.begin(); // O(1)
    for (InfoFan f : juego.infoFantasmas){ // O(fs*|maxEv|)
        l.insert(it, vecToList(f.eventos)); // O(|Ev|)
        it++; // O(1)
    }
    return l;
}

list<Evento> ExtremeExorcism::vecToList(vector<Evento> vector) const {
    list<Evento> l;
    auto it = l.begin();
    for (int i = 0; i < vector.size(); ++i) {
        l.insert(it, vector[i]);
        it++;
    }
    return l;
}

vector<Evento> ExtremeExorcism::vectorizar(list<Evento> l) {
    vector<Evento> v;
    for (auto it = l.begin(); it != l.end(); it++) {
        v.push_back(*it);
    }
    return v;
}

Pos ExtremeExorcism::avanzar(Pos p, Dir d) {
    Pos pos = Pos();
    switch (d)
    {
        case ARRIBA:
            pos = Pos(p.first + 1, p.second);
            break;
        case ABAJO:
            pos = Pos(p.first - 1, p.second);
            break;
        case IZQUIERDA:
            pos = Pos(p.first, p.second - 1);
            break;
        case DERECHA:
            pos = Pos(p.first, p.second + 1);
            break;
    }
    return pos;
}

Evento ExtremeExorcism::aplicar(Accion a, Evento eventoActual) {
    if (a == DISPARAR) {
        return Evento(eventoActual.pos, eventoActual.dir, true);
    }

    if (a == ESPERAR) {
        return Evento(eventoActual.pos, eventoActual.dir, false);
    }

    // Se que es una acción de mover
    Dir dirDeA = dirDe(a);

    Pos prox = avanzar(eventoActual.pos, dirDeA);
    if (juego.mapa.valida(prox) && !juego.mapa.ocupado(prox)) {
        // Si es una posición valida que no está ocupada, entonces me puedo
        // mover en esa dirección.
        return Evento(prox, dirDeA, false);
    }
    // Sino, solamente cambia la dirección en la que miro, pero mi posición
    // se mantiene.
    return Evento(eventoActual.pos, dirDeA, false);
}

Dir ExtremeExorcism::dirDe(Accion a) {
    switch(a){
        case MARRIBA:
            return ARRIBA;
        case MABAJO:
            return ABAJO;
        case MIZQUIERDA:
            return IZQUIERDA;
        case MDERECHA:
            return DERECHA;
        default:
            // No debería pasar
            return ARRIBA;
    }
}

vector<Evento> ExtremeExorcism::inversa(vector<Evento> eventos) {
    // Copio el vector de entrada
    vector<Evento> es = eventos;

    // Me guardo la longitud original
    int longOriginal = es.size();

    // Creo el evento pasar y lo agrego 5 veces
    Evento pasar = Evento(eventos.back().pos, eventos.back().dir, false);
    for(int i = 0; i < 5; i++) {
        es.push_back(pasar);
    }

    // Recorro los eventos de la secuencia original de atrás para adelante,
    // inviertiéndolos y agregándolos al final.
    for (int i = longOriginal - 1; i >= 0; i--) {
        es.push_back(invertir(es[i]));
    }

    pasar = Evento(es.back().pos, es.back().dir, false);
    for(int i = 0; i < 5; i++) {
        es.push_back(pasar);
    }

    return es;
}

Evento ExtremeExorcism::invertir(Evento e) {
    return Evento(e.pos, invertir(e.dir), e.dispara);
}

Dir ExtremeExorcism::invertir(Dir d) {
    switch (d) {
    case ARRIBA:
        return ABAJO;
    case ABAJO:
        return ARRIBA;
    case DERECHA:
        return IZQUIERDA;
    case IZQUIERDA:
        return DERECHA;
    default:
        return ARRIBA;
    }
}
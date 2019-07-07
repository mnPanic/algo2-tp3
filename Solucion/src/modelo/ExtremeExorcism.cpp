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

ExtremeExorcism::PasoDisparo::PasoDisparo(int pj, int fan) : pj(pj), fan(fan) {}

void ExtremeExorcism::pasar() {
    juego.paso++; // O(1)

    reiniciarDisparosFan();         // O(1)

    // Actualizo las acciones de los fantasmas, actualizando el mapa de
    // disparos si es que disparan
    actualizarFantasmas();          // O(#fv * m)

    chequearMuerteJugadores();      // O(#jv)

    agregarPasarFaltantes();        // O(#jv)
}

void ExtremeExorcism::ejecutarAccion(const Jugador& j, Accion a) {
    juego.paso++; // O(1)

    InfoPJ infoPJ = actualizarPJ(j, a);                 // O(|j|)
    Evento eventoPJ = eventoActualPJ(infoPJ);           // O(1)

    reiniciarDisparosFan();                             // O(1)

    // Modifico el mapa de disparos (solo si el pj dispara)
    actualizarMapaDisparosConPJ(eventoPJ);            // O(m)

    // Veo que fantasmas mueren, guardaandome si murió el fantasma especial
    bool murioFanEspecial = chequearMuerteFantasmas();  // O(#fv)

    if(murioFanEspecial) {
        nuevaRonda(infoPJ);   // O(m^2 + #f + locJugadores + #j * (|pjMasLargo| + long(maxEv)))
    } else {
        // Actualizo las acciones de los fantasmas, actualizando el mapa de
        // disparos si disparan
        actualizarFantasmas();      // O(#fv * m)

        chequearMuerteJugadores();  // O(#jv)
        agregarPasarFaltantes();    // O(#jv)
    }
}

void ExtremeExorcism::iniciarJugadores(const set<Jugador>& jugadores) {
    map<Jugador, PosYDir> localPJs = ctx.localizar_jugadores(jugadores, fantasmas(), juego.mapa); // O(locJugadores)

    for(auto& pair : localPJs) {                                                    // O(#pjs * |maxPJ|)
        Jugador pj = pair.first;
        PosYDir localizacion = pair.second;

        InfoActualPJ infoActual = InfoActualPJ{pj, localizacion};                   // O(1)
        auto itInfoActual = juego.infoActualJugadoresVivos.insert(
                juego.infoActualJugadoresVivos.begin(),
                infoActual
        ); // O(1)

        InfoPJ info = nuevaInfoPJ(localizacion, itInfoActual);                      // O(1)
        // La agrego al trie y me guardo un puntero a la info guardada
        InfoPJ* infoPtr = &juego.infoJugadores[pj];                                 // O(|pj|) // TODO: Cambiar por el nombre real de la función
        *infoPtr = info;

        juego.infoJugadoresVivos.insert(
                juego.infoJugadoresVivos.begin(),
                infoPtr
        );                              // O(1)
    }
}

ExtremeExorcism::InfoPJ ExtremeExorcism::nuevaInfoPJ(PosYDir localizacion,
                                                     list<InfoActualPJ>::iterator itInfoActual) {
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

    juego.infoFantasmaEspecial = itInfoActualFan;   // O(1)

    // Le doy forma al vector de eventos
    vector<Evento> nuevosEventosFan = inversa(eventosFan);  // O(long(eventosFan)^2)

    InfoFan infoFan = InfoFan(nuevosEventosFan, true, itInfoActualFan); // O(1)

    auto itInfoFan = juego.infoFantasmas.insert(
            juego.infoFantasmas.begin(),
            infoFan
    );  // O(1)

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
        anterior = f[f.size() - 1];
    }
    return f;
}

ExtremeExorcism::PasoDisparo& ExtremeExorcism::pasoDisparoEn(Pos pos) {
    return juego.mapaDisparos[pos.first][pos.second];
}

ExtremeExorcism::InfoPJ& ExtremeExorcism::actualizarPJ(const Jugador& j, Accion a) {
    InfoPJ& infoPJ = juego.infoJugadores.at(j);

    // Genero un evento con la acción y el evento anterior
    Evento eventoPJ = aplicar(a, eventoActualPJ(infoPJ));
    infoPJ.eventos.push_back(eventoPJ);

    (infoPJ.infoActual)->local = PosYDir(eventoPJ.pos, eventoPJ.dir);

    return infoPJ;
}

Evento ExtremeExorcism::eventoActualPJ(InfoPJ infoPJ) const {
    return infoPJ.eventos.back();   // O(1)
}

Evento ExtremeExorcism::eventoActualFan(ExtremeExorcism::InfoFan info, int paso) const {
    return info.eventos[paso % info.eventos.size()];
}

void ExtremeExorcism::reiniciarDisparosFan() {
    juego.disparosFanUltimoPaso.clear(); // TODO: Como hacer clear de list?
}

void ExtremeExorcism::actualizarMapaDisparosConPJ(Evento eventoPJ) {
    if (eventoPJ.dispara){
        agregarDisparo(eventoPJ.pos, eventoPJ.dir, false);  // O(m)
    }
}

void ExtremeExorcism::agregarDisparo(Pos pos, Dir dir, bool esFan) {
    // Parado desde la siguiente pos en el mapa de disparos, recorro hacia dir
    // hasta que me choco con un obstaculo o con la pared.
    pos = avanzar(pos, dir);
    while(juego.mapa.valida(pos) && !juego.mapa.ocupado(pos)) {    // O(m)
        PasoDisparo& disp = pasoDisparoEn(pos);
        int& pasoDisparo = esFan? disp.fan : disp.pj;

        // Si no pasé por esta posición con otro,
        // (i.e si en el mapa de disparos no está ya el paso actual)
        if (pasoDisparo != juego.paso) {
            pasoDisparo = juego.paso;

            if (esFan) {
                juego.disparosFanUltimoPaso.push_back(pos);
            }
        }

        pos = avanzar(pos, dir);
    }
}

bool ExtremeExorcism::chequearMuerteFantasmas() {
    bool muereFanEspecial = false;

    auto itFanVivos = juego.infoFantasmasVivos.begin();
    while(itFanVivos != juego.infoFantasmasVivos.end() && !muereFanEspecial) {   // O()

        InfoFan& infoFan = **itFanVivos;
        Evento eventoActual = eventoActualFan(infoFan, juego.paso);

        if (fanAfectadoPorDisparo(eventoActual.pos)) {
            muereFanEspecial = muereFan(itFanVivos++);
        } else {
            ++itFanVivos;
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
    InfoFan& infoFan = **itFanVivos;     // O(1)

    infoFan.vivo = false;               // O(1)
    auto itInfoActual = infoFan.infoActual;      // O(1)

    InfoActualFan& info = *(juego.infoFantasmaEspecial); // O(1)
    bool eraEspecial = (*itInfoActual == info);    // O(1)

    juego.infoActualFantasmasVivos.erase(itInfoActual); // O(1)
    juego.infoFantasmasVivos.erase(itFanVivos); // O(1)

    return eraEspecial;
}

void ExtremeExorcism::nuevaRonda(const ExtremeExorcism::InfoPJ& pjMatoFanEspecial) {
    juego.ronda++;  // O(1)
    juego.paso = 0; // O(1)

    // Reinicio las estructuras
    reiniciarMapaDisparos();    // O(m^2)
    reiniciarDisparosFan();     // O(1)
    reiniciarFantasmas();       // O(#f)

    vector<Evento> nuevoFan = vectorizar(pjMatoFanEspecial.eventos);
    nuevoFanEspecial(nuevoFan);

    reiniciarJugadores();        // O(locJugadores + #j * (|maxPJ| + long(maxEvt))
}


void ExtremeExorcism::reiniciarMapaDisparos() {
    // Recorro todas las posiciones y las reinicio
    for (vector<PasoDisparo>& row : juego.mapaDisparos) {
        for (PasoDisparo& pos : row) {
            pos.pj = 0;
            pos.fan = 0;
        }
    }
}

void ExtremeExorcism::reiniciarFantasmas() {
    // Vacío la información de los fantasmas vivos
    juego.infoFantasmasVivos.clear(); // TODO: Como hacer clear de list?
    juego.infoActualFantasmasVivos.clear(); // TODO: Como hacer clear de list?

    for (auto itInfoFan = juego.infoFantasmas.begin();
         itInfoFan != juego.infoFantasmas.end();
         ++itInfoFan){  // O(#f)
        itInfoFan->vivo = true;

        PosYDir infoActualFan = PosYDir(itInfoFan->eventos[0].pos, itInfoFan->eventos[0].dir);
        auto itInfoActualFan = juego.infoActualFantasmasVivos.insert(  // O(1)
                juego.infoActualFantasmasVivos.begin(),
                infoActualFan
        );

        itInfoFan->infoActual = itInfoActualFan;

        // Agrego un iterador a su info a infoFantasmasVivos
        juego.infoFantasmasVivos.insert(
                juego.infoFantasmasVivos.begin(),
                itInfoFan
        );
    }
}

void ExtremeExorcism::reiniciarJugadores() {
    juego.infoActualJugadoresVivos.clear(); // TODO: Como hacer clear de list?
    juego.infoJugadoresVivos.clear(); // TODO: Como hacer clear de list?

    map<Jugador, PosYDir> localPJs = ctx.localizar_jugadores(jugadores(), fantasmas(), juego.mapa);

    for(auto& pair : localPJs) {
        Jugador pj = pair.first;
        PosYDir localizacion = pair.second;

        // Obtengo infoPJ del trie y actualizo toda su información.
        InfoPJ& infoPJ = juego.infoJugadores.at(pj);

        infoPJ.eventos.clear();
        infoPJ.eventos = crearEventosConLocalizacion(localizacion);
        infoPJ.vivo = true;
        InfoActualPJ infoActualPJ = InfoActualPJ{pj, localizacion};

        auto itInfoActual = juego.infoActualJugadoresVivos.insert(
                juego.infoActualJugadoresVivos.begin(),
                infoActualPJ
        );

        infoPJ.infoActual = itInfoActual;

        // Agrego un puntero a la infoPJ a infoJugadoresvivos
        juego.infoJugadoresVivos.insert(
                juego.infoJugadoresVivos.begin(),
                &infoPJ
        );
    }
}

list<Evento> ExtremeExorcism::crearEventosConLocalizacion(PosYDir localizacion) {
    Evento evt = Evento(localizacion.pos, localizacion.dir, false);
    list<Evento> eventos;
    eventos.push_back(evt);
    return eventos;
}

void ExtremeExorcism::actualizarFantasmas() {
    for (auto& itInfoFan : juego.infoFantasmasVivos) {   // O(#f * m)
        InfoFan info = *itInfoFan;

        Evento eventoActual = actualizarFan(info);

        if (eventoActual.dispara) {
            agregarDisparo(eventoActual.pos, eventoActual.dir, true);   // O(m)
        }
    }
}

Evento ExtremeExorcism::actualizarFan(InfoFan& info) {
    Evento evtActual = eventoActualFan(info, juego.paso);

    *(info.infoActual) = PosYDir(evtActual.pos, evtActual.dir);

    return evtActual;
}

void ExtremeExorcism::chequearMuerteJugadores() {
    auto itPJVivos = juego.infoJugadoresVivos.begin();
    while(itPJVivos != juego.infoJugadoresVivos.end() && !juego.infoJugadoresVivos.empty()) {
        InfoPJ* ptrInfoPJ = *itPJVivos;
        Evento eventoActual = eventoActualPJ(*ptrInfoPJ);

        if (pjAfectadoPorDisparo(eventoActual.pos)) {
            muerePJ(itPJVivos++);
        } else {
            itPJVivos++;
        }
    }
}

bool ExtremeExorcism::pjAfectadoPorDisparo(Pos pos) {
    // El PJ estará afectado si en la posición en la que está hay un disparo de un fantasma
    // Indexo por su posición en el mapa de disparos para obtener el paso en
    // el que hubo un disparo del fantasma.

    int pasoDispFan = pasoDisparoEn(pos).fan;
    bool afectado = (pasoDispFan == juego.paso);

    return afectado;

}

void ExtremeExorcism::muerePJ(list<InfoPJ *>::iterator itPJVivos) {
    InfoPJ& info = **itPJVivos;
    info.vivo = false;
    juego.infoActualJugadoresVivos.erase(info.infoActual); // TODO: No se puede borrar con iterador?
    juego.infoJugadoresVivos.erase(itPJVivos); // TODO: No se puede borrar con iterador?
}

void ExtremeExorcism::agregarPasarFaltantes() {
    // Recorro los jugadores vivos, y le agrego un pasar a los jugadores que
    // no se movieron en este paso

    for (InfoPJ* ptrInfoPJ: juego.infoJugadoresVivos) {

        list<Evento>& eventos = ptrInfoPJ->eventos;
        if (eventos.size() != juego.paso + 1){
            Evento pasar = Evento(eventos.back().pos, eventos.back().dir, false);
            eventos.push_back(pasar);
        }
    }
}






list<pair<Jugador, PosYDir>> ExtremeExorcism::posicionJugadores() const {
    list<pair<Jugador, PosYDir>> res;
    for(const InfoActualPJ& infoPJ : juego.infoActualJugadoresVivos){
        res.emplace_back(make_pair(infoPJ.id, infoPJ.local));
    }
    return res;
}

list<PosYDir> ExtremeExorcism::posicionFantasmas() const {
    return juego.infoActualFantasmasVivos;
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
            ps.emplace_back(e.pos, e.dir);
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

vector<Evento> ExtremeExorcism::vectorizar(const list<Evento>& l) {
    vector<Evento> v;
    for (auto it : l) {
        v.push_back(it);
    }
    return v;
}

Pos ExtremeExorcism::avanzar(Pos p, Dir d) {
    Pos pos = Pos();
    switch (d)
    {
        case ARRIBA:
            pos = Pos(p.first, p.second + 1);
            break;
        case ABAJO:
            pos = Pos(p.first, p.second - 1);
            break;
        case IZQUIERDA:
            pos = Pos(p.first - 1, p.second);
            break;
        case DERECHA:
            pos = Pos(p.first + 1, p.second);
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
    vector<Evento> es = eventos;
    int longOriginal = es.size();

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
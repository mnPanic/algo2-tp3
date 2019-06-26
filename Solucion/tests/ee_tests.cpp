#include "estado/EstadoJuego.h"
#include "gtest-1.8.1/gtest.h"
#include "modelo/Contexto.h"
#include "modelo/ExtremeExorcism.h"
#include "modelo/localizadores/localizador_aleatorio.h"
#include "modelo/localizadores/localizador_estatico.h"

#define VALIDAR_ESTADO(cant_jugs, estado_str, ee) {\
  SCOPED_TRACE("Validando estado juego");\
    EstadoJuego(cant_jugs, estado_str).verificar(ee);\
};

bool en_rango(Pos p, unsigned int tam) {
  return p.first >= 0 and p.second >= 0 and p.first < tam and p.second < tam;
}

TEST(EE, ContextoAleatorio) {
  Localizador *l = new LocalizadorAleatorio();
  Contexto ctx = Contexto(l);
  set<Jugador> jugadores = set<Jugador>{"P1", "P2", "P3"};
  Habitacion h(3, {Pos(0, 0), Pos(0, 1), Pos(0, 2), Pos(1, 1)});

  for (int i = 0; i < 100; i++) {
    map<Jugador, PosYDir> res =
        ctx.localizar_jugadores(jugadores, list<Fantasma>(), h);

    for (const Jugador &j : jugadores) { // Están todos los jugadores
      EXPECT_TRUE(res.count(j));
    }
    set<Pos> pos;
    for (const pair<Jugador, PosYDir> &j_pd : res) { // Son posiciones válidas
      EXPECT_TRUE(en_rango(j_pd.second.pos, 3));
      EXPECT_TRUE(not h.ocupado(j_pd.second.pos));
      pos.insert(j_pd.second.pos);
    }
    EXPECT_EQ(pos.size(), jugadores.size()); // Las posiciones no se repiten
  }
  delete l;
}

TEST(EE, ContextoEstatico_1J_1R) {
  Localizador *l = new LocalizadorEstatico({{"P1", PosYDir({0, 0}, ARRIBA)}});
  Contexto ctx = Contexto(l);
  set<Jugador> j1 = set<Jugador>{"P1"};
  Habitacion h(3, {});

  map<Jugador, PosYDir> res = ctx.localizar_jugadores(j1, list<Fantasma>(), h);

  EXPECT_TRUE(res.count("P1"));
  EXPECT_FALSE(res.count("P2"));
  EXPECT_EQ(res.at("P1"), PosYDir({0, 0}, ARRIBA));

  delete l;
}

TEST(EE, ContextoEstatico_2J_2R) {
  Localizador *l = new LocalizadorEstatico(
      {{0, {{"P1", PosYDir({0, 0}, ARRIBA)}, {"P2", PosYDir({1, 1}, ABAJO)}}},
       {1,
        {{"P2", PosYDir({0, 0}, ARRIBA)}, {"P1", PosYDir({1, 1}, DERECHA)}}}});
  Contexto ctx = Contexto(l);
  set<Jugador> j2 = set<Jugador>{"P1", "P2"};
  Habitacion h(3, {});

  map<Jugador, PosYDir> res = ctx.localizar_jugadores(j2, list<Fantasma>(), h);

  EXPECT_TRUE(res.count("P1"));
  EXPECT_TRUE(res.count("P2"));
  EXPECT_EQ(res.at("P1"), PosYDir({0, 0}, ARRIBA));
  EXPECT_EQ(res.at("P2"), PosYDir({1, 1}, ABAJO));

  list<Fantasma> r2;
  r2.push_back({});
  res = ctx.localizar_jugadores(j2, r2, h);

  EXPECT_TRUE(res.count("P1"));
  EXPECT_TRUE(res.count("P2"));
  EXPECT_EQ(res.at("P1"), PosYDir({1, 1}, DERECHA));
  EXPECT_EQ(res.at("P2"), PosYDir({0, 0}, ARRIBA));

  list<Fantasma> r3;
  r3.push_back({});
  r3.push_back({});

  EXPECT_TRUE(res.count("P1"));
  EXPECT_TRUE(res.count("P2"));
  EXPECT_EQ(res.at("P1"), PosYDir({1, 1}, DERECHA));
  EXPECT_EQ(res.at("P2"), PosYDir({0, 0}, ARRIBA));

  delete l;
}

TEST(EE, InitRonda_J1) {
  Localizador *l = new LocalizadorEstatico({{"P1", {PosYDir({0, 0}, ARRIBA)}}});
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"P1"};

  ExtremeExorcism ee(h, js, PosYDir({2, 2}, ARRIBA), {}, &ctx);

  EXPECT_EQ(ee.jugadores(), js);

  // Jugador
  EXPECT_TRUE(ee.jugadorVivo("P1"));
  EXPECT_EQ(ee.habitacion(), h);
  EXPECT_EQ(ee.posicionJugador("P1"), PosYDir({0, 0}, ARRIBA));
  auto pos_j = ee.posicionJugadores();
  EXPECT_EQ(pos_j.size(), 1);
  EXPECT_EQ(pos_j.back().first, "P1");
  EXPECT_EQ(pos_j.back().second, PosYDir({0, 0}, ARRIBA));

  // Fantasma
  auto pos_f = ee.posicionFantasmas();
  EXPECT_EQ(pos_f.size(), 1);
  EXPECT_EQ(pos_f.back(), PosYDir({2, 2}, ARRIBA));
  EXPECT_EQ(ee.posicionEspecial(), PosYDir({2, 2}, ARRIBA));
  EXPECT_TRUE(ee.jugadorVivo("P1"));
  EXPECT_EQ(ee.posicionJugador("P1"), PosYDir({0, 0}, ARRIBA));
  EXPECT_EQ(ee.posicionFantasmas(), list<PosYDir>{PosYDir({2, 2}, ARRIBA)});

  EXPECT_EQ(ee.posicionEspecial(), PosYDir({2, 2}, ARRIBA));
  for (int i = 0; i < 5; i++) {
    ee.pasar();
    EXPECT_EQ(ee.posicionEspecial(), PosYDir({2, 2}, ARRIBA));
  }

  ee.pasar();
  EXPECT_EQ(ee.posicionEspecial(), PosYDir({2, 2}, ABAJO));
  for (int i = 0; i < 5; i++) {
    ee.pasar();
    EXPECT_EQ(ee.posicionEspecial(), PosYDir({2, 2}, ABAJO));
  }

  delete l;
}

TEST(EE, Estado_InitRonda_J1) {
  Localizador *l = new LocalizadorEstatico({{"0", {PosYDir({0, 2}, ARRIBA)}}});
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};
  ExtremeExorcism ee(h, js, PosYDir({2, 0}, ARRIBA), {}, &ctx);

  VALIDAR_ESTADO(1, "W..\n"
                    ".#.\n"
                    "..w\n", ee);

  delete l;
}

TEST(EE, InitRonda_J2) {
  Localizador *l = new LocalizadorEstatico(
      {{"0", {PosYDir({0, 2}, ARRIBA)}}, {"1", {PosYDir({2, 2}, ABAJO)}}});
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0", "1"};
  ExtremeExorcism ee(h, js, PosYDir({2, 0}, ARRIBA), {}, &ctx);

  VALIDAR_ESTADO(2, "W.D\n"
                    ".#.\n"
                    "..w\n", ee);

  delete l;
}

TEST(EE, MoverJugador) {
  Localizador *l = new LocalizadorEstatico(
      {{"0", {PosYDir({0, 2}, ARRIBA)}}, {"1", {PosYDir({2, 2}, ABAJO)}}});
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0", "1"};

  ExtremeExorcism ee(h, js, PosYDir({2, 0}, ARRIBA), {}, &ctx);
  VALIDAR_ESTADO(2, "W.D\n"
                 ".#.\n"
                 "..w\n", ee);

  ee.ejecutarAccion("0", MDERECHA);
  VALIDAR_ESTADO(2, ".RD\n"
                 ".#.\n"
                 "..w\n", ee);

  ee.ejecutarAccion("1", MABAJO);
  VALIDAR_ESTADO(2, ".R.\n"
                 ".#D\n"
                 "..w\n", ee);

  ee.ejecutarAccion("0", DISPARAR);
  VALIDAR_ESTADO(2, ".R.\n"
                    ".#D\n"
                    "..w\n", ee);

  delete l;
}

TEST(EE, MoverJugadorObstaculo) {
  Localizador *l = new LocalizadorEstatico(
      {{"0", {PosYDir({0, 2}, ARRIBA)}}, {"1", {PosYDir({2, 2}, ABAJO)}}});
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0", "1"};

  ExtremeExorcism ee(h, js, PosYDir({2, 0}, ARRIBA), {}, &ctx);
  VALIDAR_ESTADO(2, "W.D\n"
                    ".#.\n"
                    "..w\n", ee);

  ee.ejecutarAccion("0", MDERECHA);
  VALIDAR_ESTADO(2, ".RD\n"
                    ".#.\n"
                    "..w\n", ee);

  ee.ejecutarAccion("0", MABAJO);
  VALIDAR_ESTADO(2, ".ED\n"
                    ".#.\n"
                    "..w\n", ee);

  ee.ejecutarAccion("0", MIZQUIERDA);
  VALIDAR_ESTADO(2, "Q.D\n"
                    ".#.\n"
                    "..w\n", ee);

  ee.ejecutarAccion("0", MABAJO);
  VALIDAR_ESTADO(2, "..D\n"
                    "E#.\n"
                    "..w\n", ee);

  ee.ejecutarAccion("0", MDERECHA);
  VALIDAR_ESTADO(2, "..D\n"
                    "R#.\n"
                    "..w\n", ee);

  delete l;
}

TEST(EE, MoverFuera) {
  Localizador *l = new LocalizadorEstatico({
      {"0", {PosYDir({0, 2}, ARRIBA)}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({2, 0}, ARRIBA), {}, &ctx);
  VALIDAR_ESTADO(1, "W..\n"
                    ".#.\n"
                    "..w\n", ee);

  ee.ejecutarAccion("0", MIZQUIERDA);
  VALIDAR_ESTADO(1, "Q..\n"
                    ".#.\n"
                    "..w\n", ee);

  ee.ejecutarAccion("0", MARRIBA);
  VALIDAR_ESTADO(1, "W..\n"
                    ".#.\n"
                    "..w\n", ee);

  delete l;
}

// Movimientos fantasma sin repetición
TEST(EE, Pasar) {
  Localizador *l = new LocalizadorEstatico({
      {"0", {PosYDir({0, 0}, ARRIBA)}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};
  Fantasma f{
      Evento({2, 0}, ARRIBA, false),   Evento({2, 1}, ARRIBA, false),
      Evento({2, 2}, ARRIBA, false),   Evento({1, 2}, IZQUIERDA, false),
      Evento({1, 2}, IZQUIERDA, true),
  };

  ExtremeExorcism ee(h, js, PosYDir({2, 0}, ARRIBA),
                     {MARRIBA, MARRIBA, MIZQUIERDA, DISPARAR}, &ctx);

  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "W.w\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#w\n"
                    "W..\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, "..w\n"
                    ".#.\n"
                    "W..\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, ".q.\n"
                    ".#.\n"
                    "W..\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, ".q.\n"
                    ".#.\n"
                    "W..\n", ee);
  delete l;
}

// Movimientos fantasma con repetición
TEST(EE, PasarConRepeticion) {
  Localizador *l = new LocalizadorEstatico({
      {"0", {PosYDir({0, 0}, ARRIBA)}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({2, 0}, ARRIBA),
                     {MARRIBA, MARRIBA, DISPARAR}, &ctx);

  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "W.w\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#w\n"
                    "W..\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, "..w\n"
                    ".#.\n"
                    "W..\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, "..w\n"
                    ".#.\n"
                    "W..\n", ee);

  for (int i = 0; i < 5; i++) {
    ee.pasar();
    VALIDAR_ESTADO(1, "..w\n"
                      ".#.\n"
                      "W..\n", ee);
  }

  ee.pasar();
  VALIDAR_ESTADO(1, "..e\n"
                    ".#.\n"
                    "W..\n", ee);
 
  ee.pasar();
  VALIDAR_ESTADO(1, "..e\n"
                   ".#.\n"
                   "W..\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#e\n"
                    "W..\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "W.e\n", ee);

  for (int i = 0; i < 5; i++) {
    ee.pasar();
    VALIDAR_ESTADO(1, "...\n"
                      ".#.\n"
                      "W.e\n", ee);
  }
 
  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "W.w\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#w\n"
                    "W..\n", ee);
  delete l;
}


// Fantasma mata jugador
TEST(EE, FantasmaMataJugador) {
  Localizador *l = new LocalizadorEstatico({
      {"0", {PosYDir({0, 0}, ARRIBA)}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({2, 0}, ARRIBA),
                     {ESPERAR, MIZQUIERDA, DISPARAR}, &ctx);
  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "W.w\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "W.w\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "Wq.\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    ".q.\n", ee);
  delete l;
}

// Fantasma mata varios jugadores
TEST(EE, FantasmaMataVariosJugadores) {
  Localizador *l = new LocalizadorEstatico({
      {"0", {PosYDir({1, 0}, ARRIBA)}},
      {"1", {PosYDir({2, 0}, DERECHA)}}
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0", "1"};

  ExtremeExorcism ee(h, js, PosYDir({0, 0}, DERECHA),
                     {DISPARAR}, &ctx);
  VALIDAR_ESTADO(2, "...\n"
                    ".#.\n"
                    "rWF\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(2, "...\n"
                    ".#.\n"
                    "r..\n", ee);
  delete l;
}

// Fantasma mata jugadores en la misma posición
TEST(EE, FantasmaMataJugadoresEnMismaPosicion) {
  Localizador *l = new LocalizadorEstatico({
      {"0", {PosYDir({1, 0}, ARRIBA)}},
      {"1", {PosYDir({1, 0}, DERECHA)}}
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0", "1"};

  ExtremeExorcism ee(h, js, PosYDir({0, 0}, DERECHA),
                     {DISPARAR}, &ctx);
  EXPECT_TRUE(ee.jugadorVivo("0"));
  EXPECT_TRUE(ee.jugadorVivo("1"));
  EXPECT_EQ(ee.posicionJugador("0"), PosYDir({1, 0}, ARRIBA));
  EXPECT_EQ(ee.posicionJugador("1"), PosYDir({1, 0}, DERECHA));
  
  ee.pasar();
  EXPECT_FALSE(ee.jugadorVivo("0"));
  EXPECT_FALSE(ee.jugadorVivo("1"));
  VALIDAR_ESTADO(2, "...\n"
                    ".#.\n"
                    "r..\n", ee);
  delete l; 
}

// Disparo fantasma bloqueado por mapa
TEST(EE, FantasmaNoMataJugadorEnSuCasillero) {
  Localizador *l = new LocalizadorEstatico({
      {"0", {PosYDir({0, 1}, ARRIBA)}},
      {"1", {PosYDir({0, 2}, DERECHA)}}
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0", "1"};

  ExtremeExorcism ee(h, js, PosYDir({0, 1}, ARRIBA),
                     {DISPARAR}, &ctx);
  EXPECT_TRUE(ee.jugadorVivo("0"));
  EXPECT_TRUE(ee.jugadorVivo("1"));
  EXPECT_EQ(ee.posicionJugador("0"), PosYDir({0, 1}, ARRIBA));
  EXPECT_EQ(ee.posicionJugador("1"), PosYDir({0, 2}, DERECHA));
  EXPECT_EQ(ee.posicionFantasmas(), list<PosYDir>{PosYDir({0, 1}, ARRIBA)});
  
  ee.pasar();
  EXPECT_TRUE(ee.jugadorVivo("0"));
  EXPECT_FALSE(ee.jugadorVivo("1"));
  EXPECT_EQ(ee.posicionJugador("0"), PosYDir({0, 1}, ARRIBA));
  EXPECT_EQ(ee.posicionFantasmas(), list<PosYDir>{PosYDir({0, 1}, ARRIBA)});
  delete l; 
}

// Fantasma dispara y jugador se mueve simultáneamente
TEST(EE, JugadorEsquivaDisparo) {
  Localizador *l = new LocalizadorEstatico({
      {"0", {PosYDir({0, 0}, ARRIBA)}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({2, 0}, ARRIBA),
                     {ESPERAR, MIZQUIERDA, DISPARAR}, &ctx);
  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "W.w\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "W.w\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "...\n"
                    ".#.\n"
                    "Wq.\n", ee);

  ee.ejecutarAccion("0", MARRIBA);
  VALIDAR_ESTADO(1, "...\n"
                    "W#.\n"
                    ".q.\n", ee);
}

// Disparo jugador bloqueado por mapa
TEST(EE, DisparoJugadorBloqueado) {
  Localizador *l = new LocalizadorEstatico({
      {"0", {PosYDir({1, 0}, ARRIBA)}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({1, 2}, ABAJO),
                     {DISPARAR}, &ctx);
  VALIDAR_ESTADO(1, ".e.\n"
                 ".#.\n"
                 ".W.\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(1, ".e.\n"
                 ".#.\n"
                 ".W.\n", ee);
  delete l; 
}

// Cambio de ronda, fantasma nuevo, 1 jugador
TEST(EE, CambioDeRonda) {
  Localizador *l = new LocalizadorEstatico({
      {1, {{"0", {PosYDir({1, 0}, ARRIBA)}}}},
      {2, {{"0", {PosYDir({2, 2}, ARRIBA)}}}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({0, 2}, ARRIBA),
                     {ESPERAR, ESPERAR, ESPERAR}, &ctx);

  VALIDAR_ESTADO(1, "w..\n"
                 ".#.\n"
                 ".W.\n", ee);
  
  ee.ejecutarAccion("0", MIZQUIERDA);
  VALIDAR_ESTADO(1, "w..\n"
                 ".#.\n"
                 "Q..\n", ee);

  ee.ejecutarAccion("0", MARRIBA);
  VALIDAR_ESTADO(1, "w..\n"
                 "W#.\n"
                 "...\n", ee);
  
  ee.ejecutarAccion("0", DISPARAR);
  VALIDAR_ESTADO(1, "w.W\n"
                 ".#.\n"
                 ".s.\n", ee);
  delete l;
}

// Fantasma no equiva disparo
TEST(EE, FantasmaNoEsquivaDisparo) {
  Localizador *l = new LocalizadorEstatico({
      {1, {{"0", {PosYDir({0, 0}, ARRIBA)}}}},
      {2, {{"0", {PosYDir({2, 2}, ARRIBA)}}}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({0, 2}, ARRIBA),
                     list<Accion>{DERECHA}, &ctx);

  VALIDAR_ESTADO(1, "w..\n"
                    ".#.\n"
                    "W..\n", ee);
  ee.ejecutarAccion("0", DISPARAR);

  VALIDAR_ESTADO(1, "w.W\n"
                    ".#.\n"
                    "s..\n", ee);

  delete l;
}

// Movimientos fantasma nuevo con repetición
TEST(EE, FantasmaNuevo) {
  Localizador *l = new LocalizadorEstatico({
      {1, {{"0", {PosYDir({1, 0}, ARRIBA)}}}},
      {2, {{"0", {PosYDir({2, 2}, ARRIBA)}}}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({0, 2}, ARRIBA),
                     {ESPERAR, ESPERAR, ESPERAR}, &ctx);

  ee.ejecutarAccion("0", MIZQUIERDA);
  ee.ejecutarAccion("0", MARRIBA);
  ee.ejecutarAccion("0", DISPARAR);

  VALIDAR_ESTADO(1, "w.W\n"
                 ".#.\n"
                 ".s.\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, "w.W\n"
                 ".#.\n"
                 "a..\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(1, "w.W\n"
                 "s#.\n"
                 "...\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "w.W\n"
                 "s#.\n"
                 "...\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 1);
  
  for (int i = 0; i < 5; i++) {
    ee.pasar();
    VALIDAR_ESTADO(1, "w.W\n"
                   "s#.\n"
                   "...\n", ee);
    EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  }
  
  ee.pasar();
  VALIDAR_ESTADO(1, "e.W\n"
                 "d#.\n"
                 "...\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 1);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "e.W\n"
                 "d#.\n"
                 "...\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "e.W\n"
                 ".#.\n"
                 "f..\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  
  ee.pasar();
  VALIDAR_ESTADO(1, "e.W\n"
                    ".#.\n"
                    ".d.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);

  for (int i = 0; i < 5; i++) {
    ee.pasar();
    VALIDAR_ESTADO(1, "e.W\n"
                   ".#.\n"
                   ".d.\n", ee);
    EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  }
  
  ee.pasar();
  VALIDAR_ESTADO(1, "w.W\n"
                 ".#.\n"
                 ".s.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);

  delete l;
}

// Cambio de ronda, fantasma nuevo, 2 jugadores
TEST(EE, CambioDeRonda_2J) {
  Localizador *l = new LocalizadorEstatico({
      {1, {{"0", {PosYDir({1, 0}, ARRIBA)}}, {"1", PosYDir({2, 0}, ARRIBA)}}},
      {2, {{"0", {PosYDir({1, 0}, ARRIBA)}}, {"1", PosYDir({0, 0}, ARRIBA)}}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0", "1"};

  ExtremeExorcism ee(h, js, PosYDir({0, 2}, ARRIBA),
                     {ESPERAR, ESPERAR, ESPERAR}, &ctx);

  VALIDAR_ESTADO(2, "w..\n"
                    ".#.\n"
                    ".WS\n", ee);
  
  ee.ejecutarAccion("0", MIZQUIERDA);
  VALIDAR_ESTADO(2, "w..\n"
                    ".#.\n"
                    "Q.S\n", ee);
  
  ee.ejecutarAccion("0", MARRIBA);
  VALIDAR_ESTADO(2, "w..\n"
                    "W#.\n"
                    "..S\n", ee);

  ee.ejecutarAccion("1", MARRIBA);
  VALIDAR_ESTADO(2, "w..\n"
                    "W#S\n"
                    "...\n", ee);

  ee.ejecutarAccion("1", MARRIBA);
  VALIDAR_ESTADO(2, "w.S\n"
                    "W#.\n"
                    "...\n", ee);
  
  ee.ejecutarAccion("1", MIZQUIERDA);
  VALIDAR_ESTADO(2, "wA.\n"
                    "W#.\n"
                    "...\n", ee);
  
  ee.ejecutarAccion("1", DISPARAR);
  VALIDAR_ESTADO(2, "w..\n"
                    ".#.\n"
                    "SWs\n", ee);

  // Nueva ronda
  ee.pasar();
  VALIDAR_ESTADO(2, "w..\n"
                    ".#.\n"
                    "SWs\n", ee);

  ee.pasar();
  VALIDAR_ESTADO(2, "w..\n"
                    ".#.\n"
                    "SWs\n", ee);
  
  ee.pasar();
  VALIDAR_ESTADO(2, "w..\n"
                    ".#s\n"
                    "SW.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  
  ee.pasar();
  VALIDAR_ESTADO(2, "w.s\n"
                    ".#.\n"
                    "SW.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);

  ee.pasar();
  VALIDAR_ESTADO(2, "wa.\n"
                    ".#.\n"
                    "SW.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  
  ee.pasar();
  VALIDAR_ESTADO(2, "wa.\n"
                    ".#.\n"
                    "SW.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 1);
  
  for (int i = 0; i < 2; i++) {
    ee.pasar();
    VALIDAR_ESTADO(2, "wa.\n"
                      ".#.\n"
                      "SW.\n", ee);
    EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  }

  // Inversion fantasma 1
  
  for (int i = 0; i < 3; i++) {
    ee.pasar();
    VALIDAR_ESTADO(2, "ea.\n"
                      ".#.\n"
                      "SW.\n", ee);
    EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  }

  // Inversion fantasma 2

  ee.pasar();
  VALIDAR_ESTADO(2, "ef.\n"
                    ".#.\n"
                    "SW.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 1);
  
  ee.pasar();
  VALIDAR_ESTADO(2, "ef.\n"
                    ".#.\n"
                    "SW.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  
  ee.pasar();
  VALIDAR_ESTADO(2, "e.d\n"
                    ".#.\n"
                    "SW.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  
  ee.pasar();
  VALIDAR_ESTADO(2, "e..\n"
                    ".#d\n"
                    "SW.\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  
  ee.pasar();
  VALIDAR_ESTADO(2, "e..\n"
                    ".#.\n"
                    "SWd\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);

  ee.pasar();
  VALIDAR_ESTADO(2, "e..\n"
                    ".#.\n"
                    "SWd\n", ee);
  
  // Inversion Fantasma 1
  
  for (int i = 0; i < 6; i++) {
    ee.pasar();
    VALIDAR_ESTADO(2, "w..\n"
                      ".#.\n"
                      "SWd\n", ee);
    EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  }
    
  ee.pasar();
  VALIDAR_ESTADO(2, "w..\n"
                    ".#.\n"
                    "SWs\n", ee);
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  delete l;
}

string stretch_map(unsigned int height, string row) {
  stringstream ss;
  for (int i = 0; i < height - 1; i++) {
    for (int j = 0; j < row.size() - 1; j++) {
      ss << '.';
    }
    ss << '\n';
  }
  ss << row;
  return ss.str();
}

// Disparo jugador mata dos fantasmas
TEST(EE, MuerenVariosFantasmas) {
  Localizador *l = new LocalizadorEstatico({
      {1, {{"0", {PosYDir({3, 0}, IZQUIERDA)}}}},
      {2, {{"0", {PosYDir({7, 0}, IZQUIERDA)}}}},
      {3, {{"0", {PosYDir({5, 0}, IZQUIERDA)}}}},
  });
  Habitacion h(9, {});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({0, 0}, ARRIBA),
                     {ESPERAR, ESPERAR, ESPERAR, ESPERAR, ESPERAR}, &ctx);

  VALIDAR_ESTADO(1, stretch_map(9, "w..Q.....\n"), ee);
  ee.pasar();
  ee.pasar();
  ee.pasar();
  ee.pasar();
  ee.pasar();
  ee.ejecutarAccion("0", DISPARAR);
  VALIDAR_ESTADO(1, stretch_map(9, "w..a...Q.\n"), ee);
  ee.pasar();
  ee.pasar();
  ee.pasar();
  ee.ejecutarAccion("0", DISPARAR);
  VALIDAR_ESTADO(1, stretch_map(9, "w..a.Q.z.\n"), ee);
  ee.pasar();
  ee.ejecutarAccion("0", DISPARAR);
  VALIDAR_ESTADO(1, stretch_map(9, ".....Q.z.\n"), ee);
  delete l;
}

// Ningun fantasma dispara
TEST(EE, NingunFantasmaDispara) {
  Localizador *l = new LocalizadorEstatico({
      {1, {{"0", {PosYDir({2, 0}, IZQUIERDA)}}}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({0, 0}, ARRIBA),
      {ESPERAR, MARRIBA, MIZQUIERDA, MDERECHA, MABAJO}, &ctx);

  for (int i = 0; i < 6; i++) {
    EXPECT_EQ(ee.disparosFantasmas().size(), 0);
    ee.pasar();
  }
}

// Un fantasma dispara
TEST(EE, UnFantasmaDispara) {
  Localizador *l = new LocalizadorEstatico({
      {1, {{"0", {PosYDir({2, 0}, IZQUIERDA)}}}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({0, 0}, ARRIBA),
      {ESPERAR, MARRIBA, MIZQUIERDA, MDERECHA, MABAJO, DISPARAR}, &ctx);

  for (int i = 0; i < 6; i++) {
    EXPECT_EQ(ee.disparosFantasmas().size(), 0);
    ee.pasar();
  }

  EXPECT_EQ(ee.disparosFantasmas().size(), 1);
}

// Dos fantasmas disparan
TEST(EE, DosFantasmasDisparan) {
  Localizador *l = new LocalizadorEstatico({
      {1, {{"0", {PosYDir({0, 0}, IZQUIERDA)}}}},
      {2, {{"0", {PosYDir({2, 2}, IZQUIERDA)}}}},
  });
  Habitacion h(3, {{1, 1}});
  Contexto ctx(l);
  set<Jugador> js{"0"};

  ExtremeExorcism ee(h, js, PosYDir({0, 2}, ARRIBA),
      {ESPERAR, DISPARAR}, &ctx);

  ee.ejecutarAccion("0", MARRIBA);
  ee.ejecutarAccion("0", DISPARAR);

  VALIDAR_ESTADO(1, "w.Q\n"
                    ".#.\n"
                    "a..\n", ee);

  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  ee.pasar();
  EXPECT_EQ(ee.disparosFantasmas().size(), 0);
  ee.pasar();
  EXPECT_EQ(ee.disparosFantasmas().size(), 2);
}

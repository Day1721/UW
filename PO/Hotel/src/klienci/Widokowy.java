package klienci;

import java.util.Calendar;

import hotel.Klient;
import hotel.Pokój;
import hotel.Zamówienie;

public class Widokowy extends Klient {

	public Widokowy(String imięINazwisko, Calendar dataPrzyjazdu, int czasPobytu, Pokój preferowanyPokój) {
		super(imięINazwisko, dataPrzyjazdu, czasPobytu, preferowanyPokój);
	}

	@Override
	public boolean czyAkceptujeszPropozycję(Pokój pokój, Zamówienie zamówienie) {
		if(pokój == null)return false;
		return pokój.kierunek() == zamówienie.preferowanyPokój().kierunek();
	}

	@Override
	public String nazwa() {
		return "widokowa";
	}

}

package klienci;

import java.util.Calendar;

import hotel.Klient;
import hotel.Pokój;
import hotel.Zamówienie;

public class Ugodowy extends Klient {

	public Ugodowy(String imięINazwisko, Calendar dataPrzyjazdu, int czasPobytu, Pokój preferowanyPokój) {
		super(imięINazwisko, dataPrzyjazdu, czasPobytu, preferowanyPokój);
	}

	@Override
	public boolean czyAkceptujeszPropozycję(Pokój pokój, Zamówienie zamówienie) {
		return pokój != null;
	}

	@Override
	public String nazwa() {
		return "ugodowa";
	}

}

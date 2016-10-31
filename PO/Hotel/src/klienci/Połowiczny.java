package klienci;

import java.util.Calendar;

import hotel.Klient;
import hotel.Pokój;
import hotel.Zamówienie;

public class Połowiczny extends Klient {

	public Połowiczny(String imięINazwisko, Calendar dataPrzyjazdu, int czasPobytu, Pokój preferowanyPokój) {
		super(imięINazwisko, dataPrzyjazdu, czasPobytu, preferowanyPokój);
	}
	
	private int porównajPokoje(Pokój pokój1, Pokój pokój2){
		return (pokój1.liczbaOsób() <= pokój2.liczbaOsób() ? 0 : 1) + (pokój1.cena() <= pokój2.cena() ? 0 : 1) + 
				(pokój1.styl() == pokój2.styl() ? 0 : 1) + (pokój1.kolor() == pokój2.kolor() ? 0 : 1) + 
				(pokój1.kierunek() == pokój2.kierunek() ? 0 : 1) + (pokój1.internet() == pokój2.internet() ? 0 : 1);
	}

	@Override
	public boolean czyAkceptujeszPropozycję(Pokój pokój, Zamówienie zamówienie) {
		if(pokój == null)return false;
		return porównajPokoje(pokój, zamówienie.preferowanyPokój()) <= 3;
	}

	@Override
	public String nazwa() {
		return "połowiczna";
	}

}

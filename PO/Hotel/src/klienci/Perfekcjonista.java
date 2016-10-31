package klienci;

import java.util.Calendar;

import hotel.Klient;
import hotel.Pokój;
import hotel.Zamówienie;

public class Perfekcjonista extends Klient {

	public Perfekcjonista(String imięINazwisko, Calendar dataPrzyjazdu, int czasPobytu, Pokój preferowanyPokój) {
		super(imięINazwisko, dataPrzyjazdu, czasPobytu, preferowanyPokój);
	}

	@Override
	public boolean czyAkceptujeszPropozycję(Pokój pokój, Zamówienie zamówienie){
		if(pokój == null)return false;
		Pokój preferowanyPokój = zamówienie.preferowanyPokój();
		return preferowanyPokój.porównaj(pokój);
	}
	
	@Override
	public String nazwa(){
		return "perfekcjonistyczna";
	}
}

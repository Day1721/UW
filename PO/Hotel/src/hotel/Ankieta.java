package hotel;

import java.util.Calendar;

public class Ankieta {
	private Calendar dataPrzyjazdu;
	private int czasPobytu;
	private Pokój preferowanyPokój;
	public Ankieta(int czasPobytu, Pokój pokój, Calendar dataPrzyjazdu){
		this.czasPobytu = czasPobytu;
		this.preferowanyPokój = pokój;
		this.dataPrzyjazdu = dataPrzyjazdu;
	}
	public Calendar dataPrzyjazdu(){
		return (Calendar) dataPrzyjazdu.clone();
	}
	public int liczbaDni(){
		return czasPobytu;
	}
	public Pokój preferowanyPokój(){
		return preferowanyPokój;
	}
}

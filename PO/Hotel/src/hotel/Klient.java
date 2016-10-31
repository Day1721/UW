package hotel;

import java.util.Calendar;

public abstract class Klient {
	protected String imięINazwisko;
	protected Calendar dataPrzyjazdu;
	protected int czasPobytu;
	protected Pokój preferowanyPokój;
	public abstract boolean czyAkceptujeszPropozycję(Pokój pokój, Zamówienie zamówienie);
	public abstract String nazwa();
	
	public Klient(String imięINazwisko, Calendar dataPrzyjazdu, int czasPobytu, Pokój preferowanyPokój){
		this.imięINazwisko = imięINazwisko;
		this.dataPrzyjazdu = dataPrzyjazdu;
		this.czasPobytu = czasPobytu;
		this.preferowanyPokój = preferowanyPokój;
	}
	
	public Ankieta wypełnijAnkietę(){
		return new Ankieta(czasPobytu, preferowanyPokój, dataPrzyjazdu);
	}
	
	@Override
	public String toString(){
		return imięINazwisko + ", " + nazwa();
		
	}
}

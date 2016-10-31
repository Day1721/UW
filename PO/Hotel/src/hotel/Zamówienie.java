package hotel;

import java.util.Calendar;

public class Zamówienie {
	private int licznik;
	private Klient klient;
	private Pokój preferowanyPokój;
	private Calendar dataPrzyjazdu;
	private int czasPobytu;
	public Zamówienie(Klient klient, Pokój preferowanyPokój, Calendar dataPrzyjazdu, int czasPobytu){
		this.klient = klient;
		this.preferowanyPokój = preferowanyPokój;
		this.dataPrzyjazdu = dataPrzyjazdu;
		this.czasPobytu = czasPobytu;
		licznik = 0;
	}
	public Klient klient(){
		return klient;
	}
	public int licznik(){
		return licznik;
	}
	public void zwiększLicznik(){
		licznik++;
	}
	@Override
	public String toString(){
		return "Data przyjazdu: " + dataPrzyjazdu.get(Calendar.DAY_OF_MONTH) + "." + dataPrzyjazdu.get(Calendar.MONTH) + "." 
				+ dataPrzyjazdu.get(Calendar.YEAR) + ", czas pobytu: " + czasPobytu + (czasPobytu == 1 ? " dzień, " : " dni, ") +
				"preferencja pokoju: " + preferowanyPokój.wypiszOpis();
	}
	public PrzedziałCzasowy przedziałCzasowy(){
		return new PrzedziałCzasowy(dataPrzyjazdu, czasPobytu);
	}
	public Pokój preferowanyPokój(){
		return preferowanyPokój;
	}
}

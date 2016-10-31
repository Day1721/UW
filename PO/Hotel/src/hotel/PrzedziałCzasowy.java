package hotel;

import java.util.Calendar;

public class PrzedziałCzasowy {
	private Calendar początek;
	private Calendar koniec;
	public PrzedziałCzasowy(Calendar początek, Calendar koniec){
		if(początek.compareTo(koniec) > 0){
			this.początek = koniec;
			this.koniec = początek;
		}
		else{
			this.początek = początek;
			this.koniec = koniec;
		}
	}
	public PrzedziałCzasowy(Calendar początek, int ilośćDni){
		this.koniec = this.początek = początek;
		this.koniec.add(Calendar.DAY_OF_YEAR, ilośćDni);
	}
	public Calendar początek(){
		return (Calendar) początek.clone();
	}
	public Calendar koniec(){
		return (Calendar) koniec.clone();
	}
	public int porównaj(PrzedziałCzasowy przedział){
		if(this.koniec.compareTo(przedział.początek) < 0){
			return -1;
		}
		if(this.początek.compareTo(przedział.koniec) > 0){
			return 1;
		}
		return 0;
	}
}

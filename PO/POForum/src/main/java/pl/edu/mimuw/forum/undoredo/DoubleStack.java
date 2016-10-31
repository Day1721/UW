package pl.edu.mimuw.forum.undoredo;

import java.util.Stack;

import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;

public class DoubleStack<T> {
	private Stack<T> undo;
	private Stack<T> redo;
	private boolean isEnabled;
	
	public DoubleStack(){
		undo = new Stack<T>();
		redo = new Stack<T>();
		isEnabled = true;
	}
	
	public void add(T elt){
		if(!isEnabled) return;
		redo.clear();
		undo.push(elt);
		canRedo.set(false);
		canUndo.set(true);
	}
	
	public boolean canUndo(){
		return !undo.isEmpty();
	}
	
	public boolean canRedo(){
		return !redo.isEmpty();
	}
	
	public T undo(){
		T elt = undo.pop();
		redo.push(elt);
		canUndo.set(canUndo());
		canRedo.set(canRedo());
		return elt;
	}
	
	public T redo(){
		T elt = redo.pop();
		undo.push(elt);
		canUndo.set(canUndo());
		canRedo.set(canRedo());
		return elt;
	}
	
	public void clearUndo(){
		undo.clear();
		canUndo.set(false);
	}
	
	public void enable(){
		isEnabled = true;
	}
	
	public void disable(){
		isEnabled = false;
	}
	
	public final BooleanProperty canUndo = new SimpleBooleanProperty(false);
	public final BooleanProperty canRedo = new SimpleBooleanProperty(false);
}

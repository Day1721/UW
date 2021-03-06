package pl.edu.mimuw.forum.ui.controllers;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintWriter;
import java.io.Reader;
import java.net.URL;
import java.util.List;
import java.util.Optional;
import java.util.ResourceBundle;

import com.thoughtworks.xstream.XStream;
import com.thoughtworks.xstream.io.xml.DomDriver;

import javafx.beans.binding.Bindings;
import javafx.beans.binding.BooleanBinding;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import pl.edu.mimuw.forum.exceptions.ApplicationException;
import pl.edu.mimuw.forum.ui.bindings.MainPaneBindings;
import pl.edu.mimuw.forum.ui.helpers.DialogHelper;
import pl.edu.mimuw.forum.ui.models.NodeViewModel;
import pl.edu.mimuw.forum.ui.models.SuggestionViewModel;
import pl.edu.mimuw.forum.ui.models.SurveyViewModel;
import pl.edu.mimuw.forum.ui.models.TaskViewModel;
import pl.edu.mimuw.forum.ui.tree.ForumTreeItem;
import pl.edu.mimuw.forum.ui.tree.TreeLabel;
import pl.edu.mimuw.forum.undoredo.DoubleStack;
import pl.edu.mimuw.forum.undoredo.ICommand;
import pl.edu.mimuw.forum.undoredo.LambdaCommand;

/**
 * Kontroler glownego widoku reprezentujacego forum.
 * Widok sklada sie z drzewa zawierajacego wszystkie wezly forum oraz
 * panelu z polami do edycji wybranego wezla.
 * @author konraddurnoga
 */
public class MainPaneController implements Initializable {

	/**
	 * Korzen drzewa-modelu forum.
	 */
	private NodeViewModel document;

	/**
	 * Wiazania stosowane do komunikacji z {@link pl.edu.mimuw.forum.ui.controller.ApplicationController }.
	 */
	private MainPaneBindings bindings;

	/**
	 * Widok drzewa forum (wyswietlany w lewym panelu).
	 */
	@FXML
	private TreeView<NodeViewModel> treePane;

	/**
	 * Kontroler panelu wyswietlajacego pola do edycji wybranego wezla w drzewie.
	 */
	@FXML
	private DetailsPaneController detailsController;
	
	/**
	 * 2-Stos do obsługi undo/redo
	 */
	public DoubleStack<ICommand> stack = new DoubleStack<>();

	public MainPaneController() {
		bindings = new MainPaneBindings();
	}

	@Override
	public void initialize(URL location, ResourceBundle resources) {
		BooleanBinding nodeSelectedBinding = Bindings.isNotNull(treePane.getSelectionModel().selectedItemProperty());
		bindings.nodeAdditionAvailableProperty().bind(nodeSelectedBinding);
		bindings.nodeRemovaleAvailableProperty()
				.bind(nodeSelectedBinding.and(
						Bindings.createBooleanBinding(() -> getCurrentTreeItem().orElse(null) != treePane.getRoot(),
								treePane.rootProperty(), nodeSelectedBinding)));
		
		bindings.hasChangesProperty().set(false);		// Nalezy ustawic na true w przypadku, gdy w widoku sa zmiany do
														// zapisania i false wpp, w odpowiednim miejscu kontrolera (niekoniecznie tutaj)
														// Spowoduje to dodanie badz usuniecie znaku '*' z tytulu zakladki w ktorej
														// otwarty jest plik - '*' oznacza niezapisane zmiany
		bindings.undoAvailableProperty().set(false);	
		bindings.redoAvailableProperty().set(false);	// Podobnie z undo i redo
		
		stack.canUndo.addListener((observer, oldValue, newValue) -> {
			bindings.undoAvailableProperty().set(newValue);
			bindings.hasChangesProperty().set(stack.canUndo());
		});
		
		stack.canRedo.addListener((observer, oldValue, newValue) -> {
			bindings.redoAvailableProperty().set(newValue);
			bindings.hasChangesProperty().set(stack.canUndo());
		}); 
		
	}

	public MainPaneBindings getPaneBindings() {
		return bindings;
	}

	/**
	 * Otwiera plik z zapisem forum i tworzy reprezentacje graficzna wezlow forum.
	 * @param file
	 * @return
	 * @throws ApplicationException
	 */
	public Node open(File file) throws ApplicationException {
		if (file != null) {
			XStream xstream = new XStream(new DomDriver("Unicode"));
			xstream.addImplicitCollection(pl.edu.mimuw.forum.data.Node.class, "children");
			try {
				Reader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file), "UTF-8"));
				ObjectInputStream inStream = xstream.createObjectInputStream(reader);
				pl.edu.mimuw.forum.data.Node node = (pl.edu.mimuw.forum.data.Node) inStream.readObject();
				document = node.getModel();
				AddListenersToTree(document);
			} catch (Exception e){
				throw new ApplicationException(e);
			}
		} else {
			document = new NodeViewModel("Welcome to a new forum", "Admin");
		}

		/** Dzieki temu kontroler aplikacji bedzie mogl wyswietlic nazwe pliku jako tytul zakladki.
		 * Obsluga znajduje sie w {@link pl.edu.mimuw.forum.ui.controller.ApplicationController#createTab }
		 */
		getPaneBindings().fileProperty().set(file);

		return openInView(document);
	}
	
	private void AddListenersToTree(NodeViewModel node){
		this.addListenerToNode(node);
		if(node.getChildren() != null)
			node.getChildren().forEach((child) -> AddListenersToTree(child));
	}

	/**
	 * Zapisuje aktualny stan forum do pliku.
	 * @throws ApplicationException
	 */
	public void save() throws ApplicationException {
		pl.edu.mimuw.forum.data.Node node = document.toNode();
		XStream xstream = new XStream(new DomDriver("Unicode"));
		xstream.addImplicitCollection(pl.edu.mimuw.forum.data.Node.class, "children");
		File file = getPaneBindings().fileProperty().get();
		try {
			PrintWriter writer = new PrintWriter(file, "UTF-8");
			ObjectOutputStream outStream = xstream.createObjectOutputStream(writer, "Forum");
			outStream.writeObject(node);
			outStream.close();
		} catch (Exception e) {
			throw new ApplicationException(e);
		}
		if (document != null) {
			System.out.println("On save " + document.toNode());	//Tak tworzymy drzewo do zapisu z modelu aplikacji
		}
		stack.clearUndo();
	}
	
	/**
	 * Cofa ostatnio wykonana operacje na forum.
	 * @throws ApplicationException
	 */
	public void undo() throws ApplicationException {
		if(!stack.canUndo()) throw new ApplicationException();
		stack.disable();
		stack.undo().undo();
		stack.enable();
		System.out.println("On undo");	//TODO Tutaj umiescic obsluge undo
	}

	/**
	 * Ponawia ostatnia cofnieta operacje na forum.
	 * @throws ApplicationException
	 */
	public void redo() throws ApplicationException {
		if(!stack.canRedo()) throw new ApplicationException();
		stack.disable();
		stack.redo().redo();
		stack.enable();
		System.out.println("On redo");	//TODO Tutaj umiescic obsluge redo
	}

	/**
	 * Podaje nowy wezel jako ostatnie dziecko aktualnie wybranego wezla.
	 * @param node
	 * @throws ApplicationException
	 */
	public void addNode(NodeViewModel node) throws ApplicationException {
		addListenerToNode(node);
		getCurrentNode().ifPresent(currentlySelected -> {
			currentlySelected.getChildren().add(node);		// Zmieniamy jedynie model, widok (TreeView) jest aktualizowany z poziomu
															// funkcji nasluchujacej na zmiany w modelu (zob. metode createViewNode ponizej)
		});
	}
	
	private void addListenerToNode(NodeViewModel node){
		node.getAuthor().addListener((observer, oldValue, newValue) -> {
			stack.add(new LambdaCommand(
					() -> node.getAuthor().setValue(oldValue),
					() -> node.getAuthor().setValue(newValue)
				));
		});
		node.getContent().addListener((observer, oldValue, newValue) -> {
			stack.add(new LambdaCommand(
					() -> node.getContent().setValue(oldValue),
					() -> node.getContent().setValue(newValue)
				));
		});
		switch(node.getName()){
		case "Suggestion":
			SuggestionViewModel suggestion = (SuggestionViewModel) node;
			suggestion.getResponse().addListener((observer, oldValue, newValue) -> {
				stack.add(new LambdaCommand(
						() -> suggestion.getResponse().setValue(oldValue),
						() -> suggestion.getResponse().setValue(newValue)
					));
			});
			suggestion.getIsResponseAccepted().addListener((observer, oldValue, newValue) -> {
				stack.add(new LambdaCommand(
						() -> suggestion.getIsResponseAccepted().setValue(oldValue),
						() -> suggestion.getIsResponseAccepted().setValue(newValue)
					));
			});
			break;
		case "Task":
			TaskViewModel task = (TaskViewModel) node;
			task.getDueDate().addListener((observer, oldValue, newValue) -> {
				stack.add(new LambdaCommand(
						() -> task.getDueDate().setValue(oldValue),
						() -> task.getDueDate().setValue(newValue)
					));
			});
			break;
		case "Survey":
			SurveyViewModel survey = (SurveyViewModel) node;
			survey.getLikes().addListener((observer, oldValue, newValue) -> {
				stack.add(new LambdaCommand(
						() -> survey.getLikes().setValue(oldValue),
						() -> survey.getLikes().setValue(newValue)
					));
			});
			survey.getDislikes().addListener((observer, oldValue, newValue) -> {
				stack.add(new LambdaCommand(
						() -> survey.getDislikes().setValue(oldValue),
						() -> survey.getDislikes().setValue(newValue)
					));
			});
			break;
		}
	}

	/**
	 * Usuwa aktualnie wybrany wezel.
	 */
	public void deleteNode() {
		getCurrentTreeItem().ifPresent(currentlySelectedItem -> {
			TreeItem<NodeViewModel> parent = currentlySelectedItem.getParent();

			NodeViewModel parentModel;
			NodeViewModel currentModel = currentlySelectedItem.getValue();
			if (parent == null) {
				return; // Blokujemy usuniecie korzenia - TreeView bez korzenia jest niewygodne w obsludze
			} else {
				parentModel = parent.getValue();
				parentModel.getChildren().remove(currentModel); // Zmieniamy jedynie model, widok (TreeView) jest aktualizowany z poziomu
																// funkcji nasluchujacej na zmiany w modelu (zob. metode createViewNode ponizej)
			}

		});
	}

	private Node openInView(NodeViewModel document) throws ApplicationException {
		Node view = loadFXML();

		treePane.setCellFactory(tv -> {
			try {
				//Do reprezentacji graficznej wezla uzywamy niestandardowej klasy wyswietlajacej 2 etykiety
				return new TreeLabel();
			} catch (ApplicationException e) {
				DialogHelper.ShowError("Error creating a tree cell.", e);
				return null;
			}
		});

		ForumTreeItem root = createViewNode(document);
		root.addEventHandler(TreeItem.<NodeViewModel> childrenModificationEvent(), event -> {
			//TODO Moze przydac sie do wykrywania usuwania/dodawania wezlow w drzewie (widoku)
			if (event.wasAdded()) {
				System.out.println("Adding to " + event.getSource());
			}
			
			if (event.wasRemoved()) {
				System.out.println("Removing from " + event.getSource());
			}
		});

		treePane.setRoot(root);

		for (NodeViewModel w : document.getChildren()) {
			addToTree(w, root);
		}

		expandAll(root);

		treePane.getSelectionModel().selectedItemProperty()
				.addListener((observable, oldValue, newValue) -> onItemSelected(oldValue, newValue));

		return view;
	}
	
	private Node loadFXML() throws ApplicationException {
		FXMLLoader loader = new FXMLLoader();
		loader.setController(this);
		loader.setLocation(getClass().getResource("/fxml/main_pane.fxml"));

		try {
			return loader.load();
		} catch (IOException e) {
			throw new ApplicationException(e);
		}
	}
	
	private Optional<? extends NodeViewModel> getCurrentNode() {   //TODO Ważne miejsce !!!
		return getCurrentTreeItem().<NodeViewModel> map(TreeItem::getValue);
	}

	private Optional<TreeItem<NodeViewModel>> getCurrentTreeItem() {
		return Optional.ofNullable(treePane.getSelectionModel().getSelectedItem());
	}

	private void addToTree(NodeViewModel node, ForumTreeItem parentViewNode, int position) {
		ForumTreeItem viewNode = createViewNode(node);

		List<TreeItem<NodeViewModel>> siblings = parentViewNode.getChildren();
		siblings.add(position == -1 ? siblings.size() : position, viewNode);

		node.getChildren().forEach(child -> addToTree(child, viewNode));
	}

	private void addToTree(NodeViewModel node, ForumTreeItem parentViewNode) {
		addToTree(node, parentViewNode, -1);
	}

	private void removeFromTree(ForumTreeItem viewNode) {
		viewNode.removeChildListener();
		TreeItem<NodeViewModel> parent = viewNode.getParent();
		if (parent != null) {
			viewNode.getParent().getChildren().remove(viewNode);
		} else {
			treePane.setRoot(null);
		}
	}

	private ForumTreeItem createViewNode(NodeViewModel node) {
		ForumTreeItem viewNode = new ForumTreeItem(node);
		viewNode.setChildListener(change -> {	// wywolywanem, gdy w modelu dla tego wezla zmieni sie zawartosc kolekcji dzieci
			while (change.next()) {
				if (change.wasAdded()) {
					int i = change.getFrom();
					for (NodeViewModel child : change.getAddedSubList()) {
						// TODO Tutaj byc moze nalezy dodac zapisywanie jaka operacja jest wykonywana
						// by mozna bylo ja odtworzyc przy undo/redo
						final int j = i;
						stack.add(new LambdaCommand(
								() -> removeFromTree((ForumTreeItem) viewNode.getChildren().get(j)), 
								() -> addToTree(child, viewNode, j)));
						addToTree(child, viewNode, i);	// uwzgledniamy nowy wezel modelu w widoku
						i++;
					}
				}

				if (change.wasRemoved()) {
					for (int i = change.getFrom(); i <= change.getTo(); ++i) {
						// TODO Tutaj byc moze nalezy dodac zapisywanie jaka operacja jest wykonywana
						// by mozna bylo ja odtworzyc przy undo/redo
						NodeViewModel child = viewNode.getChildren().get(i).getValue();
						final int j = i;
						removeFromTree((ForumTreeItem) viewNode.getChildren().get(i)); // usuwamy wezel modelu z widoku
						stack.add(new LambdaCommand(
								() -> addToTree(child, viewNode, j),
								() -> removeFromTree((ForumTreeItem) viewNode.getChildren().get(j))));
					}
				}
			}
		});

		return viewNode;
	}

	private void expandAll(TreeItem<NodeViewModel> item) {
		item.setExpanded(true);
		item.getChildren().forEach(this::expandAll);
	}

	private void onItemSelected(TreeItem<NodeViewModel> oldItem, TreeItem<NodeViewModel> newItem) {
		detailsController.setModel(newItem != null ? newItem.getValue() : null);
	}

}

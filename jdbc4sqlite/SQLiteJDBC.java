import java.sql.*;

public class SQLiteJDBC {
	Connection connection = null;
	boolean connected = false;
	
	public SQLiteJDBC() throws ClassNotFoundException{
		Class.forName("org.sqlite.JDBC");
		try    {
		  connection = DriverManager.getConnection("jdbc:sqlite:sample.db");
		  connected = true;
		} catch(SQLException e) {
		  System.err.println(e.getMessage());
		  connected = false;
		}
	}
	
	public String runQuery(){
		String rslt = "";
		try	{
			Statement statement = connection.createStatement();
			statement.setQueryTimeout(30);  // set timeout to 30 sec.
			//statement.executeUpdate("drop table if exists person");
			//statement.executeUpdate("create table person (id integer, name string)");
			//statement.executeUpdate("insert into person values(1, 'leo')");
			//statement.executeUpdate("insert into person values(2, 'yui')");
			ResultSet rs = statement.executeQuery("select * from person");
			while(rs.next()) {
				System.out.println("name = " + rs.getString("name"));
				System.out.println("id = " + rs.getInt("id"));
				rslt += "."+rs.getString("name");
			}
		} catch(SQLException e) {
			System.err.println(e.getMessage());
		}		
		return rslt;
	}
	
	public void disconnect(){
		try {
			if(connection != null)
				connection.close();
		} catch(SQLException e) {
		// connection close failed.
			System.err.println(e);
		}
	}
	
	public static void main(String[] args) throws ClassNotFoundException {
		SQLiteJDBC o = new SQLiteJDBC();
		System.out.println("result="+o.runQuery());
		o.disconnect();
		//javac SQLiteJDBC.java
		//java -cp ".;sqlite-jdbc-3.8.11.2.jar" SQLiteJDBC
	}
}
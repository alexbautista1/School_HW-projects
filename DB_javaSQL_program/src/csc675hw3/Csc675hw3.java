
package csc675hw3;

/**
 *
 * @author Alex Bautista
 * 
 * SFSU CSC675 Due Nov.30 2017
 */
import java.sql.*;
import java.util.Scanner;

public class Csc675hw3 {
     
    public static void main(String[] args) {
        
   //one connection and first set of statement objects     
   Connection c = null;
   Statement stmt = null;
   Statement stmt2 = null;
   Statement stmt3 = null;
   
   
   try {
      Class.forName("org.sqlite.JDBC");
      c = DriverManager.getConnection("jdbc:sqlite:chinook.db");
      c.setAutoCommit(false);
      System.out.println("Opened database successfully");
      System.out.println("======================================");
      System.out.println("What would you like to do?\n1.Obtain album title(s) based on artist's name\n2.Purchase history for a customer\n3.Update track price\nPlease enter a number and press ENTER:");
      Scanner userOption = new Scanner(System.in);
      int userInt = userOption.nextInt();
      Scanner input = new Scanner(System.in);
      
      
      //switch case to handle user options
      //each case uses database library objects to access the database and print to screen
      switch(userInt){
                 //List of albums given artist
          case 1: System.out.println("Name of Artist:");
                  
                  String input2 = input.nextLine();
                  stmt = c.createStatement();
                  ResultSet rs1 = stmt.executeQuery( "SELECT a.title, a.albumid FROM album a, artist t WHERE a.artistid = t.artistid AND t.name = '" + input2 + "';");
      
                  while ( rs1.next() ) {
                  int albumId = rs1.getInt("AlbumId");
                  String  name = rs1.getString("Title");
       
                  System.out.println( "albumID = " + albumId );
                  System.out.println( "NAME = " + name );
         
                  System.out.println();
                  
      }
                  rs1.close();
                  stmt.close();
                  
                  break;
                  
                  //customer history
         case 2:  System.out.println("Customerid:");
                  
                  int customerId = input.nextInt();
                  stmt2 = c.createStatement();
                  ResultSet rs2 = stmt2.executeQuery("SELECT il.quantity, i.invoicedate, a.title, t.name FROM album a, invoiceline il, invoice i, track t WHERE i.invoiceid = il.invoiceid AND il.trackid = t.trackid AND t.albumid = a.albumid AND i.customerid = '" + customerId + "';");  
                  while ( rs2.next() ) {
                  int quantity = rs2.getInt("quantity");
                  int date = rs2.getInt("invoicedate");
                  String  album = rs2.getString("title");
                  String  trackName = rs2.getString("name");
       
                  System.out.println( "Quantity = " + quantity );
                  System.out.println( "Invoice date = " + date );
                  System.out.println( "Albumname = " + album );
                  System.out.println( "Trackname = " + trackName );
         
                  System.out.println();
                  
      }     
                  
                  rs2.close();
                  stmt2.close();
                  
                  break;
                  
                 //update track price
         case 3: System.out.println("Trackid(4 digits):");
                  
                  int trackid = input.nextInt();
                  stmt3 = c.createStatement();
                  ResultSet rs3 = stmt3.executeQuery( "SELECT t.unitprice from track t where t.trackid = '" + trackid + "';");
      
                  while ( rs3.next() ) {
                  float unitPrice = rs3.getFloat("unitprice");
                  System.out.println( "trackprice = " + unitPrice );
                  
         
                  System.out.println();
                  
      }
                  
             System.out.println("Set Price:");
             float newPrice = input.nextFloat();
             Statement stmt4 = null;
             Statement stmt5 = null;
            stmt4 = c.createStatement();
            String sql = "UPDATE track set unitprice = " + newPrice + " where trackid = " + trackid + ";"; 
            stmt4.executeUpdate(sql);
            c.commit();
            stmt5 = c.createStatement();
                  ResultSet rs4 = stmt5.executeQuery( "SELECT t.unitprice from track t where t.trackid = '" + trackid + "';");
      
                  while ( rs4.next() ) {
                  float unitPrice = rs4.getFloat("unitprice");
                  System.out.println( "NEW trackprice = " + unitPrice );
                  
         
                  System.out.println();
                  
      }
                  
            
            
            rs3.close();
            rs4.close();
                  stmt3.close();
                  stmt4.close();
                  stmt5.close();
                  
                  break;     
                  
              
         
      }
      
      //close connection to database
           c.close();
      
      
      
   } catch ( Exception e ) {
      System.err.println( e.getClass().getName() + ": " + e.getMessage() );
      System.exit(0);
   }
   System.out.println("Operation done successfully");
  }
    }

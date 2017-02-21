import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.charset.Charset;
import java.util.stream.Collectors;
import java.nio.file.DirectoryStream;
import java.util.ArrayList;
import java.nio.charset.MalformedInputException;
import java.io.BufferedReader;
import java.io.FileReader;
import java.lang.StringBuilder;

import java.nio.charset.StandardCharsets;


public class LogfileParser {

  public static void main(String[] args) {
    System.out.println("LogfileParser 0.1");

    Path dir = Paths.get("/media/jascha/530abec2-0ac0-4858-bf4b-242cc3dfc37d/git-logs/");
       try (DirectoryStream<Path> stream = Files.newDirectoryStream(dir)) {
           for (Path entry: stream) {
             System.out.println("now parsing file: " + entry.toAbsolutePath().toString());
             parseFile(entry.toAbsolutePath().toString());

           }
       } catch (Exception e) {
      e.printStackTrace();
      System.exit(1);
    }




  }

  public static void parseFile(String filename) throws Exception {

    ArrayList<Commit> cl = new ArrayList<>();
String contents = "";

StringBuilder sb = new StringBuilder();

      try {
        BufferedReader r = new BufferedReader(new FileReader(filename));
                  String line = null;
                while((line=r.readLine())!=null){

sb.append(line + '\n');

                        //  lines.add(line);

           }

         }
         catch(MalformedInputException mie){
                 // ignore or do something
           }

contents = sb.toString();

    //contents = Files.lines(Paths.get(filename), Charset.forName("UTF-8")).collect(Collectors.joining("\n"));

    String[] compos = filename.replace("$","/").split("§");

    String[] commits = contents.split("\ncommit");
    System.out.println("# Commits: " + commits.length);

    commits[0] = commits[0].replace("commit ", "");

    for(int i = 0; i<commits.length; i++) {

      System.out.println("Parsing commit #" + (i+1) + "/" + commits.length);

      Commit c = new Commit();
      c.project = compos[0].replace("/media/jascha/530abec2-0ac0-4858-bf4b-242cc3dfc37d/git-logs/","");
      c.branch = compos[1].replace("remotes/origin/","").replace(".log","");
      BufferedReader reader = new BufferedReader(new StringReader(commits[i]));

      // First Line of a commit is always the hash

      c.hash = reader.readLine().trim();

      // The next 2-3 lines can be merge, author or date.

      String line;

      while((line = reader.readLine())!=null && !line.equals("")) {

        if(line.substring(0, 6).equals("Author")) {
          c.author = line.substring(8);
          String[] ath = c.author.split("<");
          c.author_name = ath[0].trim();
          c.author_email = ath[1].replace(">","").replace(";","+").trim();
          } else if(line.substring(0, 4).equals("Date")) {
          c.date = line.substring(8);
        } else if(line.substring(0, 5).equals("Merge")) {
          c.merge = line.substring(8);
        }

      }

      // Here comes the description of the commit

      while((line = reader.readLine())!=null && !line.equals("")) {
        c.desc += line + "\n";
      }

      // And now the files section

      while((line = reader.readLine())!=null) {

        String[] sf = line.split("\t");

        if(sf.length == 3) {

          File f = new File();

          if(!sf[0].equals("-")) {  // check if not a binary
            f.added = Integer.parseInt(sf[0]);
            f.deleted = Integer.parseInt(sf[1]);
          }

          f.name = sf[2].replace(";","");
          f.setExt();

          c.addFile(f);

        } else {

          c.desc += line + "\n";

        }

      }

      cl.add(c);

    }

    // Build the output csv files

    PrintWriter pw_commits = new PrintWriter(new FileWriter("/media/jascha/530abec2-0ac0-4858-bf4b-242cc3dfc37d/git-csv/" + compos[0].replace("/media/jascha/530abec2-0ac0-4858-bf4b-242cc3dfc37d/git-logs/","").replace("/","_") + "§" + compos[1].replace("/","_") + "_commits.csv"));
    PrintWriter pw_files = new PrintWriter(new FileWriter("/media/jascha/530abec2-0ac0-4858-bf4b-242cc3dfc37d/git-csv/" + compos[0].replace("/media/jascha/530abec2-0ac0-4858-bf4b-242cc3dfc37d/git-logs/","").replace("/","_") + "§" + compos[1].replace("/","_") + "_files.csv"));

    pw_commits.write("project;branch;hash;merge;author_name;author_email;date");
    //pw_commits.write("project;branch;hash;merge;author;author_name;author_email;date;desc");
    pw_files.write("project;branch;hash;added;deleted;name;ext");

    for(Commit c: cl) {

      pw_commits.write("\n" + c.project + ";" + c.branch + ";" + c.hash + ";" + c.merge + ";" + c.author_name + ";" + c.author_email + ";" + c.date);
//      pw_commits.write("\n" + c.project + ";" + c.branch + ";" + c.hash + ";" + c.merge + ";" + c.author + ";" + c.author_name + ";" + c.author_email + ";" + c.date + ";" + c.desc.replace("\n", "<cr>").replace(";", ","));

      for(File f: c.files) {
        pw_files.write("\n" + c.project + ";" + c.branch + ";" + c.hash + ";" + f.added + ";" + f.deleted + ";" + f.name + ";" + f.ext);
      }

    }

    pw_commits.close();
    pw_files.close();
  }


}


class Commit {

  public String project;
  public String branch;
  public String hash;
  public String merge;
  public String author;
  public String author_name;
  public String author_email;
  public String date;
  public String desc = "";
  public ArrayList<File> files;

  Commit() {
    files = new ArrayList<>();
  }

  public void addFile(File f) {
    files.add(f);
  }

}

class File {

  public int added = 0;
  public int deleted = 0;
  public String name;
  public String ext;

  public void setExt() {
    int indexOfExt = name.lastIndexOf('.');
    if(indexOfExt != -1) {
      ext = name.substring(indexOfExt);
    }
  }

}

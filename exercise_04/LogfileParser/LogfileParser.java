import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Collectors;
import java.util.ArrayList;

public class LogfileParser {

  public static void main(String[] args) {
    System.out.println("LogfileParser 0.1");

    ArrayList<Commit> cl = new ArrayList<>();

    try {

      String contents = Files.lines(Paths.get(args[0])).collect(Collectors.joining("\n"));

      String[] commits = contents.split("\ncommit");
      System.out.println("# Commits: " + commits.length);

      commits[0] = commits[0].replace("commit ", "");

      for(int i = 0; i<commits.length; i++) {

        System.out.println("Parsing commit #" + (i+1) + "/" + commits.length);

        Commit c = new Commit();
        BufferedReader reader = new BufferedReader(new StringReader(commits[i]));

        // First Line of a commit is always the hash

        c.hash = reader.readLine().trim();

        // The next 2-3 lines can be merge, author or date.

        String line;

        while(!(line = reader.readLine()).equals("")) {

          if(line.substring(0, 6).equals("Author")) {
            c.author = line.substring(8);
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

            f.name = sf[2];
            f.setExt();

            c.addFile(f);

          } else {

            c.desc += line + "\n";

          }

        }

        cl.add(c);

      }

      // Build the output csv files

      PrintWriter pw_commits = new PrintWriter(new FileWriter("commits.csv"));
      PrintWriter pw_files = new PrintWriter(new FileWriter("files.csv"));

      pw_commits.write("hash;merge;author;date;desc");
      pw_files.write("hash;added;deleted;name;ext");

      for(Commit c: cl) {

        pw_commits.write("\n" + c.hash + ";" + c.merge + ";" + c.author + ";" + c.date + ";" + c.desc.replace("\n", "<cr>").replace(";", ","));

        for(File f: c.files) {
          pw_files.write("\n" + c.hash + ";" + f.added + ";" + f.deleted + ";" + f.name + ";" + f.ext);
        }

      }

      pw_commits.close();
      pw_files.close();



    } catch (Exception e) {
      e.printStackTrace();
    }




  }


}


class Commit {

  public String hash;
  public String merge;
  public String author;
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

  public int added;
  public int deleted;
  public String name;
  public String ext;

  public void setExt() {
    int indexOfExt = name.lastIndexOf('.');
    if(indexOfExt != -1) {
      ext = name.substring(indexOfExt);
    }
  }

}

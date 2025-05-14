import java.io.InputStreamReader;
import java.io.BufferedReader;

public class Ex2_1 {
  public static int[] ID;
  public static int[] SZ;

  public static void main(String[] args) throws Exception {
    if (args.length != 1) {
      throw new IllegalArgumentException("Expected N");
    }

    final int N = Integer.parseInt(args[0]);
    ID = new int[N];
    SZ = new int[N];
    for (int i = 0; i < N; i++) {
      ID[i] = i;
      SZ[i] = 1;
    }

    final BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
    String line;

    int unionCount = 0;
    final long started = System.currentTimeMillis();
    while ((line = reader.readLine()) != null) {
      String vals[] = line.split(" ");
      if (vals.length != 2) {
        throw new IllegalStateException("Expected two numbers!");
      }

      int p = Integer.parseInt(vals[0]);
      int q = Integer.parseInt(vals[1]);

      int i, j;
      for (i = p; i != ID[i]; i = ID[i]) {
        ID[i] = ID[ID[i]];
      }

      for (j = q; j != ID[j]; j = ID[j]) {
        ID[j] = ID[ID[j]];
      }

      if (i == j) {
        continue;
      }      
  
      if (SZ[i] < SZ[j]) {
        ID[i] = ID[j];
        SZ[j] += SZ[i];
        unionCount = Math.max(SZ[j], unionCount);
      } else {
        ID[j] = ID[i];
        SZ[i] += SZ[j];
        unionCount = Math.max(SZ[i], unionCount);
      } 

      if (unionCount == N - 1) {
        break;
      }
    }

    System.out.printf("Took: %d", System.currentTimeMillis() - started);
  }
}

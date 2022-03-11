/* grep programı varsayılan olarak büyük/küçük harf duyarlı olduğu için ve substringleri de okuduğu için bu programı da o şekilde tasarladım. */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define OUTPUTFILE "output.txt"
// function decleration
void grepf(FILE *inf, FILE *outf, char word[]);

int main(int argc, char *argv[])
{

    /* argüman hataları ve mesajları */
    if (argc == 1)
    {
        printf("Yeterli argüman sağlanmadı. Lütfen sırayla aranacak dosya adını ve kelimeyi girin. \n");
        return EXIT_FAILURE;
    }
    // help flagi
    else if (strcmp(argv[1], "--help") == 0)
    {
        printf("İlk argüman olarak arama yapılacak dosyanın adını veya dosya yolunu,\nikinci argüman olarak aranacak kelimeyi girin.\nProgram büyük/küçük harfe duyarlıdır. Örnek kullanım:\n./grepFile aranacakDosya.txt kelime \n");
        return EXIT_SUCCESS;
    }
    else if (argc <= 2)
    {
        printf("Yeterli argüman sağlanmadı. Lütfen sırayla aranacak dosya adını ve kelimeyi girin. \n");
        return EXIT_FAILURE;
    }
    else if (argc > 3)
    {
        printf("Çok fazla argüman yazıldı. Detaylı bilgi için\n./grepFile --help \nkomutunu çalıştırın.\n");
    }
    // argümanlar doğru girildiyse uygulama çalışacak
    else
    {
        /* dosyaları aç ve hazırla */
        // input dosyasını okuma modunda aç, açamazsa hata ver
        FILE *inf = fopen(argv[1], "r");
        if (!inf)
        {
            printf("Aranacak dosya bulunamadı. Dosya adını doğru yazdığınızdan ve doğru dizinde olduğunuzdan emin olun.\n");
            return EXIT_FAILURE;
        }

        // Output dosyasını (yoksa) oluştur ve yazma modunda aç, olmazsa hata ver
        FILE *outf = fopen(OUTPUTFILE, "w");
        if (!outf)
        {
            printf("Çıktı dosyası açılamadı / oluşturulamadı.\n");
            return EXIT_FAILURE;
        }

        // grepf fonksiyonunu çalıştır
        grepf(inf, outf, argv[2]);

        // Dosyaları kapat
        fclose(inf);
        fclose(outf);

        return EXIT_SUCCESS;
    }
}

// function definition
void grepf(FILE *inf, FILE *outf, char word[])
{
    // getline metodunun parametreleri
    char *lineBuffer = NULL;
    size_t lineBufferSize = 0;
    ssize_t currentLine;
    int lineNumber = 0;
    int wordCounter = 0;
    char space[] = " "; // kelime ayracı

    // Input dosyasının ilk satırı
    currentLine = getline(&lineBuffer, &lineBufferSize, inf);

    /* Dosya boyunca satır satır gezin. */
    while (currentLine >= 0) // dosya bitince getline  -1 döndürür
    {
        lineNumber++; // satır numaralarını 1'den başlatıyoruz
        // currentLine istenilen kelimeyi içeriyorsa bunu output dosyasına yaz
        if (strstr(lineBuffer, word))
        {
            fprintf(outf, "%d. satır\n", lineNumber); //çıktı dosyasına satır numarasını yaz
            // bu satırı kelime kelime böl ve bu kelimelerin 'word' ile eşleşip eşleşmediğini kontrol et
            char *token = strtok(lineBuffer, space);
            while (token != NULL)
            {
                if (strstr(token, word))
                {
                    wordCounter++;
                }

                token = strtok(NULL, space);
            }
        }

        // sonraki satıra geç
        currentLine = getline(&lineBuffer, &lineBufferSize, inf);
    }

    // dosya tamamen okunduktan sonra counter artmadıysa dosyaya bunu yaz
    if (wordCounter == 0)
    {
        fprintf(outf, "%s kelimesi bu dosyada mevcut değil.", word); // dosyaya yaz
        printf("%s kelimesi bu dosyada geçmiyor.\n", word);          // terminale yaz
    }
    else if (wordCounter >= 1)
    {
        printf("\"%s\" kelimesi bu dosyada %d kez geçiyor.\nDetayları output.txt dosyasında bulabilirsiniz. \n", word, wordCounter);

        free(lineBuffer);
        lineBuffer = NULL;
    }
}
/*readline() -> termianlde fonksiyondan girdi alan fonksiyondur. 
Normal read() veya fgets() den farkı:
     *Prompt gösterir.
	 *Ok tuşları ile geçmişe erişim sağlar.
	 *Satır düzenleme yapar.

Fonkisyon imzası:
    #include <readline/readline.h>

    char *readline(const char *prompt);

prompt      Ekranda gösterilecek yazı ("minishell> " gibi)
Döndürdüğü  Kullanıcının yazdığı satır (malloc'lanmış)
ctrl-D'de   NULL döndürür

readline() sana malloc ile ayrılmış bir string verir. Sen free() etmekle sorumlusun.

*/

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    char *line;

    while (1)
    {
        // Prompt göster ve kullanıcıdan girdi al
        line = readline("minishell> ");

        // ctrl-D basılırsa readline NULL döndürür
        if (line == NULL)
        {
			printf("\nçıkılıyor...\n");
			rl_clear_history();
            break ;
        }

		// "clear" yazılırsa ekranı temizle ve prompt'u yenile
        if (strcmp(line, "clear") == 0)
        {
            free(line);
            rl_on_new_line();  // readline'a: artık yeni satırdayız 
            rl_replace_line("", 0); // readline'a: satırı temizle
            rl_redisplay();         // readline'a: ekranı yenile
			printf("ekran temizlendi\n"); 
            continue ;
        }

        // Boş satır geldiyse (sadece Enter'a basıldıysa) history'e ekleme
        if (line[0] != '\0')
            add_history(line); // Yukarı ok ile geri çağırılabilir

        // Girdiyi işle (şimdilik sadece yazdır)
        printf("Girdiğin: [%s]\n", line);

        // readline malloc yaptığı için free etmek zorundasın!
        free(line);
    }
    return (0);
}

/*
add_history() fonksiyonu yukarıda kullanıldığı gibi önceki komudu getirir.
rl_clear_history() fonksiyonu ise geçmişi temizler. Ve free eder.

rl_on_new_line()    → "readline'a: artık yeni satırdayız"
rl_replace_line()   → "readline'a: satırı bununla değiştir"
rl_redisplay()      → "readline'a: ekranı yenile"
*/


/*
Şimdi de access, open, read, close fonksiyonlarını inceleyeceğim. redirection kısımlarında kullanılacaklar.

Bunları yapma sebeplerimiz: cat ile dosya okuma gibi işlemler yaparken dosyanın durumunu kontrol etmek,
gereksiz process oluşturmayı önlemek
güvenli doğru çıkışı yapabilmek 

->ilk olarak access()
prototipi => int access(const char *pathname, int mode);

	Bu dosyaya erişebilir miyim?" kontrolü yapar. Dosya var mı, izinler uygun mu gibi.
	mode yerine F_OK R_OK W_OK X_OK gibi makrolar kullanılır.
	Bunlar sırasıyla "var mı?", "okuma izni var mı?", "yazma izni var mı?", "çalıştırma izni var mı?" anlamına gelir.

access içinde girilecek PATH hakkında:
	ortam değişkenlerinin(envp) içindeki Path değişkeni içindeki klasörleridir. 
	Klasör içlerinde aranan komut var mı kontrol ederken kullanılır.

->open()
Prototipi => int open(const char *pathname, int flags, mode_t mode);
*/
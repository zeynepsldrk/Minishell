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
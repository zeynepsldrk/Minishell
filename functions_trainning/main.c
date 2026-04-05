/********* readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history *********/

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


/*
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

*/

/*
add_history() fonksiyonu yukarıda kullanıldığı gibi önceki komudu getirir.
rl_clear_history() fonksiyonu ise geçmişi temizler. Ve free eder.

rl_on_new_line()    → "readline'a: artık yeni satırdayız"
rl_replace_line()   → "readline'a: satırı bununla değiştir"
rl_redisplay()      → "readline'a: ekranı yenile"
*/


/********* access, open, read, close *********/

/*
Redirection kısımlarında kullanılacaklar.

Bunları yapma sebeplerimiz: cat ile dosya okuma gibi işlemler yaparken dosyanın durumunu kontrol etmek,
gereksiz process oluşturmayı önlemek
güvenli doğru çıkışı yapabilmek 

->access()
Prototipi => int access(const char *pathname, int mode);

	Bu dosyaya erişebilir miyim?" kontrolü yapar. Dosya var mı, izinler uygun mu gibi.
	mode yerine F_OK R_OK W_OK X_OK gibi makrolar kullanılır.
	Bunlar sırasıyla "var mı?", "okuma izni var mı?", "yazma izni var mı?", "çalıştırma izni var mı?" anlamına gelir.

access içinde girilecek PATH hakkında:
	ortam değişkenlerinin(envp) içindeki Path değişkeni içindeki klasörleridir. 
	Klasör içlerinde aranan komut var mı kontrol ederken kullanılır.

->open()
Prototipi => int open(const char *pathname, int flags, mode_t mode);

	open() fonksiyonu aslında redirectionlardan gelecek durumlara göre bazı davranışlaar yapar.
	dosya açar, yoksa oluşturur vs.

	* > (overwrite) : örneği ile ls > dosya.txt yani dosyayı yazmak için açarız.
	dosya yoksa oluştur varsa içini temizle ve yaz

	* >> (append) : örneği ile ls >> dosya.txt yani dosyanın sonuna eklemek yaparız.
	dosya yoksa oluştur varsa içindekileri bozmadan sonuna ekle

	* < (input) : örneği ile cat < dosya.txt yani dosyayı okuyacağız
	dosyayı okumalı dosya yoksa sistem bir şey yapmadan hata vermeli

	** O_CRATE kullanıdığı durumlarda 0644 mode izmi gereklidir

->read()
Prototipi => ssize_t read(int fd, void *buf, size_t count);

	<< (Here_doc) kullanımında EOF a kadar gnl deki gibi okuma yapmakta ve 
	okuduğu değerleri bir yerde biriktirip cat e göndermeyi sağlar.

->close()
Prototipi => int close(int fildes);

	open() ile açılan fd leri, pipe ile açılan fd leri kapatmak için kullanacağız.
	Zorunluluk yani kullanamazsak sıkıntı kardeşim


*/

/********* fork, wait, waitpid, wait3, wait4 *********/

/*
->fork()
Prototipi => pid_t fork(void);

	Amaç ana program minishellimiz çalşırken üzerinde komutlar çalıştırabilmek
	fork fonksiyonu çalışınca;
		-parent içinde child id sini döndürür (pozitif bir sayı).
		-child kendi içinde sıfır döndürür.
		-Yaniii dönen pid değeri pid == 0 ise child ve burada child pid i görmek için getpid() kullanılır
								 pid < 0 ise error
								 pid > 0 ise parent  ve buradaki pid child process id dir.

->wait()
Prototipi => pid_t wait(int *status);

	-fork ile child process oluşturduğumuzda ana processin hemen bitmesini
engellemek için bekleme sağlar. Ana processi bekletir.
	-Dönüş değeri : İşini bitiren çocuğun pid ini döner. Hiç child yoksa -1 döner.
	-wait() içinde aldığı status değişkeni : child in neden ve nasıl bittiği hakkında
bilgi verir. sinyalle mi, exit ile mi öldü? (sayı bit bazlı paketlenir)
			int status değeri ilk başta içinde ne olduğu önemli olmayan bir değişken
			içindeki adrese wait() child in neden bittiğine göre bir sayı yazar.
			status ün analmını anlayan makrolar kullanacağız. Bunlar :
				WIFEXITED: Eğer exit ile bittiyse.
				WEXITSTATUS: Exit durum kodu.
				WIFSIGNALED: Sinyalle bittiyse.
				WTERMSIG: Hangi sinyalle öldü?
	-wait() kullanmazsak child process zombie process olarak öylece kalır. Arka planda
boş yere yer kaplayan processler olur. İşletim sisteminin de belirli sayıda child
process açma hakkı olduğu için ileride sıkıntı oluşturur.
	-Eğer wait fonksiyonu başarısız olursa wait geriye -1 döner. status değikenine dokunmaz.
Bu yüzden kodun sağlıklı olması için status sıfır başlatılmalı.
	-wait(NULL) gibi bir şey yazılabilir. Bu hangi çocuğun öldüğünün önmli olmadığı
sadece child in bitmesini ister. Bu durumda işletim sistemi raporu yazacak bir yer bulamaz ve raporu çöpe atar. 
Rapordan kasıt nasıl sonladığı hakkında bilgiler. Ki bu bizim için önemli..
	-$? komutu için çıkış değerini basmada kullanacağız.


->waitpid()
Prototipi => pid_t waitpid(pid_t pid, int *status, int options);

	-wait() in gelişmiş versiyonu sadece belirli bir child process üzerinde kontrol sağlar.
	-fonksiyon içinde pid > 0 ise sadece o pid e ait child ı bekle.
					  pid == -1 ise herhangi bri child bekle aynı wait() gibi.
					  pid == 0 ise çağırılan süreçle aynı grup id sine ait herhangi bir child bekle.
					  pid < -1 ise grup id si bu değerin mutlak değerine eşit olan herhangi bir child bekle.
	-status aynı wait() deki gibi
	-options değişkeni de child işini bitirnceey kadar ana process ne yapacak?
		değeri sıfır ise child bitinceye kadar hiçbir şey yapma.
			   WNOHANG ise ana process child a kısa bir bakar ve işini yapmaya devam eder.
			   WUNTRACED ise ana process hiçbir şey yapmadan bekler. Child sadece ölünce değil 
			duraksadığında da ana process uyanır.
	-return değeri > 0 ise işini bitiren child in gerçek id sidir. 
				   == 0 ise sadece WNOHANG kullandıysan olur. Child hala çalışıyor.
				   == -1 ise bir hata oldu


->wait3 ve wait4 fonksiyonları performans hakkında değerlendirmeiçin kullanıldığından
minishell projesinde kullanılmaya gerek yoktur.

*/


/********* getcwd, chdir, getenv *********/

/*

->getcwd()
Prototipi => char *getcwd(char *buf, size_t size);

    -Bir program çalıştığında program bir konum üzerindedir. Bu konumu veren fonk.
    -*buf: dizinin yolunun yazıldığı dizi 
    -*size: dizinin boyutu
    -getcwd() fonk. pwd komutunun çalışması için kullanacağız. Ya da cd ile dizinin değiştirdiğimiz
    yeni konumu alırken de kullanılır.

    !!Attention!!
    -getcwd(NULL, 0); durumunda içeride malloc açılır. Free yapılması gerekir. Kendi buf'ını stack de
    tanımlarsak free gerekli değil. 
    -Mevcut dizi bir şekilde silindi. getcwd() fonk. hata veriri. Bu durumda shell çökmemeli, eğer konumu okuma yetkisi
    elinden alınırsa fonk. hata döner.
    Yanii getcwd() fonk. başarısız olursa NULL döner. Başarısız olma durumları:
        -Eğer buf NULL değilse ve size sıfır ise.
        -Eğer buf NULL değilse ve size çok küçükse.
        -Eğer buf NULL ise ve malloc başarısız olursa.
        -Eğer mevcut dizinin konumunu okuma yetkisi elinden alınırsa.
    -getcwd() fonk. başarısız olursa errno değişkeni şu değerleri alır:
        -ERANGE: Eğer buf NULL değilse ve size sıfır ise veya buf NULL değilse ve size çok küçükse.
        -ENOMEM: Eğer buf NULL ise ve malloc başarısız olursa.
        -EACCES: Eğer mevcut dizinin konumunu okuma yetkisi elinden alınırsa.
    NULL döndüğü durumlarda errno'ya bakarak hatanın ne olduğunu anlayabiliriz. perror() ile
    o kısmı atla programçalışmaya devam etmeli.


->chdir()
Prototipi => int chdir(const char *path);

    -chdir() fonk. cd komutunun çalışması için kullanacağız.
    -*path: gitmek istediğimiz dizinin yolu. İşlem başarılı olursa 0 döner, başarısız olursa -1 döner.
    

	



*/


/********* stat, lstat, fstat, unlink, opendir, readdir, closedir *********/

/********* dup, dup2, pipe *********/

/********* pipe, dup, dup2, execve *********/

/********* strerror, perror, isatty, ttyname, ttyslot *********/

/********* ioctl, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs *********/

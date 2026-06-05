#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Kendi projenin token enum/tip yapılarını buraya uyarla
typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_REDIRECT_IN,   // <
    TOKEN_REDIRECT_OUT,  // >
    TOKEN_HERE_DOC,      // <<
    TOKEN_APPEND         // >>
} t_type;

typedef struct s_token {
    char            *str;
    t_type          type;
    struct s_token  *next;
} t_token;

// --- TEST EDİLECEK FONKSİYON PROTOTİPİ ---
// (Kendi fonksiyon prototipini buraya ekle)
char **get_argv(t_token *token)
{
    char **argv;
    int i;

    argv = malloc(sizeof(char *) * (word_count(token) + 1));
    i = 0;
    if (!argv)
        return NULL;
    while(token != NULL && token->type != PIPE)
    {
        if (token->type == WORD)
        {
            argv[i] = ft_strdup(token->context);
            i++;
        }
        if(token->type == REDIRECT_IN || token->type == REDIRECT_OUT || token->type == HEREDOC || token->type == APPEND)
        {
            if(token->next == NULL) // eger redirect'in targeti yoksa donguden cik
                break; 
            token = token->next; // redir sonrasi target word'u saymayalim diye
        }
        token = token->next;
    }
    argv[i] = NULL;
    return (argv);
}

// --- YARDIMCI TEST FONKSİYONLARI ---
// Kolayca token listesi oluşturabilmek için yardımcı fonksiyon
t_token *create_token(char *str, t_type type)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->str = strdup(str);
    new->type = type;
    new->next = NULL;
    return (new);
}

// Test bittikten sonra belleği temizlemek için
void free_tokens(t_token *head)
{
    t_token *tmp;
    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->str);
        free(tmp);
    }
}

// --- MAIN FUNCTION ---
int main(void)
{
    // 1. Test Verisini Hazırlama: [cat] -> [<] -> [input.txt] -> [-n]
    t_token *head = create_token("cat", TOKEN_WORD);
    head->next = create_token("<", TOKEN_REDIRECT_IN);
    head->next->next = create_token("input.txt", TOKEN_WORD); // veya redirect argümanı tipi
    head->next->next->next = create_token("-n", TOKEN_WORD);

    printf("=== MINISHELL TEST: get_argv ===\n");
    printf("Girdi Tokenlari: [cat] [<] [input.txt] [-n]\n");
    printf("Beklenen Cikti: [\"cat\", \"-n\", NULL]\n");
    printf("---------------------------------\n");

    // 2. Senin fonksiyonunu çağırma
    char **result_argv = get_argv(head);

    // 3. Sonuçları Ekrana Yazdırma
    if (!result_argv)
    {
        printf("Hata: get_argv NULL döndürdü!\n");
    }
    else
    {
        printf("Senin Fonksiyonunun Ciktisi:\n");
        int i = 0;
        while (result_argv[i] != NULL)
        {
            printf("argv[%d]: \"%s\"\n", i, result_argv[i]);
            i++;
        }
        printf("argv[%d]: NULL\n", i);
        
        // Bellek sızıntısı olmaması için get_argv'nin döndürdüğü diziyi free'liyoruz
        // Not: Eğer get_argv tokenlerin içindeki str'leri kopyalamadan (strdup yapmadan) 
        // direkt işaret ediyorsa, sadece dizinin kendisini free etmek yeterlidir.
        free(result_argv);
    }

    // Temizlik
    free_tokens(head);
    return (0);
}
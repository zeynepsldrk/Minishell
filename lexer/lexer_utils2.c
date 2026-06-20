#include "minishell.h"

//eger "$HOME"asay gibi expand sonrasi bosluksuz bisi gelirse bunların ayrilmasi gerek
//ama normalde bosluk gorunce whitspace_tkn buffer'i sifirlayip yeni tkn baslatiyo
//bu edge case icin ayrica buffer'i temizlemem gerekiyo.
//tamam ben $HOME ve asay tokenlarını ayırıyorum ama bash arasi bosluk olmayan tokenlari bir sayiyo
//o yuzden ayirdiklarimizi birlestirmemiz gerekicek
void general_quote_handler(t_lexer *ptr, char *str)
{
    quote_tkn(ptr, str);
    //ptr->in_single = 0;
    //ptr->in_double = 0; //garbage değerleri once temziliyoruz ki her token icin expaand değeri dogru setlensin.
    // daha once en sondaydı ki her islem sonunda bir sonraki icin temiz olsun ama bu sefer de ilk ayarlanırken yanlıs olabiliodu. o yzuden en basa alinmali
    //bash burada iki artarda WORD gelen token'lar boşlukla veya ppipe vb. ile ayrilmiyosa birlesik kaabul etmis
    //bu sebeple kosullu calistirip, buffer temizliyoruz.
    if (!str[ptr->i] || str[ptr->i] == 32 || str[ptr->i] == '|'
        || str[ptr->i] == '<' || str[ptr->i] == '>')
    {
        get_token_helper(ptr); //eger iki WORD tokenı birlesi olmayacaksa yeni token eklensin die bu da kosullu calimaliii
        ptr->j = 0;
        ft_memset(ptr->buff, 0, ft_strlen(ptr->buff) + 1);
        ptr->has_quote = 0;
        ptr->j = 0;
    }
    else
    {
        get_token_helper(ptr);
        ptr->tail->is_joined = 1;
        ptr->j = 0;
        ft_memset(ptr->buff, 0, ft_strlen(ptr->buff) + 1);
        ptr->has_quote = 0;
    }
    ptr->in_single = 0;
    ptr->in_double = 0;
    //    printf("quote handler: %s\n", ptr->tail->context);
}

void join_tokens(t_shell *shell)
{
    t_token *curr;
    char    *joined;
    t_token *next_tmp;

    curr = shell->tokens;
    while(curr != NULL && curr->next != NULL)
    {
        if(curr->type == WORD && curr->next->type == WORD && curr->is_joined)
        {
            joined = ft_join_and_free(curr->context, curr->next->context);
            //join'den sonra free gerekir mi zeynebe sor
            curr->context = joined;
            next_tmp = curr->next;
            curr->next = curr->next->next;
            free(next_tmp->context);
            free(next_tmp);
        }
        else
            curr = curr->next;
    }
}

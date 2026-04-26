/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 20:15:57 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/25 20:16:08 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_key_inside(char *key, t_env_node *env_list)
{
    int i;

    i = 0;
    while (env_list)
    {
        if (str_cmp(env_list->key, key) == 0)
            return (1);
        env_list = env_list->next;
    }
    return (0);
}

char *find_key_or_value(char *arg, int i, int which_one)
{
    char *result;

    if (which_one == 1)
    {
        while (arg[i] && arg[i] != '=')
        {
            result[i] = arg[i];
            i++;
        }
        result[i] = '\0';
        return (result);
    }
    if (which_one == 2)
    {
            while (arg[i])
    {
        if (arg[i] == '=')
        {
            result = str_dup(arg + i + 1); //= işaretinden sonraki kısmı value olarak alıyoruz
            return (result);
        }
        i++;
    }
    }
    return (NULL);
}

int is_valid_for_export(char **args, int i, int j)
{
    while (args[i])
    {
        j = 0;
        if (!(ft_isalpha(args[i][0]) || args[i][0] == '_'))
            return (write(2, "export: not a valid identifier\n", 31), 1);
        else
        {
            while (args[i][j] && args[i][j] != '=')
            {
                if (!(ft_isalnum(args[i][j]) || args[i][j] == '_'))
                    return (write(2, "export: not a valid identifier\n", 31), 1);
                j++;
            }
        }
        i++;
    }
    return (0);
}

int	builtin_export(t_shell *shell)
{
    int i;
    char *key;
    char *value;

    i = 1;
    if (!is_valid_for_export(shell->cmds->argv, 1, 0)) //key olması için bazı kuralları kontrol ettim
        return (1);
    while (shell->cmds->argv[i])
    {
        key = find_key_or_value(shell->cmds->argv[i], 0, KEY); //key değerini = den ayırmak için 
        value = find_key_or_value(shell->cmds->argv[i], 0, VALUE); //value değerini = den ayırmak için
        if (!key)
            return (1);
        update_env_node(key, value, shell); //key zaten varsa değerini güncelliyor
        if (!is_key_inside(key, shell->env_list)) //eğer o key değeri hiç yoksa yeni bir node oluşturup onu ekliyoruz
            create_new_node(shell, key, value);
        i++;
    }
    return (0);
}

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

void handle_append(t_shell *shell, char *key, char *new_val)
{
    t_env_node  *node;
    char        *joined;

    node = shell->env_list;
    while (node)
    {
        if (ft_strcmp(node->key, key) == 0)
        {
            if (node->value && new_val)
            {
                joined = ft_strjoin(node->value, new_val); // mevcut + yeni
                free(node->value);
                node->value = joined;
            }
            else if (new_val)
                node->value = ft_strdup(new_val);
            return ;
        }
        node = node->next;
    }
    create_new_node(shell, key, new_val);
}

void execute_export(t_shell *shell, char *key, char *value, int i)
{
    if (is_append(shell->cmds->argv[i]))
        handle_append(shell, key, value);   // += durumu
    else
    {
        if (is_key_inside(key, shell->env_list))
            update_env_node(key, value, shell);
        else
            create_new_node(shell, key, value);
    }
}

int is_valid_for_export(char **args, int i, int j)
{
    while (args[i])
    {
        j = 0;
        if (!(ft_isalpha(args[i][0]) || args[i][0] == '_'))
            return (write(2, "export: not a valid identifier\n", 31), 1);
        while (args[i][j] && args[i][j] != '=')
        {
            if (args[i][j] == '+' && args[i][j + 1] == '=')
                break ; // += durumu geçerlidir, döngüden çık
            if (!(ft_isalnum(args[i][j]) || args[i][j] == '_'))
                return (write(2, "export: not a valid identifier\n", 31), 1);
            j++;
        }
        i++;
    }
    return (0);
}

void only_export_command(t_env_node *env_list)
{
    ft_bubble_sort(&env_list, 1);
    while (env_list)
    {
        printf("declare -x %s", env_list->key);
        if (env_list->value) //env de görünmeyen value değerleri NULL olan değişkenleri de export ile yazdırabilmek için(= olmadan yazılıyorlar)
            printf("=\"%s\"", env_list->value); //value tırnak içinde yazılmalı
        printf("\n");
        env_list = env_list->next;
    }
}

int	builtin_export(t_shell *shell)
{
    int i;
    char *key;
    char *value;

    i = 1;
    if (!shell->cmds->argv[1])
    {
        only_export_command(shell->env_list); //eğer export komutundan sonra hiç argüman gelmezse env_list'i yazdırıyoruz
        return (0);
    }
    if (is_valid_for_export(shell->cmds->argv, 1, 0)) //key olması için bazı kuralları kontrol ettim
        return (1);
    while (shell->cmds->argv[i])
    {
        key = find_key_or_value(shell->cmds->argv[i], 0, KEY); //key değerini = den ayırmak için 
        value = find_key_or_value(shell->cmds->argv[i], 0, VALUE); //value değerini = den ayırmak için
        if (!key)
            return (1);
        execute_export(shell, key, value, i);
        i++;
    }
    return (0);
}

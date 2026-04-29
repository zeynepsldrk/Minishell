/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bubblesort.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zedurak <zedurak@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 18:23:30 by zedurak           #+#    #+#             */
/*   Updated: 2026/04/29 18:23:31 by zedurak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void swap(t_env_node *temp, int *swapped)
{
    char *temp_key;
    char *temp_value;

    temp_key = temp->key;
    temp->key = temp->next->key;
    temp->next->key = temp_key;
    temp_value = temp->value;
    temp->value = temp->next->value;
    temp->next->value = temp_value;
    *swapped = 1;
}

void bubble_sort(t_env_node **env_list, int swapped)
{
    t_env_node *temp;

    if (!env_list || !*env_list)
        return ;
    while (swapped)
    {
        swapped = 0;
        temp = *env_list;
        while (temp && temp->next)
        {
            if (ft_strcmp(temp->key, temp->next->key) > 0)
                swap(temp, &swapped);
            temp = temp->next;
        }
    }
}

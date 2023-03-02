/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mani_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmassarw <mmassarw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 12:26:50 by mmassarw          #+#    #+#             */
/*   Updated: 2023/02/05 20:43:03 by mmassarw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	pop_node(t_token **list, t_token *node)
{
	if (*list == NULL || node == NULL)
		return ;
	if (node->prev != NULL)
		node->prev->next = node->next;
	if (node->next != NULL)
		node->next->prev = node->prev;
	if (*list == node)
		*list = node->next;
	node->prev = NULL;
	node->next = NULL;
	ft_free_ltoken(node);
}

void	add_node_middle(t_token *node_before, t_token *new_node)
{
	if (node_before->next)
	{
		node_before->next->prev = new_node;
		new_node->next = node_before->next;
	}
	node_before->next = new_node;
	new_node->prev = node_before;
}

void	join_same_type_nodes(t_token *head)
{
	t_token	*temp;
	char	*new_content;

	temp = head;
	while (temp && temp->next)
	{
		if (temp->type == temp->next->type || ((temp->type != SPACES && \
		temp->type != REDIRECTION && temp->type != PIPE) && \
		(temp->next->type == SINGLE || temp->next->type == DOUBLE)) \
		|| ((temp->type == SINGLE || temp->type == DOUBLE) && \
		(temp->next->type != SPACES && temp->next->type != REDIRECTION && \
		temp->next->type != PIPE)))
		{
			new_content = (char *) ft_realloc(temp->content, \
			ft_strlen(temp->content) + ft_strlen(temp->next->content) + 1, \
			ft_strlen(temp->content));
			ft_strcat(new_content, temp->next->content);
			temp->content = new_content;
			pop_node(&head, temp->next);
		}
		else
			temp = temp->next;
	}
}

void	delete_empty_nodes(t_token **head)
{
	t_token	*temp;
	t_token	*to_delete;

	temp = *head;
	while (temp)
	{
		if ((temp->type == SPACES || temp->content[0] == '\0') && \
		(temp->type != SINGLE && temp->type != DOUBLE))
		{
			if (temp->prev)
				temp->prev->next = temp->next;
			if (temp->next)
				temp->next->prev = temp->prev;
			if (*head == temp)
				*head = temp->next;
			to_delete = temp;
			temp = temp->next;
			to_delete->content = (char *) ft_free(to_delete->content);
			to_delete = (t_token *) ft_free(to_delete);
		}
		else
			temp = temp->next;
	}
}

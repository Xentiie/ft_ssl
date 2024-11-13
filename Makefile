# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/15 17:26:47 by reclaire          #+#    #+#              #
#    Updated: 2024/11/08 14:58:57 by reclaire         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include config.mk

$(NAME):	packages $(OBJS)
			$(CC)  $(INCLUDES) $(LIBS_PATHS) $(OBJS) $(LIBS) -o $(NAME)

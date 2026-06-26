/*
 *  Project  -  cmd
 *  Date     -  June 15th 2026
 *
 *  Copyright (c) 2026 Jules Nourdin & Léo Lacordaire
 */

use mio::Token;
use crate::utils::{Server, send_response, Direction};

pub fn cmd_forward(token: Token, server: &mut Server)
{
    let (old_x, old_y, new_x, new_y) = {
        let client = server.clients.get(&token).unwrap();
        let player = client.player.as_ref().unwrap();
        let (ox, oy) = (player.x, player.y);
        let (nx, ny) = match player.direction {
            Direction::N => (ox, if oy == 0 { server.params.height - 1 } else { oy - 1 }),
            Direction::S => (ox, (oy + 1) % server.params.height),
            Direction::E => ((ox + 1) % server.params.width, oy),
            Direction::W => (if ox == 0 { server.params.width - 1 } else { ox - 1 }, oy),
        };
        (ox, oy, nx, ny)
    };

    // change le token pour le nouveau
    server.world.tiles[old_y as usize][old_x as usize].players.retain(|&t| t != token);
    server.world.tiles[new_y as usize][new_x as usize].players.push(token);

    let client = server.clients.get_mut(&token).unwrap();
    let player = client.player.as_mut().unwrap();
    player.x = new_x;
    player.y = new_y;

    let _ = send_response(&mut client.stream, "ok\n");
}

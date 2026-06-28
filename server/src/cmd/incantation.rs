/*
 *  Project  -  cmd
 *  Date     -  June 15th 2026
 *
 *  Copyright (c) 2026 Jules Nourdin
 */

use mio::Token;
use crate::utils::{send_response, Player, Server, notify_gui};
use std::collections::HashMap;

struct IncantationRequirements {
    players: usize,
    resources: HashMap<String, u32>,
}

fn get_requirements(level: u32) -> Option<IncantationRequirements> {
    let mut resources = HashMap::new();
    let (players, reqs) = match level {
        1 => (1, vec![("linemate", 1)]),
        2 => (2, vec![("linemate", 1), ("deraumere", 1), ("sibur", 1)]),
        3 => (2, vec![("linemate", 2), ("sibur", 1), ("phiras", 2)]),
        4 => (4, vec![("linemate", 1), ("deraumere", 1), ("sibur", 2), ("phiras", 1)]),
        5 => (4, vec![("linemate", 1), ("deraumere", 2), ("sibur", 1), ("mendiane", 3)]),
        6 => (6, vec![("linemate", 1), ("deraumere", 2), ("sibur", 3), ("phiras", 1)]),
        7 => (6, vec![("linemate", 2), ("deraumere", 2), ("sibur", 2), ("mendiane", 2), ("phiras", 2), ("thystame", 1)]),
        _ => return None,
    };

    for (name, count) in reqs {
        resources.insert(name.to_string(), count);
    }

    Some(IncantationRequirements{players, resources})
}

fn check_incantation(server: &Server, player: &Player) -> bool {
    let requirements = match get_requirements(player.level) {
        Some(req) => req,
        None => return false,
    };

    let tile = &server.world.tiles[player.y as usize][player.x as usize];

    for (resource, required_count) in &requirements.resources {
        if tile.resources.get(resource).cloned().unwrap_or(0) < *required_count {
            return false;
        }
    }

    let players_on_tile: Vec<_> = tile
        .players
        .iter()
        .filter_map(|token| {
            server
                .clients
                .get(token)
                .and_then(|c| c.player.as_ref())
        })
        .collect();

    let mut same_level_players = 0;
    for p in players_on_tile {
        if p.level == player.level {
            same_level_players += 1;
        }
    }

    same_level_players >= requirements.players
}

fn incantation_failed(token: Token, server: &mut Server, player: &Player)
{
    if let Some(client) = server.clients.get_mut(&token) {
        send_response(&mut client.stream, "ko\n").unwrap_or_default();
    }
    notify_gui(&mut server.clients, &format!("pie {} {} 0\n", player.x, player.y));
}

fn get_participants(server: &Server, player: &Player) -> Vec<u32>
{
    server.world.tiles[player.y as usize][player.x as usize].players.iter().filter(|&&t| {
            server.clients.get(&t).and_then(|c| c.player.as_ref()).map(|p| p.level == player.level).unwrap_or(false)
        }).map(|t| t.0 as u32).collect()
}

fn build_pic_message(player: &Player, participants: &[u32]) -> String
{
    let mut pic = format!("pic {} {} {}", player.x, player.y, player.level);
    for n in participants {
        pic.push_str(&format!(" #{}", n));
    }
    pic.push('\n');
    pic
}

fn start_incantation(token: Token, server: &mut Server, player: &Player)
{
    let participants = get_participants(server, player);
    let pic = build_pic_message(player, &participants);
    notify_gui(&mut server.clients, &pic);

    if let Some(client) = server.clients.get_mut(&token) {
        let response = format!("Elevation underway\nCurrent level: {}\n", player.level + 1);
        send_response(&mut client.stream, &response).unwrap_or_default();
    }

    notify_gui(&mut server.clients, &format!("pie {} {} 1\n", player.x, player.y));
}

pub fn cmd_incantation(token: Token, server: &mut Server)
{
    let player = match server.clients.get(&token).and_then(|c| c.player.clone()) {
        Some(p) => p,
        None => {
            if let Some(client) = server.clients.get_mut(&token) {
                send_response(&mut client.stream, "ko\n").unwrap_or_default();
            }
            return;
        }
    };

    if !check_incantation(server, &player) {
        incantation_failed(token, server, &player);
        return;
    }
    start_incantation(token, server, &player);
}

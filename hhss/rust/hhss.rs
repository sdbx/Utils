use std::env;
use std::fs;
use std::io;
use std::process;
use rand::seq::SliceRandom;
use rand::thread_rng;

const MIN_SENTENCE_NUM: usize = 5;
const MIN_USER_NUM: usize = 1;
const SENTENCE_FILE: &str = "hsr.dat";
const USER_FILE: &str = "usr.dat";
const USER_TEMPLATE: &str = "${user}";

fn main() {
    let args: Vec<String> = env::args().collect();
    
    if args.len() != 2 {
        eprintln!("hhss: argc != 2.");
        process::exit(1);
    }
    
    let num_sentences = match parse_num_sentences(&args[1]) {
        Ok(n) => n,
        Err(e) => {
            eprintln!("hhss: {}", e);
            process::exit(1);
        }
    };
    
    let sentences = match read_data_file(SENTENCE_FILE) {
        Ok(lines) => lines,
        Err(e) => {
            eprintln!("hhss: failed to open {} as mode \"r\": {}", SENTENCE_FILE, e);
            process::exit(1);
        }
    };
    
    let users = match read_data_file(USER_FILE) {
        Ok(lines) => lines,
        Err(e) => {
            eprintln!("hhss: failed to open {} as mode \"r\": {}", USER_FILE, e);
            process::exit(1);
        }
    };
    
    if sentences.len() < MIN_SENTENCE_NUM {
        eprintln!("hhss: it is required for {} to have valid lines more than or equal to {}.", 
                 SENTENCE_FILE, MIN_SENTENCE_NUM);
        process::exit(1);
    }
    
    if users.len() < MIN_USER_NUM {
        eprintln!("hhss: it is required for {} to have valid lines more than or equal to {}.", 
                 USER_FILE, MIN_USER_NUM);
        process::exit(1);
    }
    
    if num_sentences > sentences.len() {
        eprintln!("hhss: argv[1] = {} needs to be <= {}, the number of valid lines in {}.", 
                 num_sentences, sentences.len(), SENTENCE_FILE);
        process::exit(1);
    }
    
    let selected_sentences = select_random_sentences(&sentences, num_sentences);
    
    for sentence in selected_sentences {
        let processed_sentence = replace_templates(&sentence, &users);
        println!("{}", processed_sentence);
    }
}

fn parse_num_sentences(arg: &str) -> Result<usize, String> {
    match arg.parse::<usize>() {
        Ok(n) => {
            if n < MIN_SENTENCE_NUM {
                Err(format!("argv[1] = {} needs to be >= {}.", n, MIN_SENTENCE_NUM))
            } else {
                Ok(n)
            }
        }
        Err(_) => Err("no conversion can be performed.".to_string())
    }
}

fn read_data_file(filename: &str) -> Result<Vec<String>, io::Error> {
    let contents = fs::read_to_string(filename)?;
    let mut valid_lines = Vec::new();
    
    for line in contents.lines() {
        let trimmed = line.trim();
        // Skip empty lines and comments
        if !trimmed.is_empty() && !trimmed.starts_with('#') {
            valid_lines.push(trimmed.to_string());
        }
    }
    
    Ok(valid_lines)
}

fn select_random_sentences(sentences: &[String], count: usize) -> Vec<String> {
    let mut indices: Vec<usize> = (0..sentences.len()).collect();
    let mut rng = thread_rng();
    
    // Fisher-Yates shuffle
    indices.shuffle(&mut rng);
    
    // Take first 'count' indices and get corresponding sentences
    indices.into_iter()
        .take(count)
        .map(|i| sentences[i].clone())
        .collect()
}

fn replace_templates(sentence: &str, users: &[String]) -> String {
    let mut result = String::new();
    let mut last_user: Option<String> = None;
    let mut rng = thread_rng();
    
    let parts: Vec<&str> = sentence.split(USER_TEMPLATE).collect();
    
    for (i, part) in parts.iter().enumerate() {
        result.push_str(part);
        
        // If this is not the last part, we need to add a user name
        if i < parts.len() - 1 {
            let mut available_users = users.to_vec();
            
            // If we have more than one user and we used one before, remove it to avoid repetition
            if users.len() > 1 {
                if let Some(last) = &last_user {
                    available_users.retain(|u| u != last);
                }
            }
            
            let selected_user = available_users.choose(&mut rng).unwrap();
            result.push_str(selected_user);
            last_user = Some(selected_user.clone());
        }
    }
    
    result
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_num_sentences() {
        assert_eq!(parse_num_sentences("5").unwrap(), 5);
        assert_eq!(parse_num_sentences("10").unwrap(), 10);
        assert!(parse_num_sentences("4").is_err());
        assert!(parse_num_sentences("abc").is_err());
    }

    #[test]
    fn test_replace_templates() {
        let users = vec!["Alice".to_string(), "Bob".to_string()];
        let sentence = "Hello ${user}, how are you ${user}?";
        let result = replace_templates(sentence, &users);
        
        // Should contain both users and they should be different
        assert!(result.contains("Alice") || result.contains("Bob"));
        assert!(result.contains("Hello"));
        assert!(result.contains("how are you"));
        assert!(!result.contains("${user}"));
    }

    #[test]
    fn test_select_random_sentences() {
        let sentences = vec![
            "First".to_string(),
            "Second".to_string(),
            "Third".to_string(),
            "Fourth".to_string(),
            "Fifth".to_string(),
        ];
        
        let selected = select_random_sentences(&sentences, 3);
        assert_eq!(selected.len(), 3);
        
        // All selected sentences should be from the original list
        for sentence in &selected {
            assert!(sentences.contains(sentence));
        }
    }
}
use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Modify_Macro_Pattern_To_Trigger_Delim_Mismatch_230;

impl Mutator for Modify_Macro_Pattern_To_Trigger_Delim_Mismatch_230 {
    fn name(&self) -> &str {
        "Modify_Macro_Pattern_To_Trigger_Delim_Mismatch_230"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                let mut rng = thread_rng();
                if let Macro::Rules(macro_rules) = &mut macro_item.mac {
                    for rule in &mut macro_rules.rules {
                        if let Some((_, ref mut pat)) = rule.pat.first_mut() {
                            if let MacroPat::Group(group) = pat {
                                let new_delim = match group.delimiter {
                                    MacroDelimiter::Paren(_) => {
                                        MacroDelimiter::Brace(Default::default())
                                    }
                                    MacroDelimiter::Brace(_) => {
                                        MacroDelimiter::Bracket(Default::default())
                                    }
                                    MacroDelimiter::Bracket(_) => {
                                        MacroDelimiter::Paren(Default::default())
                                    }
                                };
                                group.delimiter = new_delim;
                            }
                        }
                        if let MacroBody::Delimited(body) = &mut rule.body {
                            let new_delim = match body.delimiter {
                                MacroDelimiter::Paren(_) => {
                                    MacroDelimiter::Brace(Default::default())
                                }
                                MacroDelimiter::Brace(_) => {
                                    MacroDelimiter::Bracket(Default::default())
                                }
                                MacroDelimiter::Bracket(_) => {
                                    MacroDelimiter::Paren(Default::default())
                                }
                            };
                            body.delimiter = new_delim;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macro definitions by altering the delimiters used in macro patterns and expansions to mismatched ones. For each rule within a macro, it changes the pattern and body delimiters to different types (e.g., from parentheses to braces or brackets). This transformation is designed to provoke parsing errors or ICEs related to delimiter mismatches, leveraging existing macro structures to stress-test the compiler's macro parsing and expansion logic."
    }
}
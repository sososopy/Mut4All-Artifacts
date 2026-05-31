use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
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

pub struct Insert_Mismatched_Delimiter_Pairs_In_Macro_Expansion_230;

impl Mutator for Insert_Mismatched_Delimiter_Pairs_In_Macro_Expansion_230 {
    fn name(&self) -> &str {
        "Insert_Mismatched_Delimiter_Pairs_In_Macro_Expansion_230"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let delimiters = [('{', '}'), ('[', ']'), ('(', ')')];
        let mut candidates = Vec::new();
        
        struct MacroRuleVisitor<'a> {
            candidates: &'a mut Vec<(usize, usize, &'a mut TokenTree, &'a mut TokenTree)>,
            rule_index: usize,
        }
        
        impl<'a> VisitMut for MacroRuleVisitor<'a> {
            fn visit_item_macro_mut(&mut self, i: &mut ItemMacro) {
                if let Some(mac) = &mut i.mac {
                    self.visit_macro_mut(mac);
                }
            }
            
            fn visit_macro_mut(&mut self, mac: &mut Macro) {
                let mut rule_index = 0;
                for rule in &mut mac.rules {
                    self.rule_index = rule_index;
                    self.visit_macro_rule_mut(rule);
                    rule_index += 1;
                }
            }
            
            fn visit_macro_rule_mut(&mut self, rule: &mut MacroRule) {
                let mut token_index = 0;
                let mut prev_tt: Option<&mut TokenTree> = None;
                let mut tokens = std::mem::take(&mut rule.body.tokens);
                let mut new_tokens = Vec::new();
                
                for tt in &mut tokens {
                    if let TokenTree::Group(g) = tt {
                        let mut found_metavar = false;
                        let mut inner_tokens = std::mem::take(&mut g.stream());
                        let mut new_inner = Vec::new();
                        
                        for inner_tt in &mut inner_tokens {
                            if let TokenTree::Ident(ident) = inner_tt {
                                if ident.to_string().starts_with('$') {
                                    found_metavar = true;
                                }
                            }
                            new_inner.push(std::mem::replace(inner_tt, TokenTree::Ident(Ident::new("", Span::call_site()))));
                        }
                        
                        if found_metavar && !new_inner.is_empty() {
                            let stream = proc_macro2::TokenStream::from_iter(new_inner.into_iter());
                            let mut new_group = Group::new(g.delimiter(), stream);
                            self.candidates.push((self.rule_index, token_index, tt, &mut TokenTree::Group(new_group)));
                        } else {
                            let stream = proc_macro2::TokenStream::from_iter(new_inner.into_iter());
                            *g = Group::new(g.delimiter(), stream);
                        }
                    }
                    
                    if let Some(prev) = prev_tt {
                        if let TokenTree::Ident(ident) = tt {
                            if ident.to_string().starts_with('$') {
                                if let TokenTree::Punct(p) = prev {
                                    if p.as_char() == ':' {
                                        if let Some(pprev) = new_tokens.get(new_tokens.len().wrapping_sub(2)) {
                                            if let TokenTree::Ident(prev_ident) = pprev {
                                                if prev_ident.to_string().starts_with('$') {
                                                    self.candidates.push((self.rule_index, token_index.wrapping_sub(2), &mut new_tokens[new_tokens.len().wrapping_sub(2)], tt));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    new_tokens.push(std::mem::replace(tt, TokenTree::Ident(Ident::new("", Span::call_site()))));
                    prev_tt = new_tokens.last_mut();
                    token_index += 1;
                }
                
                rule.body.tokens = proc_macro2::TokenStream::from_iter(new_tokens.into_iter());
            }
        }
        
        let mut visitor = MacroRuleVisitor {
            candidates: &mut candidates,
            rule_index: 0,
        };
        visitor.visit_file_mut(file);
        
        if let Some((_, _, target_tt, metavar_tt)) = candidates.choose_mut(&mut rng) {
            if let TokenTree::Group(g) = target_tt {
                let delimiter = g.delimiter();
                let mut stream = g.stream().into_iter().collect::<Vec<_>>();
                
                for (i, tt) in stream.iter_mut().enumerate() {
                    if let TokenTree::Ident(ident) = tt {
                        if ident.to_string().starts_with('$') {
                            let opening_delim = delimiters.choose(&mut rng).unwrap();
                            let closing_delim = delimiters.choose(&mut rng).unwrap();
                            
                            while closing_delim.0 == opening_delim.0 {
                                let closing_delim = delimiters.choose(&mut rng).unwrap();
                            }
                            
                            let opening_char = opening_delim.0;
                            let closing_char = closing_delim.1;
                            
                            let new_opening = match opening_char {
                                '{' => TokenTree::Group(Group::new(Delimiter::Brace, proc_macro2::TokenStream::new())),
                                '[' => TokenTree::Group(Group::new(Delimiter::Bracket, proc_macro2::TokenStream::new())),
                                '(' => TokenTree::Group(Group::new(Delimiter::Parenthesis, proc_macro2::TokenStream::new())),
                                _ => continue,
                            };
                            
                            let mut new_closing = TokenTree::Punct(Punct::new(closing_char, Spacing::Alone));
                            if closing_char == '>' {
                                new_closing = TokenTree::Punct(Punct::new(closing_char, Spacing::Joint));
                                let second_gt = TokenTree::Punct(Punct::new(closing_char, Spacing::Alone));
                                stream.insert(i + 2, second_gt);
                            }
                            
                            stream.insert(i, new_opening);
                            stream.insert(i + 2, new_closing);
                            break;
                        }
                    }
                }
                
                let new_stream = proc_macro2::TokenStream::from_iter(stream.into_iter());
                *g = Group::new(delimiter, new_stream);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro definitions containing metavariables within delimited groups in expansion bodies. It inserts mismatched delimiter pairs around metavariables, creating parser confusion during macro expansion. By adding an opening delimiter before a metavariable and a different closing delimiter after it, this transformation tests the compiler's resilience to delimiter mismatches in macro contexts, potentially triggering ICEs or incorrect parsing behavior similar to real-world bugs involving macro fragment specifiers."
    }
}
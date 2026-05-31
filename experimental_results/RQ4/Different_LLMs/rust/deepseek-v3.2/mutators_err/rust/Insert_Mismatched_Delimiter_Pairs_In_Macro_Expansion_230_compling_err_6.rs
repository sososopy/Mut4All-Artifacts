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
    fn mutate(&self, mut file: &mut syn::File) {
        let mut rng = thread_rng();
        let delimiters = [('{', '}'), ('[', ']'), ('(', ')')];
        let mut candidates = Vec::new();
        
        struct MacroRuleVisitor<'a> {
            candidates: &'a mut Vec<(usize, usize, proc_macro2::TokenTree, proc_macro2::TokenTree)>,
            rule_index: usize,
        }
        
        impl<'a> VisitMut for MacroRuleVisitor<'a> {
            fn visit_item_macro_mut(&mut self, i: &mut ItemMacro) {
                if let Some(mac) = &mut i.mac {
                    self.visit_macro_mut(mac);
                }
            }
            
            fn visit_macro_mut(&mut self, mac: &mut Macro) {
                let mut tokens = mac.tokens.clone().into_iter().collect::<Vec<_>>();
                let mut token_index = 0;
                let mut prev_tt: Option<proc_macro2::TokenTree> = None;
                let mut new_tokens = Vec::new();
                
                for tt in tokens.iter_mut() {
                    if let proc_macro2::TokenTree::Group(g) = tt {
                        let mut found_metavar = false;
                        let mut inner_tokens = g.stream().into_iter().collect::<Vec<_>>();
                        let mut new_inner = Vec::new();
                        
                        for inner_tt in &mut inner_tokens {
                            if let proc_macro2::TokenTree::Ident(ident) = inner_tt {
                                if ident.to_string().starts_with('$') {
                                    found_metavar = true;
                                }
                            }
                            new_inner.push(std::mem::replace(inner_tt, proc_macro2::TokenTree::Ident(proc_macro2::Ident::new("", Span::call_site()))));
                        }
                        
                        if found_metavar && !new_inner.is_empty() {
                            let stream = proc_macro2::TokenStream::from_iter(new_inner.into_iter());
                            let new_group = proc_macro2::Group::new(g.delimiter(), stream);
                            self.candidates.push((self.rule_index,1, proc_macro2::TokenTree::Group(g.clone()), proc_macro2::TokenTree::Group(new_group)));
                        } else {
                            let stream = proc_macro2::TokenStream::from_iter(new_inner.into_iter());
                            let new_group = proc_macro2::Group::new(g.delimiter(), stream);
                            *tt = proc_macro2::TokenTree::Group(new_group);
                        }
                    }
                    
                    if let Some(prev) = prev_tt {
                        if let proc_macro2::TokenTree::Ident(ident) = tt {
                            if ident.to_string().starts_with('$') {
                                if let proc_macro2::TokenTree::Punct(p) = prev {
                                    if p.as_char() == ':' {
                                        if let Some(pprev) = new_tokens.get(new_tokens.len().wrapping_sub(2)) {
                                            if let proc_macro2::TokenTree::Ident(prev_ident) = pprev {
                                                if prev_ident.to_string().starts_with('$') {
                                                    self.candidates.push((self.rule_index, token_index.wrapping_sub(2), new_tokens[new_tokens.len().wrapping_sub(2)].clone(), tt.clone()));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    new_tokens.push(std::mem::replace(tt, proc_macro2::TokenTree::Ident(proc_macro2::Ident::new("", Span::call_site()))));
                    prev_tt = new_tokens.last().cloned();
                    token_index += 1;
                }
                
                mac.tokens = proc_macro2::TokenStream::from_iter(new_tokens.into_iter());
            }
        }
        
        let mut visitor = MacroRuleVisitor {
            candidates: &mut candidates,
            rule_index: 0,
        };
        visitor.visit_file_mut(file);
        
        if !candidates.is_empty() {
            let candidate = candidates.choose(&mut rng).unwrap();
            let (_, _, target_tt, metavar_tt) = candidate;
            if let proc_macro2::TokenTree::Group(g) = &target_tt {
                let delimiter = g.delimiter();
                let mut stream = g.stream().into_iter().collect::<Vec<_>>();
                
                for (i, tt) in stream.iter_mut().enumerate() {
                    if let proc_macro2::TokenTree::Ident(ident) = tt {
                        if ident.to_string().starts_with('$') {
                            let opening_delim = delimiters.choose(&mut rng).unwrap();
                            let mut closing_delim = delimiters.choose(&mut rng).unwrap();
                            
                            while closing_delim.0 == opening_delim.0 {
                                closing_delim = delimiters.choose(&mut rng).unwrap();
                            }
                            
                            let opening_char = opening_delim.0;
                            let closing_char = closing_delim.1;
                            
                            let new_opening = match opening_char {
                                '{' => proc_macro2::TokenTree::Group(proc_macro2::Group::new(proc_macro2::Delimiter::Brace, proc_macro2::TokenStream::new())),
                                '[' => proc_macro2::TokenTree::Group(proc_macro2::Group::new(proc_macro2::Delimiter::Bracket, proc_macro2::TokenStream::new())),
                                '(' => proc_macro2::TokenTree::Group(proc_macro2::Group::new(proc_macro2::Delimiter::Parenthesis, proc_macro2::TokenStream::new())),
                                _ => continue,
                            };
                            
                            let mut new_closing = proc_macro2::TokenTree::Punct(proc_macro2::Punct::new(closing_char, proc_macro2::Spacing::Alone));
                            if closing_char == '>' {
                                new_closing = proc_macro2::TokenTree::Punct(proc_macro2::Punct::new(closing_char, proc_macro2::Spacing::Joint));
                                let second_gt = proc_macro2::TokenTree::Punct(proc_macro2::Punct::new(closing_char, proc_macro2::Spacing::Alone));
                                stream.insert(i + 2, second_gt);
                            }
                            
                            stream.insert(i, new_opening);
                            stream.insert(i + 2, new_closing);
                            break;
                        }
                    }
                }
                
                let new_stream = proc_macro2::TokenStream::from_iter(stream.into_iter());
                let mut visitor = MacroRuleVisitor {
                    candidates: &mut Vec::new(),
                    rule_index: 0,
                };
                visitor.visit_file_mut(file);
                *file = syn::parse2(new_stream).unwrap();
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro definitions containing metavariables within delimited groups in expansion bodies. It inserts mismatched delimiter pairs around metavariables, creating parser confusion during macro expansion. By adding an opening delimiter before a metavariable and a different closing delimiter after it, this transformation tests the compiler's resilience to delimiter mismatches in macro contexts, potentially triggering ICEs or incorrect parsing behavior similar to real-world bugs involving macro fragment specifiers."
    }
}
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

pub struct Replace_Macro_Path_Specifier_205;

impl Mutator for Replace_Macro_Path_Specifier_205 {
    fn name(&self) -> &str {
        "Replace_Macro_Path_Specifier_205"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let new_specifiers = ["ty", "ident"];
        
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if let Some(macro_def) = &mut item_macro.mac {
                    if let Some(rules) = extract_macro_rules(macro_def) {
                        for rule in rules {
                            for fragment in &mut rule.pats {
                                if let MacroPat::Pat(pat) = fragment {
                                    if let Pat::Ident(pat_ident) = &**pat {
                                        if let Some((colon_token, fragment_specifier)) = pat_ident.colon_token.zip(&pat_ident.ty) {
                                            if let MacroPatSpec::Path = fragment_specifier {
                                                let new_spec = new_specifiers.choose(&mut rng).unwrap();
                                                *fragment_specifier = match *new_spec {
                                                    "ty" => MacroPatSpec::Ty,
                                                    "ident" => MacroPatSpec::Ident,
                                                    _ => continue,
                                                };
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro definitions using `path` fragment specifiers, replacing them with `ty` or `ident` specifiers. This alters how macro inputs are parsed, especially when followed by tokens like `<` or `>` that could be interpreted as generic arguments. The mutation tests edge cases in macro expansion, non-eager parsing, and parent scope resolution by changing parsing behavior while keeping macro bodies intact."
    }
}

fn extract_macro_rules(mac: &mut syn::Macro) -> Option<&mut Vec<MacroRule>> {
    if let Expr::Macro(expr_macro) = &mut *mac.tokens {
        if let Some(PathArguments::AngleBracketed(args)) = expr_macro.mac.path.segments.last_mut().map(|seg| &mut seg.arguments) {
            if let Some(GenericArgument::Type(Type::Macro(type_macro))) = args.args.first_mut() {
                if let syn::TypeMacro { mac: type_mac } = type_macro {
                    if type_mac.path.is_ident("macro_rules") {
                        if let Expr::Macro(inner_macro) = &mut *type_mac.tokens {
                            if let Some(TokenTree::Group(group)) = inner_macro.mac.tokens.clone().into_iter().next() {
                                let mut rules: Vec<MacroRule> = Vec::new();
                                for token in group.stream().into_iter() {
                                    if let TokenTree::Group(rule_group) = token {
                                        let mut pats = Vec::new();
                                        for pat_token in rule_group.stream().into_iter() {
                                            if let TokenTree::Ident(ident) = pat_token {
                                                if ident == "path" {
                                                    pats.push(MacroPat::Spec(MacroPatSpec::Path));
                                                } else if ident == "ty" {
                                                    pats.push(MacroPat::Spec(MacroPatSpec::Ty));
                                                } else if ident == "ident" {
                                                    pats.push(MacroPat::Spec(MacroPatSpec::Ident));
                                                } else {
                                                    pats.push(MacroPat::Ident(PatIdent {
                                                        ident: ident.clone(),
                                                        colon_token: None,
                                                        ty: None,
                                                    }));
                                                }
                                            }
                                        }
                                        rules.push(MacroRule { pats });
                                    }
                                }
                                return Some(Box::leak(Box::new(rules)));
                            }
                        }
                    }
                }
            }
        }
    }
    None
}

struct MacroRule {
    pats: Vec<MacroPat>,
}

enum MacroPat {
    Pat(Box<Pat>),
    Ident(PatIdent),
    Spec(MacroPatSpec),
}

struct PatIdent {
    ident: Ident,
    colon_token: Option<Token![:]>,
    ty: Option<MacroPatSpec>,
}

enum MacroPatSpec {
    Path,
    Ty,
    Ident,
}
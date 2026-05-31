use proc_macro2::{Span, TokenTree};
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

pub struct Inline_Assembly_Immediate_Mutation_172;

impl Mutator for Inline_Assembly_Immediate_Mutation_172 {
    fn name(&self) -> &str {
        "Inline_Assembly_Immediate_Mutation_172"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let syn::Stmt::Expr(expr, _) = stmt {
                        if let syn::Expr::Macro(macro_expr) = expr {
                            if macro_expr.mac.path.is_ident("asm") {
                                let mut new_tokens = proc_macro2::TokenStream::new();
                                for token_tree in macro_expr.mac.tokens.clone().into_iter() {
                                    if let TokenTree::Literal(lit) = token_tree {
                                        let lit_str = lit.to_string();
                                        if lit_str.starts_with('"') && lit_str.ends_with('"') {
                                            let mut mutated_string = lit_str.trim_matches('"').to_string();
                                            let re = Regex::new(r"\b(-?\d+)\b").unwrap();
                                            let mut replacements = Vec::new();
                                            for capture in re.captures_iter(&mutated_string) {
                                                let num_str = capture.get(1).unwrap().as_str();
                                                let original_num = num_str.parse::<i64>().unwrap_or(0);
                                                let mut rng = thread_rng();
                                                let choices = vec![0, 1, 65535, 4294967295, -1, original_num + 1, original_num - 1];
                                                let new_num = *choices.choose(&mut rng).unwrap();
                                                replacements.push((capture.get(1).unwrap().start(), capture.get(1).unwrap().end(), new_num.to_string()));
                                            }
                                            for (start, end, new_str) in replacements.iter().rev() {
                                                mutated_string.replace_range(*start..*end, &new_str);
                                            }
                                            new_tokens.extend(quote! { #mutated_string });
                                        } else {
                                            new_tokens.extend(quote! { #lit });
                                        }
                                    } else {
                                        new_tokens.extend(quote! { #token_tree });
                                    }
                                }
                                macro_expr.mac.tokens = new_tokens;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inline assembly blocks (`asm!` macro invocations) within Rust code. It applies when the `asm!` macro is used with a string literal that contains numeric immediate operands (e.g., `0`, `1`, `123`). The operator mutates these numeric immediates to other values, specifically focusing on edge cases such as zero, small positive numbers, large numbers, and negative numbers (if syntactically allowed by the assembly syntax), to test the compiler's handling of different immediate values in assembly strings."
    }
}
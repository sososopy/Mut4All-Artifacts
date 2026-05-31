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

pub struct Modify_Inline_Assembly_Directives_171;

impl Mutator for Modify_Inline_Assembly_Directives_171 {
    fn name(&self) -> &str {
        "Modify_Inline_Assembly_Directives_171"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if item_macro.mac.path.is_ident("global_asm") {
                    let mut tokens = item_macro.mac.tokens.clone().into_iter().collect::<Vec<_>>();
                    for token in tokens.iter_mut() {
                        if let proc_macro2::TokenTree::Literal(lit) = token {
                            let lit_str = lit.to_string();
                            if lit_str.starts_with('\"') && lit_str.ends_with('\"') {
                                let content = &lit_str[1..lit_str.len() - 1];
                                let mut mutated_content = String::new();
                                let mut last_end = 0;
                                for caps in Regex::new(r"\b\w+\b")
                                    .unwrap()
                                    .captures_iter(content)
                                {
                                    if let Some(matched) = caps.get(0) {
                                        let start = matched.start();
                                        let end = matched.end();
                                        mutated_content.push_str(&content[last_end..start]);
                                        let original = &content[start..end];
                                        let mutated = match original {
                                            "MOV" => "MOOV",
                                            "ADD" => "ADDD",
                                            "SUB" => "SUUB",
                                            "JMP" => "JMMP",
                                            "NOP" => "NOOP",
                                            _ => original,
                                        };
                                        mutated_content.push_str(mutated);
                                        last_end = end;
                                    }
                                }
                                mutated_content.push_str(&content[last_end..]);
                                *lit = proc_macro2::Literal::string(&mutated_content);
                            }
                        }
                    }
                    item_macro.mac.tokens = proc_macro2::TokenStream::from_iter(tokens);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets `global_asm!` macro invocations by identifying inline assembly directives and mnemonics within the macro's string content. It introduces syntax errors by altering known instruction mnemonics to non-standard forms, such as changing `MOV` to `MOOV`. This transformation aims to stress-test the compiler's handling of inline assembly by introducing deliberate assembly syntax errors, potentially uncovering weaknesses or bugs in the compiler's assembly parsing and error recovery mechanisms."
    }
}
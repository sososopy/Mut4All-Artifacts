use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*, str::FromStr};
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

pub struct Modify_Inline_Assembly_Operand_172;

impl Mutator for Modify_Inline_Assembly_Operand_172 {
    fn name(&self) -> &str {
        "Modify_Inline_Assembly_Operand_172"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                if let Some(ident) = &item_macro.mac.path.get_ident() {
                    if *ident == "asm" {
                        let mut tokens = item_macro.mac.tokens.clone();
                        let mut tokens_string = tokens.to_string();
                        let re = Regex::new(r#"global\.get\s+(\d+)"#).unwrap();
                        tokens_string = re
                            .replace(&tokens_string, |caps: &regex::Captures| {
                                let num = caps[1].parse::<u32>().unwrap();
                                format!("global.get {}", num + 1)
                            })
                            .to_string();
                        let new_tokens = proc_macro2::TokenStream::from_str(&tokens_string).unwrap();
                        item_macro.mac.tokens = new_tokens;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets inline assembly blocks using the `asm!` macro, specifically modifying operands that match the pattern `global.get <number>`. It increments the operand number by 1, transforming instructions like `global.get 0` to `global.get 1`. This mutation tests the compiler's handling of inline assembly operand variations, particularly in how it resolves and generates code for different operand values within the constraints of the target architecture's assembly language."
    }
}
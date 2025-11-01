use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Inline_Asm_Directives_313;

impl Mutator for Modify_Inline_Asm_Directives_313 {
    fn name(&self) -> &str {
        "Modify_Inline_Asm_Directives_313"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(ref mut mac) = item {
                if mac.mac.path.is_ident("global_asm") {
                    if let Some(tt) = mac.mac.tokens.clone().into_iter().next() {
                        if let TokenTree::Literal(lit) = tt {
                            let asm_code = lit.to_string();
                            let mut rng = thread_rng();
                            let mutated_code = Regex::new(r"\b(MOV|ADD|SUB|MUL|DIV)\b")
                                .unwrap()
                                .replace_all(&asm_code, |caps: &regex::Captures| {
                                    let instr = caps.get(0).unwrap().as_str();
                                    let mut chars: Vec<char> = instr.chars().collect();
                                    chars.shuffle(&mut rng);
                                    chars.iter().collect::<String>()
                                });
                            mac.mac.tokens = quote! { #mutated_code };
                        }
                    }
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the `global_asm!` macro, which includes inline assembly code. The operator identifies valid assembly instruction mnemonics such as MOV, ADD, SUB, etc., and mutates them by shuffling their characters, creating a non-standard mnemonic. This mutation aims to introduce syntax errors in the assembly code while maintaining valid Rust syntax, potentially exposing issues in the Rust compiler's handling of inline assembly."
    }
}
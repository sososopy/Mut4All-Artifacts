use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use std::str::FromStr;
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

pub struct Invalid_MacroStmt_228;

impl Mutator for Invalid_MacroStmt_228 {
    fn name(&self) -> &str {
        "Invalid_MacroStmt_228"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                if let Ok(block) = syn::parse2::<syn::Block>(macro_item.mac.tokens.clone()) {
                    let mut modified_block = block.clone();
                    for stmt in &mut modified_block.stmts {
                        if let Stmt::Local(local) = stmt {
                            let original_stmt = quote!(#stmt).to_string();
                            let modified_stmt = original_stmt.replace("let", "var");
                            let modified_block_str = format!("{{{}}}", modified_stmt);
                            let modified_block_tokens = proc_macro2::TokenStream::from_str(&modified_block_str).unwrap();
                            macro_item.mac.tokens = modified_block_tokens.into();
                            break;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations expecting a statement argument. It replaces a valid 'let' statement with an invalid 'var' statement, introducing a non-terminal parse error. This tests the compiler's error recovery and macro parsing logic when encountering unexpected tokens in NtStmt contexts."
    }
}
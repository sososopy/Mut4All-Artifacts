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

pub struct Transform_Thread_Local_Storage_101;

impl Mutator for Transform_Thread_Local_Storage_101 {
    fn name(&self) -> &str {
        "Transform_Thread_Local_Storage_101"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if let Some(mac) = item_macro.mac.path.segments.last() {
                    if mac.ident == "thread_local" {
                        if let Ok(mut tokens) = syn::parse2::<syn::Block>(item_macro.mac.tokens.clone()) {
                            for stmt in &mut tokens.stmts {
                                if let Stmt::Item(Item::Static(item_static)) = stmt {
                                    let init_expr = &item_static.expr;
                                    let mutated_expr: Expr = parse_quote! {{
                                        let x = #init_expr;
                                        if condition() {
                                            x.modify();
                                        }
                                        x
                                    }};
                                    item_static.expr = Box::new(mutated_expr);
                                }
                            }
                            item_macro.mac.tokens = tokens.into_token_stream();
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macros using thread-local storage, specifically transforming the initialization of thread-local static variables. By introducing a conditional logic block within the macro, it tests the compiler's handling of complex initialization scenarios. This involves wrapping the initializer with a conditional statement to decide the initialization path, potentially exposing edge cases in the compiler's macro expansion and type checks."
    }
}
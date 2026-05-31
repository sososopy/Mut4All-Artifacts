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

pub struct Modify_Coroutine_Blocking_Context_10;

impl Mutator for Modify_Coroutine_Blocking_Context_10 {
    fn name(&self) -> &str {
        "Modify_Coroutine_Blocking_Context_10"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut block_on_found = false;
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(Expr::Call(ExprCall { func, args, .. }), _) = stmt {
                        if let Expr::Path(ExprPath { path, .. }) = &**func {
                            if path.segments.last().unwrap().ident == "block_on" {
                                block_on_found = true;
                                if let Some(expr) = args.iter_mut().nth(1) {
                                    *stmt = Stmt::Expr(parse_quote! { async move { #expr }.await; }, None);
                                }
                            }
                        }
                    }
                }
                if block_on_found {
                    func.sig.asyncness = Some(token::Async { span: Span::call_site() });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
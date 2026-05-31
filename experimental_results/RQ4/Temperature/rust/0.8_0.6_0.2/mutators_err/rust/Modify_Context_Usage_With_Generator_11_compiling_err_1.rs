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

pub struct Modify_Context_Usage_With_Generator_11;

impl Mutator for Modify_Context_Usage_With_Generator_11 {
    fn name(&self) -> &str {
        "Modify_Context_Usage_With_Generator_11"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if func.sig.ident == "start" {
                            if let ReturnType::Type(_, return_type) = &func.sig.output {
                                if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                    if let Some(last_stmt) = func.block.stmts.last_mut() {
                                        if let Stmt::Expr(expr) = last_stmt {
                                            if let Expr::Closure(closure) = expr {
                                                if let Some(Stmt::Expr(expr)) = closure.body.as_ref() {
                                                    if let Expr::Match(expr_match) = expr {
                                                        let new_match = parse_quote! {
                                                            match *ctx.link {
                                                                SomeFieldOrMethod(ref value) => {
                                                                    value.do_something();
                                                                },
                                                                _ => (),
                                                            }
                                                        };
                                                        *expr_match = new_match;
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
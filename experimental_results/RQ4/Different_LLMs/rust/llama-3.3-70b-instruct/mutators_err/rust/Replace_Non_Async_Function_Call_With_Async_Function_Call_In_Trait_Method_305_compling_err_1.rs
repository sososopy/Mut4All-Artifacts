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

pub struct Replace_Non_Async_Function_Call_With_Async_Function_Call_In_Trait_Method_305;

impl Mutator for Replace_Non_Async_Function_Call_With_Async_Function_Call_In_Trait_Method_305 {
    fn name(&self) -> &str {
        "Replace_Non_Async_Function_Call_With_Async_Function_Call_In_Trait_Method_305"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut async_func = func.clone();
                        async_func.sig.asyncness = Some(token::Async {
                            span: Span::call_site(),
                        });
                        *func = async_func;
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr) = stmt {
                                if let Expr::Call(expr_call) = &**expr {
                                    if let ExprPath(path) = &*expr_call.expr {
                                        if path.path.segments.last().unwrap().ident == "foo" {
                                            let new_expr = parse_quote! {
                                                async {
                                                    #expr_call
                                                }
                                            };
                                            *expr = Box::new(new_expr);
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
        "The mutation operator replaces non-async function calls with async function calls in trait methods. This transformation tests the compiler's ability to handle indirect recursion of async trait method calls and ensures correct normalization and type checking of recursive async calls."
    }
}
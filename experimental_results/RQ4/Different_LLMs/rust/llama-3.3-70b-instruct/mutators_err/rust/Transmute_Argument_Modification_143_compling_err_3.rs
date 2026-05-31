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

pub struct Transmute_Argument_Modification_143;

impl Mutator for Transmute_Argument_Modification_143 {
    fn name(&self) -> &str {
        "Transmute_Argument_Modification_143"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Expr(expr) = item {
                if let Expr::Call(ExprCall { func, args, .. }) = expr {
                    if let Expr::Path(ExprPath { path, .. }) = func {
                        if path.is_ident("transmute") {
                            let mut new_args = args.clone();
                            let inf_var = Pat::Ident(Ident::new("_inf_var", Span::call_site()));
                            new_args.push(Expr::Closure(
                                syn::ExprClosure {
                                    attrs: vec![],
                                    constness: None,
                                    asyncness: None,
                                    movability: None,
                                    capture: None,
                                    lifetimes: Lifetime::none(),
                                    inputs: syn::punctuated::Punctuated::from_iter(vec![FnArg::Typed(
                                        syn::ArgTyped {
                                            pat: Box::new(inf_var),
                                            ty: None,
                                        }
                                    )]),
                                    output: ReturnType::Default,
                                    or1_token: None,
                                    body: Box::new(Expr::Block(
                                        syn::ExprBlock {
                                            attrs: vec![],
                                            label: None,
                                            block: syn::Block {
                                                brace_token: token::Brace::default(),
                                                stmts: vec![],
                                            },
                                        }
                                    )),
                                }
                            ));
                            *expr = Expr::Call(ExprCall {
                                attrs: vec![],
                                func: Box::new(Expr::Path(ExprPath {
                                    attrs: vec![],
                                    path: path.clone(),
                                    qself: None,
                                })),
                                paren_token: token::Paren::default(),
                                args: new_args,
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets the std::mem::transmute function and modifies its argument to introduce inference variables. It applies to code constructs that use transmute with closure arguments and replaces the original closure argument with a new closure that captures an inference variable."
    }
}
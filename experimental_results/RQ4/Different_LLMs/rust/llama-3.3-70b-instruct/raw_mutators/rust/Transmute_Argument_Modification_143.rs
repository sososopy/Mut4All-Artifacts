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
                if let Expr::Call(ExprCall { expr, args, .. }) = &**expr {
                    if let Expr::Path(ExprPath { path, .. }) = &**expr {
                        if path.is_ident("transmute") {
                            let mut new_args = args.clone();
                            let inf_var = PatType {
                                attrs: vec![],
                                pat: Box::new(Pat::Ident(Ident::new("_inf_var", Span::call_site()))),
                                colon_token: token::Colon::default(),
                                ty: Box::new(Type::Infer(TypeInfer {
                                    underscore_token: token::Underscore::default(),
                                })),
                            };
                            new_args.push(Expr::Closure(
                                syn::ExprClosure {
                                    asyncness: None,
                                    movability: None,
                                    capture: None,
                                    inputs: syn::punctuated::Punctuated::from_iter(vec![inf_var]),
                                    output: ReturnType::Default(Default::default()),
                                    body: Block {
                                        stmts: syn::punctuated::Punctuated::new(),
                                        ..Default::default()
                                    },
                                }
                            ));
                            *expr = Expr::Call(ExprCall {
                                attrs: vec![],
                                expr: Box::new(Expr::Path(ExprPath {
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
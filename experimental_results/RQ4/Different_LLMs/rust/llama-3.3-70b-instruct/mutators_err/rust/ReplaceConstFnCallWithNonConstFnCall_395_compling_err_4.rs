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

pub struct ReplaceConstFnCallWithNonConstFnCall_395;

impl Mutator for ReplaceConstFnCallWithNonConstFnCall_395 {
    fn name(&self) -> &str {
        "ReplaceConstFnCallWithNonConstFnCall_395"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Call(ExprCall {
                        attrs: _,
                        func: box Expr::Path(path),
                        paren_token: _,
                        args: _,
                    }), _) = stmt
                    {
                        let new_stmt = Stmt::Expr(
                            parse_quote! {
                                {
                                    let fun = #path;
                                    #path(#func.sig.inputs.iter().map(|arg| {
                                        match arg {
                                            FnArg::Typed(PatType { pat, .. }) => {
                                                quote!(#pat)
                                            }
                                            _ => panic!("Unsupported argument type"),
                                        }
                                    }).collect::<Vec<_>>());
                                }
                            },
                            None,
                        );
                        new_stmts.push(new_stmt);
                    } else {
                        new_stmts.push(stmt.clone());
                    }
                }
                func.block.stmts = new_stmts;
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant function calls in the code and replaces them with non-constant function calls. This transformation aims to trigger bugs related to the evaluation of constant expressions and the interaction between constant and non-constant code."
    }
}
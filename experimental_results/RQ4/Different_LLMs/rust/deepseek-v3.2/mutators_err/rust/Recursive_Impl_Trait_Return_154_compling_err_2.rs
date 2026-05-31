use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Recursive_Impl_Trait_Return_154;

impl Mutator for Recursive_Impl_Trait_Return_154 {
    fn name(&self) -> &str {
        "Recursive_Impl_Trait_Return_154"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(_) = &**return_type {
                        let fn_name = &item_fn.sig.ident;
                        let mut new_block = item_fn.block.clone();
                        new_block.stmts.clear();
                        
                        if item_fn.sig.inputs.len() > 0 {
                            let first_param = &item_fn.sig.inputs[0];
                            let arg_expr = match first_param {
                                FnArg::Typed(pat_type) => {
                                    let pat = &pat_type.pat;
                                    let ident = match pat {
                                        Pat::Ident(pat_ident) => pat_ident.ident.clone(),
                                        _ => Ident::new("arg", Span::call_site()),
                                    };
                                    Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: SynPath::from(ident),
                                    })
                                },
                                FnArg::Receiver(_) => {
                                    Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: SynPath::from(Ident::new("self", Span::call_site())),
                                    })
                                },
                            };
                            let recursive_call: Expr = parse_quote! { #fn_name(#fn_name(#arg_expr)) };
                            new_block.stmts.push(Stmt::Expr(recursive_call, None));
                        } else {
                            item_fn.sig.inputs.push(parse_quote!(_dummy: ()));
                            let recursive_call: Expr = parse_quote! { #fn_name(#fn_name(())) };
                            new_block.stmts.push(Stmt::Expr(recursive_call, None));
                        }
                        
                        item_fn.block = new_block;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning impl Trait and replaces their bodies with recursive calls using their own return values. For functions with parameters, it uses the first parameter as the argument. For parameterless functions, it adds a dummy unit parameter. This creates infinite type recursion patterns that stress the compiler's trait solving and type inference systems, potentially triggering ICEs similar to bug report patterns."
    }
}
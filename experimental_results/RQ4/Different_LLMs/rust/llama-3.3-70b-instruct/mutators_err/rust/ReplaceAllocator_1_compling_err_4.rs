use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct ReplaceAllocator_1;

impl Mutator for ReplaceAllocator_1 {
    fn name(&self) -> &str {
        "ReplaceAllocator_1"
    }

    fn chain_of_thought(&self) -> &str {
        "ReplaceAllocator_1 chain of thought"
    }

    fn mutate(&self, file: &mut syn::File) {
        let allocators = vec![
            "std::alloc::System",
            "std::alloc::Global",
            "std::alloc::System",
            "std::alloc::Global",
        ];

        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for allocator in &allocators {
                    let allocator_path = syn::parse_str(allocator).unwrap();
                    for statement in &mut func.block.stmts {
                        if let Stmt::Expr(expr, _) = statement {
                            if let Expr::Call(ExprCall {
                                attrs: _,
                                func,
                                paren_token: _,
                                args,
                            }) = expr
                            {
                                if let Expr::Path(ExprPath {
                                    attrs: _,
                                    path,
                                    qself: _,
                                }) = **func
                                {
                                    let path_segments = path.segments.clone();
                                    if path_segments.len() == 1
                                        && (path_segments[0].ident == "Box"
                                            || path_segments[0].ident == "Vec")
                                    {
                                        let mut new_args = Punctuated::new();
                                        for arg in args {
                                            if let Expr::Path(ExprPath {
                                                attrs: _,
                                                path: arg_path,
                                                qself: _,
                                            }) = arg
                                            {
                                                let arg_path_segments = arg_path.segments.clone();
                                                if arg_path_segments.len() == 1 {
                                                    new_args.push(arg.clone());
                                                }
                                            } else {
                                                new_args.push(arg.clone());
                                            }
                                        }
                                        let new_expr = Expr::Call(ExprCall {
                                            attrs: vec![],
                                            func: Box::new(Expr::Path(ExprPath {
                                                attrs: vec![],
                                                path: allocator_path.clone(),
                                                qself: None,
                                            })),
                                            paren_token: Paren::default(),
                                            args: new_args,
                                        });
                                        *statement = Stmt::Expr(new_expr, None);
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
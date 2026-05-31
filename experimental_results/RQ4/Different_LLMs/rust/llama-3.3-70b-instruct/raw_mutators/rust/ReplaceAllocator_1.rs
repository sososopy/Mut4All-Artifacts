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
                        if let Stmt::Expr(expr) = statement {
                            if let Expr::Call(ExprCall {
                                attrs: _,
                                expr,
                                paren_token: _,
                                args,
                            }) = expr
                            {
                                if let Expr::Path(ExprPath {
                                    attrs: _,
                                    path,
                                    qself: _,
                                }) = *expr
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
                                                if arg_path_segments.len() == 1
                                                    && arg_path_segments[0].ident == "new_in"
                                                {
                                                    new_args.push(parse_quote! {
                                                        #allocator_path
                                                    });
                                                } else {
                                                    new_args.push(arg);
                                                }
                                            } else {
                                                new_args.push(arg);
                                            }
                                        }
                                        *expr = parse_quote! {
                                            #path(#new_args)
                                        };
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
        "The mutation operator targets the code construct of memory allocation using the allocator API. It applies to any code that uses the allocator API to allocate memory, such as Box::new_in or Vec::with_capacity_in. The operator works by replacing the existing allocator with a different one, for example, replacing std::alloc::Global with std::alloc::System. This can help expose bugs related to memory allocation and deallocation, such as incorrect handling of allocator-specific functionality or differences in behavior between allocators."
    }
}
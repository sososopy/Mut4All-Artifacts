use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes,
    Expr,
    ExprCall,
    ExprPath,
    File,
    FnArg,
    GenericArgument,
    GenericParam,
    Ident,
    Item,
    ItemFn,
    ItemStruct,
    Lifetime,
    LifetimeParam,
    Local,
    Pat,
    PatType,
    Path as SynPath,
    PathArguments,
    ReturnType,
    Stmt,
    TraitBound,
    TraitBoundModifier,
    Type,
    TypeImplTrait,
    TypeParamBound,
    TypePath,
    parse_quote,
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

pub struct Generic_Const_Exprs_Array_Size_Mutation_366;

impl Mutator for Generic_Const_Exprs_Array_Size_Mutation_366 {
    fn name(&self) -> &str {
        "Generic_Const_Exprs_Array_Size_Mutation_366"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Type(pat_type) = &local.pat {
                            if let Type::Array(array_type) = &*local.ty {
                                if let Expr::Call(call_expr) = &array_type.len {
                                    if let Expr::Path(path_expr) = &call_expr.func {
                                        let new_len = parse_quote!( #path_expr + 1 );
                                        array_type.len = new_len;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Local(local) = stmt {
                                if let Pat::Type(pat_type) = &local.pat {
                                    if let Type::Array(array_type) = &*local.ty {
                                        if let Expr::Call(call_expr) = &array_type.len {
                                            if let Expr::Path(path_expr) = &call_expr.func {
                                                let new_len = parse_quote!( #path_expr + 1 );
                                                array_type.len = new_len;
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
        "The mutation operator targets array declarations with a const expression as the size. It replaces the const expression with a different constant expression, potentially triggering bugs related to const evaluation and borrow checking."
    }
}
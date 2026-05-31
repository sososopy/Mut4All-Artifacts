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

pub struct Modify_Const_Trait_Method_In_NonConst_Context_308;

impl Mutator for Modify_Const_Trait_Method_In_NonConst_Context_308 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Method_In_NonConst_Context_308"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_trait_methods = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait
                    .modifiers
                    .iter()
                    .any(|m| matches!(m, TraitModifier::Const(_)))
                {
                    for item in &item_trait.items {
                        if let TraitItem::Fn(item_fn) = item {
                            const_trait_methods.insert(item_fn.sig.ident.to_string());
                        }
                    }
                }
            }
        }
        let mut const_trait_impls = HashSet::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl
                    .modifiers
                    .iter()
                    .any(|m| matches!(m, ImplModifier::Const(_)))
                {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        const_trait_impls.insert(path.segments.last().unwrap().ident.to_string());
                    }
                }
            }
        }
        let mut trait_methods = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for item in &item_trait.items {
                    if let TraitItem::Fn(item_fn) = item {
                        trait_methods.insert(item_fn.sig.ident.to_string());
                    }
                }
            }
        }
        let mut impls = HashSet::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    impls.insert(path.segments.last().unwrap().ident.to_string());
                }
            }
        }
        let mut trait_map = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                trait_map.insert(item_trait.ident.to_string());
            }
        }
        let mut impl_map = HashSet::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    impl_map.insert(path.segments.last().unwrap().ident.to_string());
                }
            }
        }
        let mut const_trait_map = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                if item_trait
                    .modifiers
                    .iter()
                    .any(|m| matches!(m, TraitModifier::Const(_)))
                {
                    const_trait_map.insert(item_trait.ident.to_string());
                }
            }
        }
        let mut const_impl_map = HashSet::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl
                    .modifiers
                    .iter()
                    .any(|m| matches!(m, ImplModifier::Const(_)))
                {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        const_impl_map.insert(path.segments.last().unwrap().ident.to_string());
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods
                                                    .contains(&expr_path.path.segments.last().unwrap().ident.to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods
                                                        .contains(&expr_path.path.segments.last().unwrap().ident.to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impls.contains(&path.segments.last().unwrap().ident.to_string())
                    {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_impl_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if impl_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impls.contains(&path.segments.last().unwrap().ident.to_string())
                    {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_impl_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if impl_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impls.contains(&path.segments.last().unwrap().ident.to_string())
                    {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impls.contains(&path.segments.last().unwrap().ident.to_string())
                    {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impls.contains(&path.segments.last().unwrap().ident.to_string())
                    {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impls.contains(&path.segments.last().unwrap().ident.to_string())
                    {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impls.contains(&path.segments.last().unwrap().ident.to_string())
                    {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impls.contains(&path.segments.last().unwrap().ident.to_string())
                    {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impls.contains(&path.segments.last().unwrap().ident.to_string())
                    {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_map.contains(&path.segments.last().unwrap().ident.to_string()) {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                let mut has_const_method = false;
                                for stmt in &method.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Call(call) = expr {
                                            if let Expr::Path(expr_path) = &*call.func {
                                                if const_trait_methods.contains(&expr_path
                                                    .path
                                                    .segments
                                                    .last()
                                                    .unwrap()
                                                    .ident
                                                    .to_string())
                                                {
                                                    has_const_method = true;
                                                }
                                            }
                                        }
                                    }
                                }
                                if has_const_method {
                                    let mut new_stmts = Vec::new();
                                    for stmt in &method.block.stmts {
                                        if let Stmt::Expr(expr, semi) = stmt {
                                            if let Expr::Call(call) = expr {
                                                if let Expr::Path(expr_path) = &*call.func {
                                                    if const_trait_methods.contains(&expr_path
                                                        .path
                                                        .segments
                                                        .last()
                                                        .unwrap()
                                                        .ident
                                                        .to_string())
                                                    {
                                                        let mut new_args = Vec::new();
                                                        for arg in &call.args {
                                                            new_args.push(arg.clone());
                                                        }
                                                        let new_func = Expr::Path(ExprPath {
                                                            attrs: Vec::new(),
                                                            qself: None,
                                                            path: {
                                                                let mut segments =
                                                                    Punctuated::new();
                                                                for segment in &expr_path
                                                                    .path
                                                                    .segments
                                                                {
                                                                    segments.push(
                                                                        PathSegment {
                                                                            ident: segment.ident
                                                                                .clone(),
                                                                            arguments: segment
                                                                                .arguments
                                                                                .clone(),
                                                                        },
                                                                    );
                                                                }
                                                                segments
                                                                    .pop()
                                                                    .unwrap()
                                                                    .into_value();
                                                                segments.push(PathSegment {
                                                                    ident: Ident::new(
                                                                        &format!(
                                                                            "{}_non_const",
                                                                            expr_path
                                                                                .path
                                                                                .segments
                                                                                .last()
                                                                                .unwrap()
                                                                                .ident
                                                                                .to_string()
                                                                        ),
                                                                        Span::call_site(),
                                                                    ),
                                                                    arguments:
                                                                        PathArguments::None,
                                                                });
                                                                SynPath {
                                                                    leading_colon: None,
                                                                    segments,
                                                                }
                                                            },
                                                        });
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(ExprCall {
                                                                attrs: Vec::new(),
                                                                func: Box::new(new_func),
                                                                paren_token: Default::default(),
                                                                args: Punctuated::from_iter(
                                                                    new_args,
                                                                ),
                                                            }),
                                                            semi.clone(),
                                                        ));
                                                    } else {
                                                        new_stmts.push(Stmt::Expr(
                                                            Expr::Call(call.clone()),
                                                            semi.clone(),
                                                        ));
                                                    }
                                                }
                                            }
                                        } else {
                                            new_stmts.push(stmt.clone());
                                        }
                                    }
                                    method.block.stmts = new_stmts;
                                }
                            }
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if const_trait_impl
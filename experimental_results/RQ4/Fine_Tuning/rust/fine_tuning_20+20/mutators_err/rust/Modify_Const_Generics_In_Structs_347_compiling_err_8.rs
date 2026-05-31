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

pub struct Modify_Const_Generics_In_Structs_347;

impl Mutator for Modify_Const_Generics_In_Structs_347 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_In_Structs_347"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_structs = Vec::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if !item_struct.generics.params.is_empty() {
                    target_structs.push(item_struct.ident.clone());
                }
            }
        }
        let mut target_impls = Vec::new();
        for item in &file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(ident) = path.get_ident() {
                        if target_structs.contains(ident) {
                            target_impls.push(ident.clone());
                        }
                    }
                }
            }
        }
        let mut const_generic_structs = Vec::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct
                    .generics
                    .params
                    .iter()
                    .any(|param| matches!(param, GenericParam::Const(_)))
                {
                    const_generic_structs.push(item_struct.ident.clone());
                }
            }
        }
        let mut modified = false;
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(ident) = path.get_ident() {
                        if const_generic_structs.contains(ident) {
                            for impl_item in &mut item_impl.items {
                                if let ImplItem::Fn(method) = impl_item {
                                    if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                                        if let Type::Path(type_path) = &**ty {
                                            if let Some(segment) = type_path.path.segments.last() {
                                                if segment.ident == *ident {
                                                    if let PathArguments::AngleBracketed(
                                                        angle_args,
                                                    ) = &segment.arguments
                                                    {
                                                        if let Some(GenericArgument::Const(
                                                            const_expr,
                                                        )) = angle_args.args.last()
                                                        {
                                                            if let Expr::Block(expr_block) =
                                                                const_expr
                                                            {
                                                                if let Some(Stmt::Expr(expr, _)) =
                                                                    expr_block.block.stmts.first()
                                                                {
                                                                    if let Expr::Binary(
                                                                        expr_binary,
                                                                    ) = expr
                                                                    {
                                                                        if let Expr::Path(
                                                                            expr_path,
                                                                        ) = &*expr_binary.right
                                                                        {
                                                                            if let Some(
                                                                                right_ident,
                                                                            ) = expr_path
                                                                                .path
                                                                                .get_ident()
                                                                            {
                                                                                if right_ident
                                                                                    == "OFFSET"
                                                                                {
                                                                                    modified = true;
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
                            }
                        }
                    }
                }
            }
        }
        if modified {
            return;
        }
        let mut offset_declared = false;
        for item in &file.items {
            if let Item::Const(item_const) = item {
                if item_const.ident == "OFFSET" {
                    offset_declared = true;
                    break;
                }
            }
        }
        if !offset_declared {
            file.items.insert(0, parse_quote! { const OFFSET: usize = 5; });
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(ident) = path.get_ident() {
                        if const_generic_structs.contains(ident) {
                            for impl_item in &mut item_impl.items {
                                if let ImplItem::Fn(method) = impl_item {
                                    if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                                        if let Type::Path(type_path) = &**ty {
                                            if let Some(segment) = type_path.path.segments.last()
                                            {
                                                if segment.ident == *ident {
                                                    if let PathArguments::AngleBracketed(
                                                        angle_args,
                                                    ) = &segment.arguments
                                                    {
                                                        if let Some(GenericArgument::Const(
                                                            const_expr,
                                                        )) = angle_args.args.last()
                                                        {
                                                            if let Expr::Block(expr_block) =
                                                                const_expr
                                                            {
                                                                if let Some(Stmt::Expr(expr, _)) =
                                                                    expr_block.block.stmts.first()
                                                                {
                                                                    if let Expr::Binary(
                                                                        expr_binary,
                                                                    ) = expr
                                                                    {
                                                                        let new_right: Expr =
                                                                            parse_quote! { N + OFFSET };
                                                                        *expr_binary.right =
                                                                            new_right;
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
                }
            }
        }
        let mut has_trigger = false;
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "trigger_issue" {
                    has_trigger = true;
                    break;
                }
            }
        }
        if !has_trigger {
            file.items.push(parse_quote! {
                pub fn trigger_issue() {
                    Example::<0>.modify();
                }
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs using const generics and modifies impl functions that utilize these generics in their return type. It introduces an external constant into the const expression, altering the function's logic to depend on this external value. By doing so, it stresses the compiler's const evaluation and generic resolution mechanisms, aiming to uncover issues related to const dependency handling and error diagnostics. The mutator also ensures the presence of a trigger function to activate the modified behavior."
    }
}
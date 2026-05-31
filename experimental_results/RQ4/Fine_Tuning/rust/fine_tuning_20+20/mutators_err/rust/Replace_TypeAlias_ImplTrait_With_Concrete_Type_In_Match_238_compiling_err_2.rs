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

pub struct Replace_TypeAlias_ImplTrait_With_Concrete_Type_In_Match_238;

fn resolve_impl_trait(impl_trait: &TypeImplTrait) -> Option<Type> {
    let bounds: Vec<_> = impl_trait
        .bounds
        .iter()
        .filter_map(|bound| {
            if let TypeParamBound::Trait(trait_bound) = bound {
                Some(trait_bound)
            } else {
                None
            }
        })
        .collect();

    if bounds.len() != 1 {
        return None;
    }

    let trait_bound = &bounds[0];
    let segments: Vec<_> = trait_bound.path.segments.iter().collect();

    if segments.len() != 1 {
        return None;
    }

    let segment = &segments[0];
    if segment.ident == "Copy" {
        Some(parse_quote!(Option<(u32, u32)>))
    } else {
        None
    }
}

fn find_impl_trait_type_alias(items: &[Item]) -> Option<Ident> {
    for item in items {
        if let Item::Type(item_type) = item {
            if let Type::ImplTrait(_) = &*item_type.ty {
                return Some(item_type.ident.clone());
            }
        }
    }
    None
}

fn find_impl_trait_type(items: &[Item], type_ident: &Ident) -> Option<Type> {
    for item in items {
        if let Item::Type(item_type) = item {
            if item_type.ident == *type_ident {
                if let Type::ImplTrait(impl_trait) = &*item_type.ty {
                    return resolve_impl_trait(impl_trait);
                }
            }
        }
    }
    None
}

fn replace_type_alias_with_concrete_type(
    items: &mut [Item],
    type_ident: &Ident,
    concrete_type: &Type,
) {
    for item in items {
        if let Item::Type(item_type) = item {
            if item_type.ident == *type_ident {
                *item = Item::Verbatim(quote! {});
            }
        }
    }
}

fn replace_type_alias_in_let(local: &mut Local, type_ident: &Ident, concrete_type: &Type) -> bool {
    if let Some((_, ty)) = &mut local.ty {
        if let Type::Path(type_path) = &**ty {
            if type_path.qself.is_none()
                && type_path.path.segments.len() == 1
                && type_path.path.segments[0].ident == *type_ident
            {
                *ty = Box::new(concrete_type.clone());
                return true;
            }
        }
    }
    false
}

fn replace_type_alias_in_pat_type(
    pat_type: &mut PatType,
    type_ident: &Ident,
    concrete_type: &Type,
) -> bool {
    if let Type::Path(type_path) = &*pat_type.ty {
        if type_path.qself.is_none()
            && type_path.path.segments.len() == 1
            && type_path.path.segments[0].ident == *type_ident
        {
            pat_type.ty = Box::new(concrete_type.clone());
            return true;
        }
    }
    false
}

fn replace_type_alias_in_stmt(
    stmt: &mut Stmt,
    type_ident: &Ident,
    concrete_type: &Type,
) -> Option<Ident> {
    match stmt {
        Stmt::Local(local) => {
            if replace_type_alias_in_let(local, type_ident, concrete_type) {
                if let Pat::Ident(pat_ident) = &*local.pat {
                    return Some(pat_ident.ident.clone());
                }
            }
        }
        Stmt::Item(item) => {
            if let Item::Verbatim(_) = item {
                return None;
            }
            if let Item::Fn(_) = item {
                return None;
            }
            if let Item::Impl(_) = item {
                return None;
            }
            if let Item::Struct(_) = item {
                return None;
            }
            if let Item::Enum(_) = item {
                return None;
            }
            if let Item::Trait(_) = item {
                return None;
            }
            if let Item::Mod(_) = item {
                return None;
            }
            if let Item::Type(_) = item {
                return None;
            }
            if let Item::Union(_) = item {
                return None;
            }
            if let Item::ForeignMod(_) = item {
                return None;
            }
            if let Item::Static(_) = item {
                return None;
            }
            if let Item::Const(_) = item {
                return None;
            }
            if let Item::Macro(_) = item {
                return None;
            }
            if let Item::ExternCrate(_) = item {
                return None;
            }
            if let Item::Use(_) = item {
                return None;
            }
            if let Item::TraitAlias(_) = item {
                return None;
            }
            if let Item::Verbatim(_) = item {
                return None;
            }
            if let Item::Mod(_) = item {
                return None;
            }
            if let Item::Static(_) = item {
                return None;
            }
            if let Item::Const(_) = item {
                return None;
            }
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if replace_type_alias_in_pat_type(pat_type, type_ident, concrete_type) {
                            if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                return Some(pat_ident.ident.clone());
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if replace_type_alias_in_pat_type(
                                    pat_type,
                                    type_ident,
                                    concrete_type,
                                ) {
                                    if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                        return Some(pat_ident.ident.clone());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        _ => {}
    }
    None
}

fn replace_type_alias_in_block(
    block: &mut Block,
    type_ident: &Ident,
    concrete_type: &Type,
) -> Option<Ident> {
    for stmt in &mut block.stmts {
        if let Some(ident) = replace_type_alias_in_stmt(stmt, type_ident, concrete_type) {
            return Some(ident);
        }
    }
    None
}

fn replace_match_expr_with_concrete_type(
    expr: &mut Expr,
    var_ident: &Ident,
    concrete_type: &Type,
) {
    if let Expr::Match(expr_match) = expr {
        if let Expr::Path(expr_path) = &*expr_match.expr {
            if expr_path.path.is_ident(var_ident) {
                expr_match.expr = Box::new(Expr::Verbatim(quote! { #var_ident }));
                let new_arms = match concrete_type {
                    Type::Path(type_path) => {
                        if type_path.path.is_ident("Option") {
                            vec![
                                parse_quote! { None => () },
                                parse_quote! { Some((a, b)) => () },
                            ]
                        } else {
                            expr_match.arms.clone()
                        }
                    }
                    _ => expr_match.arms.clone(),
                };
                expr_match.arms = new_arms;
            }
        }
    }
}

fn replace_match_exprs_with_concrete_type(
    block: &mut Block,
    var_ident: &Ident,
    concrete_type: &Type,
) {
    for stmt in &mut block.stmts {
        if let Stmt::Expr(expr, _) = stmt {
            replace_match_expr_with_concrete_type(expr, var_ident, concrete_type);
        }
    }
}

impl Mutator for Replace_TypeAlias_ImplTrait_With_Concrete_Type_In_Match_238 {
    fn name(&self) -> &str {
        "Replace_TypeAlias_ImplTrait_With_Concrete_Type_In_Match_238"
    }
    fn mutate(&self, file: &mut syn::File) {
        if let Some(type_ident) = find_impl_trait_type_alias(&file.items) {
            if let Some(concrete_type) = find_impl_trait_type(&file.items, &type_ident) {
                replace_type_alias_with_concrete_type(&mut file.items, &type_ident, &concrete_type);
                for item in &mut file.items {
                    if let Item::Fn(item_fn) = item {
                        if let Some(var_ident) = replace_type_alias_in_block(
                            &mut item_fn.block,
                            &type_ident,
                            &concrete_type,
                        ) {
                            replace_match_exprs_with_concrete_type(
                                &mut item_fn.block,
                                &var_ident,
                                &concrete_type,
                            );
                        }
                    }
                    if let Item::Impl(item_impl) = item {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                if let Some(var_ident) = replace_type_alias_in_block(
                                    &mut method.block,
                                    &type_ident,
                                    &concrete_type,
                                ) {
                                    replace_match_exprs_with_concrete_type(
                                        &mut method.block,
                                        &var_ident,
                                        &concrete_type,
                                    );
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets type aliases using `impl Trait` and replaces their usage with concrete types in variable declarations. It ensures that match expressions using these variables are updated to pattern against the new concrete type, aiming to expose inconsistencies in type alias resolution and pattern matching."
    }
}
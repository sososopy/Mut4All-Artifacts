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

pub struct Modify_Inline_Attribute_For_Const_Generic_Expr_344;

impl Mutator for Modify_Inline_Attribute_For_Const_Generic_Expr_344 {
    fn name(&self) -> &str {
        "Modify_Inline_Attribute_For_Const_Generic_Expr_344"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let has_const_param = item_fn.sig.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Const(_))
                });
                if !has_const_param {
                    continue;
                }
                let where_clause = &item_fn.sig.generics.where_clause;
                let has_array_len_expr = if let Some(where_clause) = where_clause {
                    where_clause.predicates.iter().any(|pred| {
                        if let WherePredicate::Type(pred_type) = pred {
                            if let Type::Array(type_array) = &pred_type.bounded_ty {
                                if let Expr::Path(expr_path) = &type_array.len {
                                    let path_str = expr_path.path.segments.iter().map(|seg| seg.ident.to_string()).collect::<Vec<_>>().join("::");
                                    item_fn.sig.generics.params.iter().any(|param| {
                                        if let GenericParam::Const(const_param) = param {
                                            path_str.contains(&const_param.ident.to_string())
                                        } else {
                                            false
                                        }
                                    })
                                } else {
                                    false
                                }
                            } else {
                                false
                            }
                        } else {
                            false
                        }
                    })
                } else {
                    false
                };
                if !has_array_len_expr {
                    continue;
                }
                let mut has_inline = false;
                let mut inline_always = false;
                item_fn.attrs.retain(|attr| {
                    if attr.path().is_ident("inline") {
                        has_inline = true;
                        if let Ok(meta) = attr.parse_meta() {
                            if let Meta::List(meta_list) = meta {
                                if meta_list.tokens.to_string().contains("always") {
                                    inline_always = true;
                                    return false;
                                }
                            }
                        }
                        false
                    } else {
                        true
                    }
                });
                if has_inline && inline_always {
                } else if !has_inline {
                    item_fn.attrs.push(parse_quote!(#[inline(never)]));
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(item_fn) = impl_item {
                        let has_const_param = item_fn.sig.generics.params.iter().any(|param| {
                            matches!(param, GenericParam::Const(_))
                        });
                        if !has_const_param {
                            continue;
                        }
                        let where_clause = &item_fn.sig.generics.where_clause;
                        let has_array_len_expr = if let Some(where_clause) = where_clause {
                            where_clause.predicates.iter().any(|pred| {
                                if let WherePredicate::Type(pred_type) = pred {
                                    if let Type::Array(type_array) = &pred_type.bounded_ty {
                                        if let Expr::Path(expr_path) = &type_array.len {
                                            let path_str = expr_path.path.segments.iter().map(|seg| seg.ident.to_string()).collect::<Vec<_>>().join("::");
                                            item_fn.sig.generics.params.iter().any(|param| {
                                                if let GenericParam::Const(const_param) = param {
                                                    path_str.contains(&const_param.ident.to_string())
                                                } else {
                                                    false
                                                }
                                            })
                                        } else {
                                            false
                                        }
                                    } else {
                                        false
                                    }
                                } else {
                                    false
                                }
                            })
                        } else {
                            false
                        };
                        if !has_array_len_expr {
                            continue;
                        }
                        let mut has_inline = false;
                        let mut inline_always = false;
                        item_fn.attrs.retain(|attr| {
                            if attr.path().is_ident("inline") {
                                has_inline = true;
                                if let Ok(meta) = attr.parse_meta() {
                                    if let Meta::List(meta_list) = meta {
                                        if meta_list.tokens.to_string().contains("always") {
                                            inline_always = true;
                                            return false;
                                        }
                                    }
                                }
                                false
                            } else {
                                true
                            }
                        });
                        if has_inline && inline_always {
                        } else if !has_inline {
                            item_fn.attrs.push(parse_quote!(#[inline(never)]));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions and methods with const generic parameters used in array length expressions within where clauses. It checks for inline attributes and modifies them: if #[inline(always)] is present, it removes the attribute; if no inline attribute exists, it adds #[inline(never)]. This transformation alters the inlining behavior during constant propagation, potentially exposing mismatches between const evaluation contexts and parameter environments, which can trigger ICEs related to generic_const_exprs."
    }
}
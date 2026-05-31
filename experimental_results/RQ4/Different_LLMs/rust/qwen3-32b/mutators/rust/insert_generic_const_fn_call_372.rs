use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprBlock, File, FnArg, GenericArgument, GenericParam, Ident,
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
    WhereClause, WherePredicate,
    *,
};

use crate::mutator::Mutator;

pub struct Insert_Generic_Const_Fn_Call_372;

impl Mutator for Insert_Generic_Const_Fn_Call_372 {
    fn name(&self) -> &str {
        "Insert_Generic_Const_Fn_Call_372"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                for generic in &mut generics.params {
                    if let GenericParam::Const(const_param) = generic {
                        if let Some(expr) = &mut const_param.default {
                            let original_expr = expr.clone();
                            let new_block = parse_quote! {{
                                const fn tmp() { }
                                tmp();
                                #original_expr
                            }};
                            *expr = new_block;
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        let generics = &mut method.sig.generics;
                        for generic in &mut generics.params {
                            if let GenericParam::Const(const_param) = generic {
                                if let Some(expr) = &mut const_param.default {
                                    let original_expr = expr.clone();
                                    let new_block = parse_quote! {{
                                        const fn tmp() { }
                                        tmp();
                                        #original_expr
                                    }};
                                    *expr = new_block;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic const expressions in function parameters by inserting an inline const function call. It replaces the original expression with a block containing a nested const function declaration, an immediate call to it, and then returns the original value. This transformation increases AST complexity, stresses const evaluation contexts, and may expose compiler limitations in handling deeply nested const expressions with inline function definitions."
    }
}
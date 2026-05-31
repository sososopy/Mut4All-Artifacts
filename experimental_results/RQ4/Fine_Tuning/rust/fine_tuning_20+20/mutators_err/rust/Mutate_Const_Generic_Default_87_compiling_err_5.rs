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

pub struct Mutate_Const_Generic_Default_87;

impl Mutator for Mutate_Const_Generic_Default_87 {
    fn name(&self) -> &str {
        "Mutate_Const_Generic_Default_87"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut modified = false;
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &const_param.default {
                            if let Expr::Block(expr_block) = default.as_ref() {
                                let new_expr: Expr = parse_quote! { 10 / 0 };
                                const_param.default = Some(new_expr);
                                modified = true;
                            }
                        }
                    }
                }
                if modified {
                    let struct_ident = &item_struct.ident;
                    let generics = &item_struct.generics;
                    let fields = &item_struct.fields;
                    let new_struct = if let Some(semi) = &item_struct.semi_token {
                        quote! {
                            struct #struct_ident #generics #semi
                        }
                    } else {
                        quote! {
                            struct #struct_ident #generics #fields
                        }
                    };
                    *item = syn::parse2(new_struct).unwrap();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions with const generic parameters that have default values. It specifically looks for const generic defaults that use a `const` block, such as `const N: usize = { const {3} }`. The mutator replaces these defaults with a more complex expression that may trigger compile-time errors, such as a division by zero (`10 / 0`). By altering the const generic defaults in this way, the mutator aims to provoke internal compiler errors (ICEs) or other unexpected behaviors related to const evaluation and generic type resolution."
    }
}
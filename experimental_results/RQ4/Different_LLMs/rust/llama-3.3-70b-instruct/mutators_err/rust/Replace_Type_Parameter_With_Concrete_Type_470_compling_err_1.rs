use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Replace_Type_Parameter_With_Concrete_Type_470;

impl Mutator for Replace_Type_Parameter_With_Concrete_Type_470 {
    fn name(&self) -> &str {
        "Replace_Type_Parameter_With_Concrete_Type_470"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut concrete_types = Vec::new();
        for item in &file.items {
            if let Item::Struct(struct_item) = item {
                concrete_types.push(Ident::new(&struct_item.ident.to_string(), Span::call_site()));
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        if let Some(concrete_type) = concrete_types.choose(&mut thread_rng()) {
                            param = parse_quote!(#concrete_type);
                        }
                    }
                }
            }

            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                if let Some(concrete_type) = concrete_types.choose(&mut thread_rng()) {
                                    param = parse_quote!(#concrete_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces type parameters in generic functions and types with concrete types. This transformation tests the compiler's ability to handle type substitutions and ensures that it correctly resolves the types and their relationships."
    }
}
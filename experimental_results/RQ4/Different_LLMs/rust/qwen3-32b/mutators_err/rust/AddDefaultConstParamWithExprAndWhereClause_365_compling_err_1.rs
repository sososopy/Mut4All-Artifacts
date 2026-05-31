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

pub struct AddDefaultConstParamWithExprAndWhereClause_365;

impl Mutator for AddDefaultConstParamWithExprAndWhereClause_365 {
    fn name(&self) -> &str {
        "AddDefaultConstParamWithExprAndWhereClause_365"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let has_const = trait_item.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Const(_))
                });
                if !has_const {
                    trait_item.generics.params.push(parse_quote!(const X: usize));
                    trait_item.generics.params.push(parse_quote!(const Y: usize));
                    trait_item.generics.params.push(parse_quote!(const Z: usize = { X + Y }));

                    let mut where_clause = syn::WhereClause {
                        where_token: Default::default(),
                        predicates: Punctuated::new(),
                    };
                    let array_type = parse_quote! { [(); X + Y] };
                    let predicate = syn::WherePredicate::Type(syn::PredicateType {
                        lifetimes: None,
                        bounded_ty: Box::new(array_type),
                        colon_token: Default::default(),
                        bounds: Punctuated::new(),
                    });
                    where_clause.predicates.push(predicate);
                    trait_item.generics.where_clause = Some(where_clause);

                    let trait_ident = &trait_item.ident;
                    let impl_item = parse_quote! {
                        impl<const X: usize, const Y: usize> #trait_ident<X, Y> for MyStruct {}
                    };
                    file.items.push(Item::Impl(impl_item));

                    let has_my_struct = file.items.iter().any(|item| {
                        if let syn::Item::Struct(struct_item) = item {
                            struct_item.ident == "MyStruct"
                        } else {
                            false
                        }
                    });
                    if !has_my_struct {
                        let struct_item = parse_quote! {
                            pub struct MyStruct;
                        };
                        file.items.push(Item::Struct(struct_item));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
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

pub struct Add_Associated_Type_With_Lifetime_Bounds_In_Trait_And_Use_In_Struct_36;

impl Mutator for Add_Associated_Type_With_Lifetime_Bounds_In_Trait_And_Use_In_Struct_36 {
    fn name(&self) -> &str {
        "Add_Associated_Type_With_Lifetime_Bounds_In_Trait_And_Use_In_Struct_36"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                // Add associated type with lifetime and where clause to trait
                let associated_type = parse_quote! {
                    type Associated<'a> where Self: 'static;
                };
                trait_item.items.push(syn::TraitItem::Type(associated_type));

                // Create a new struct that uses the trait and its associated type
                let struct_name = format_ident!("NewStruct_{}", trait_item.ident);
                let trait_name = &trait_item.ident;

                // Define struct generics with trait bound and where clause
                let struct_generics = parse_quote! {
                    <T: #trait_name>
                };
                let where_clause = parse_quote! {
                    where for<'x> T::Associated<'x>: Clone
                };

                let mut struct_generics = struct_generics;
                struct_generics.where_clause = Some(where_clause);

                // Create struct with field using associated type
                let struct_item = syn::ItemStruct {
                    attrs: vec![],
                    vis: parse_quote!(pub),
                    struct_token: Default::default(),
                    ident: struct_name.clone(),
                    generics: struct_generics,
                    fields: syn::Fields::Named(syn::FieldsNamed {
                        brace_token: Default::default(),
                        named: Punctuated::from_iter(vec![syn::Field {
                            attrs: vec![],
                            vis: parse_quote!(pub),
                            ident: Some(Ident::new("data", Span::call_site())),
                            colon_token: Default::default(),
                            ty: Box::new(parse_quote!(T::Associated<'static>)),
                        }]),
                    }),
                    semi_token: None,
                };

                file.items.push(syn::Item::Struct(struct_item));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
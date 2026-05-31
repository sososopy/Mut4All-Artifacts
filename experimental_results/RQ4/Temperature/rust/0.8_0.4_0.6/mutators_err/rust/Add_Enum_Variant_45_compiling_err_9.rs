use proc_macro2::{Span};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command};
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

pub struct Add_Enum_Variant_45;

impl Mutator for Add_Enum_Variant_45 {
    fn name(&self) -> &str {
        "Add_Enum_Variant_45"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut variant_counter = 1;
        for item in &mut file.items {
            if let syn::Item::Enum(item_enum) = item {
                let new_variant_name = format!("NewVariant{}", variant_counter);
                let new_variant_ident = Ident::new(&new_variant_name, Span::call_site());
                let new_variant = syn::Variant {
                    attrs: Vec::new(),
                    ident: new_variant_ident,
                    fields: syn::Fields::Unnamed(syn::FieldsUnnamed {
                        paren_token: token::Paren::default(),
                        unnamed: Punctuated::from_iter(vec![syn::Field {
                            attrs: Vec::new(),
                            vis: syn::Visibility::Inherited,
                            ident: None,
                            colon_token: None,
                            ty: parse_quote!(i32),
                        }]),
                    }),
                    discriminant: None,
                };
                item_enum.variants.push(new_variant);
                variant_counter += 1;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
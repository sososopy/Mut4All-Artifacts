use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Add_Phantom_Alias_Field_264;

impl Mutator for Add_Phantom_Alias_Field_264 {
    fn name(&self) -> &str {
        "Add_Phantom_Alias_Field_264"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_my_trait = false;
        let mut has_wrapper = false;
        let mut has_nested_alias = false;
        let mut has_alias_alias = false;

        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.ident == "MyTrait" {
                    has_my_trait = true;
                }
            }
            if let Item::Struct(struct_item) = item {
                if struct_item.ident == "Wrapper" {
                    has_wrapper = true;
                }
            }
            if let Item::Type(type_item) = item {
                if type_item.ident == "Nested" {
                    has_nested_alias = true;
                }
                if type_item.ident == "Alias" {
                    has_alias_alias = true;
                }
            }
        }

        if !has_my_trait {
            let new_item = parse_quote! {
                trait MyTrait {
                    type Assoc;
                }
            };
            file.items.insert(0, new_item);
        }

        if !has_wrapper {
            let new_item = parse_quote! {
                struct Wrapper<T> {
                    _marker: std::marker::PhantomData<T>,
                }
            };
            file.items.insert(0, new_item);
        }

        if !has_nested_alias {
            let new_item = parse_quote! {
                type Nested<T> = T::Assoc;
            };
            file.items.insert(0, new_item);
        }

        if !has_alias_alias {
            let new_item = parse_quote! {
                type Alias<T> = Wrapper<Nested<T>>;
            };
            file.items.insert(0, new_item);
        }

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let new_param = parse_quote!(T: MyTrait);
                item_struct.generics.params.push(new_param);

                let new_field = syn::Field {
                    attrs: Vec::new(),
                    vis: syn::Visibility::Public(token::Pub::default()),
                    mutability: syn::FieldMutability::None,
                    ident: Some(Ident::new("phantom", Span::call_site())),
                    colon_token: Some(Default::default()),
                    ty: parse_quote!(std::marker::PhantomData<Alias<Nested<T>>>),
                };

                match &mut item_struct.fields {
                    syn::Fields::Named(fields_named) => {
                        fields_named.named.push(new_field);
                    }
                    _ => {
                        let mut fields_named = syn::FieldsNamed {
                            brace_token: Default::default(),
                            named: Punctuated::new(),
                        };
                        fields_named.named.push(new_field);
                        item_struct.fields = syn::Fields::Named(fields_named);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a PhantomData field with a nested type alias involving an associated type from a trait. It adds a generic parameter T with MyTrait bound to the struct, and a field of type PhantomData<Alias<Nested<T>>>. This creates a complex type-resolution scenario requiring the compiler to infer nested associated types, potentially exposing issues in trait system interactions and opaque type handling."
    }
}
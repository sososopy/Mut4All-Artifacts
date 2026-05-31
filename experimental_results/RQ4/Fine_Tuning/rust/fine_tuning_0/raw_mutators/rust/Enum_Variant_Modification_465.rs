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

pub struct Enum_Variant_Modification_465;

impl Mutator for Enum_Variant_Modification_465 {
    fn name(&self) -> &str {
        "Enum_Variant_Modification_465"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                if enum_item.variants.len() > 1 {
                    let new_struct_ident = Ident::new("NewStruct", Span::call_site());
                    let new_variant_ident = Ident::new("VariantNew", Span::call_site());

                    // Add new struct definition
                    file.items.push(syn::Item::Struct(ItemStruct {
                        attrs: vec![],
                        vis: syn::Visibility::Public(syn::VisPublic {
                            pub_token: token::Pub(Span::call_site()),
                        }),
                        struct_token: token::Struct(Span::call_site()),
                        ident: new_struct_ident.clone(),
                        generics: syn::Generics::default(),
                        fields: syn::Fields::Named(syn::FieldsNamed {
                            brace_token: token::Brace(Span::call_site()),
                            named: {
                                let mut fields = Punctuated::new();
                                fields.push(syn::Field {
                                    attrs: vec![],
                                    vis: syn::Visibility::Inherited,
                                    ident: Some(Ident::new("x", Span::call_site())),
                                    colon_token: Some(token::Colon(Span::call_site())),
                                    ty: Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path::from(Ident::new("i32", Span::call_site())),
                                    }),
                                });
                                fields
                            },
                        }),
                    }));

                    // Add new variant to enum
                    enum_item.variants.push(syn::Variant {
                        attrs: vec![],
                        ident: new_variant_ident.clone(),
                        fields: syn::Fields::Named(syn::FieldsNamed {
                            brace_token: token::Brace(Span::call_site()),
                            named: {
                                let mut fields = Punctuated::new();
                                fields.push(syn::Field {
                                    attrs: vec![],
                                    vis: syn::Visibility::Inherited,
                                    ident: Some(Ident::new("source", Span::call_site())),
                                    colon_token: Some(token::Colon(Span::call_site())),
                                    ty: Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path::from(new_struct_ident.clone()),
                                    }),
                                });
                                fields
                            },
                        }),
                        discriminant: None,
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a new variant to enums with multiple variants, using a newly declared struct with a unique field type. This transformation tests the compiler's handling of enum variant additions and associated data structures, potentially revealing weaknesses in enum logic and type trait handling."
    }
}
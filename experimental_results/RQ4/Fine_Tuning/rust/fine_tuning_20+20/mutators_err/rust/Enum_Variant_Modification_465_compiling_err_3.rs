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

pub struct Enum_Variant_Modification_465;

impl Mutator for Enum_Variant_Modification_465 {
    fn name(&self) -> &str {
        "Enum_Variant_Modification_465"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_enum = None;
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Enum(item_enum) = item {
                if item_enum.variants.len() > 1 {
                    target_enum = Some(index);
                    break;
                }
            }
        }
        if let Some(index) = target_enum {
            let new_struct_ident = Ident::new(
                &format!("NewStruct{}", index),
                Span::call_site(),
            );
            let new_struct = Item::Struct(ItemStruct {
                attrs: vec![],
                vis: syn::Visibility::Public(Default::default()),
                struct_token: Default::default(),
                ident: new_struct_ident.clone(),
                generics: Default::default(),
                fields: syn::Fields::Named(syn::FieldsNamed {
                    brace_token: Default::default(),
                    named: {
                        let mut named = Punctuated::new();
                        named.push(syn::Field {
                            attrs: vec![],
                            vis: syn::Visibility::Inherited,
                            ident: Some(Ident::new("x", Span::call_site())),
                            colon_token: Some(Default::default()),
                            ty: syn::Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new("i32", Span::call_site())),
                            }),
                        });
                        named
                    },
                }),
                semi_token: None,
            });
            file.items.insert(index, new_struct);
            if let Item::Enum(item_enum) = &mut file.items[index + 1] {
                let new_variant = syn::Variant {
                    attrs: vec![],
                    ident: Ident::new(
                        &format!("Variant{}", item_enum.variants.len() + 1),
                        Span::call_site(),
                    ),
                    fields: syn::Fields::Named(syn::FieldsNamed {
                        brace_token: Default::default(),
                        named: {
                            let mut named = Punctuated::new();
                            named.push(syn::Field {
                                attrs: vec![],
                                vis: syn::Visibility::Inherited,
                                ident: Some(Ident::new("source", Span::call_site())),
                                colon_token: Some(Default::default()),
                                ty: syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(new_struct_ident),
                                }),
                            });
                            named
                        },
                    }),
                    discriminant: None,
                };
                item_enum.variants.push(new_variant);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets enums with multiple variants, adding a new variant that uses a newly declared struct for its data. This transformation stresses enum handling, particularly in contexts involving transmutability or type trait assertions, by introducing additional variant complexity and testing the compiler's ability to adapt to expanded enum definitions."
    }
}
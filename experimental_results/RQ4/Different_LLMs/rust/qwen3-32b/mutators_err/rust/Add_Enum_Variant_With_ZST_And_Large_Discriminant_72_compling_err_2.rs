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

pub struct Add_Enum_Variant_With_ZST_And_Large_Discriminant_72;

impl Mutator for Add_Enum_Variant_With_ZST_And_Large_Discriminant_72 {
    fn name(&self) -> &str {
        "Add_Enum_Variant_With_ZST_And_Large_Discriminant_72"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_zst = false;
        for item in &file.items {
            if let syn::Item::Struct(s) = item {
                if s.ident == "ZST" {
                    has_zst = true;
                    break;
                }
            }
        }
        if !has_zst {
            let zst = parse_quote! {
                struct ZST;
            };
            file.items.push(zst);
        }

        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                let variant_name = Ident::new("MutatedVariant_72", Span::call_site());
                let variant_exists = enum_item.variants.iter().any(|v| v.ident == variant_name);
                if !variant_exists {
                    let new_variant = syn::Variant {
                        attrs: vec![],
                        ident: variant_name,
                        fields: syn::Fields::Unnamed(syn::FieldsUnnamed {
                            paren_token: Default::default(),
                            unnamed: {
                                let mut p = Punctuated::new();
                                let field = syn::Field {
                                    attrs: vec![],
                                    vis: syn::Visibility::Inherited,
                                    colon_token: None,
                                    ident: None,
                                    ty: Box::new(parse_quote!(ZST)),
                                };
                                p.push(field);
                                p
                            },
                        }),
                        discriminant: Some((
                            syn::token::Eq::default(),
                            parse_quote!(0x7FFFFFFF),
                        )),
                    };
                    enum_item.variants.push(new_variant);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an enum variant with a zero-sized type (ZST) field and an explicit discriminant of 0x7FFFFFFF to existing enums. This forces the compiler to evaluate discriminant calculations involving variant-type-dependent layouts, potentially exposing bugs in const evaluation or discriminant assignment logic."
    }
}
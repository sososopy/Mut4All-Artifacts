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

pub struct Replace_Enum_Variant_Type_With_Never_463;

impl Mutator for Replace_Enum_Variant_Type_With_Never_463 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_Type_With_Never_463"
    }
    fn mutate(&self, file: &mut syn::File) {
        let feature_attr = parse_quote! { #![feature(never_type)] };
        file.attrs.push(feature_attr);

        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    match variant.fields {
                        VariantFields::Unit => {
                            let new_fields = VariantFields::Tuple {
                                paren_token: Default::default(),
                                fields: syn::punctuated::Punctuated::new(),
                            };
                            let field = syn::Field {
                                attrs: Vec::new(),
                                vis: parse_quote! { pub },
                                ident: None,
                                colon_token: None,
                                mutability: None,
                                ty: parse_quote! { ! },
                            };
                            new_fields.fields.push_value(field);
                            variant.fields = new_fields;
                        },
                        VariantFields::Tuple(ref mut fields) => {
                            for field in fields {
                                if let syn::Type::Never(_) = *field.ty {
                                    continue;
                                }
                                field.ty = Box::new(parse_quote! { ! });
                            }
                        },
                        VariantFields::Struct(ref mut struct_fields) => {
                            for field in &mut struct_fields.fields {
                                if let syn::Type::Never(_) = *field.ty {
                                    continue;
                                }
                                field.ty = Box::new(parse_quote! { ! });
                            }
                        },
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
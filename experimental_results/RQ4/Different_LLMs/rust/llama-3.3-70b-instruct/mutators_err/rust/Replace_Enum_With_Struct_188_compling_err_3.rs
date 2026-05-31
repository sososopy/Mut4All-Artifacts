use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct ReplaceEnumWithStruct188;

impl Mutator for ReplaceEnumWithStruct188 {
    fn name(&self) -> &str {
        "ReplaceEnumWithStruct188"
    }

    fn chain_of_thought(&self) -> &str {
        "Replace enum with struct"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                let mut struct_fields = Vec::new();
                for variant in &enum_item.variants {
                    let field_name = variant.ident.clone();
                    let field_type = Type::Path(TypePath {
                        qself: None,
                        path: SynPath {
                            leading_colon: None,
                            segments: Punctuated::from_iter(vec![PathSegment {
                                ident: field_name,
                                arguments: PathArguments::None,
                            }]),
                        },
                    });
                    struct_fields.push((field_name, field_type));
                }
                let struct_item = ItemStruct {
                    attrs: vec![],
                    vis: enum_item.vis.clone(),
                    ident: enum_item.ident.clone(),
                    generics: enum_item.generics.clone(),
                    fields: syn::Fields::Named(syn::FieldsNamed {
                        named: Punctuated::from_iter(struct_fields.into_iter().map(|(field_name, field_type)| syn::Field {
                            attrs: vec![],
                            vis: syn::Visibility::Inherited,
                            ident: Some(field_name),
                            mutability: None,
                            colon_token: token::Colon,
                            ty: field_type,
                        })),
                        brace_token: token::Brace {
                            span: Span::call_site(),
                        },
                    }),
                    struct_token: token::Struct {
                        span: Span::call_site(),
                    },
                };
                *item = Item::Struct(struct_item);
            }
        }
    }
}
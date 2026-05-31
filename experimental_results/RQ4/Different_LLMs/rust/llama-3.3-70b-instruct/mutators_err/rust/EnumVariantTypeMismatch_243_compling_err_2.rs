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

pub struct EnumVariantTypeMismatch_243;

impl Mutator for EnumVariantTypeMismatch_243 {
    fn name(&self) -> &str {
        "EnumVariantTypeMismatch_243"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    if let syn::Fields::Named(fields) = &mut variant.fields {
                        for field in &mut fields.named {
                            if let Some(ty) = &mut field.ty {
                                let new_ty = match ty {
                                    Type::Reference(ref ty) => {
                                        Type::Reference(TypeReference {
                                            and_token: ty.and_token,
                                            lifetime: Some(Lifetime {
                                                apostrophe: token::Lifetime::new("static", Span::call_site()),
                                                ident: Ident::new("static", Span::call_site()),
                                            }),
                                            mutability: ty.mutability,
                                            elem: Box::new(Type::Tuple(TypeTuple {
                                                paren_token: Paren::default(),
                                                elems: Punctuated::new(),
                                            })),
                                        })
                                    }
                                    _ => {
                                        Type::Tuple(TypeTuple {
                                            paren_token: Paren::default(),
                                            elems: Punctuated::new(),
                                        })
                                    }
                                };
                                *ty = new_ty;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum variants with type annotations and introduces a type mismatch between the declared type of the variant and the type used in a pattern match. This transformation aims to test the compiler's ability to handle type checking and pattern matching on enum variants with complex types."
    }
}
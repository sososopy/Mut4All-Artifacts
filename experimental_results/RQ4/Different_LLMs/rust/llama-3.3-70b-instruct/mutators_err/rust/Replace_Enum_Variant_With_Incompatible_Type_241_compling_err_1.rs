use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Replace_Enum_Variant_With_Incompatible_Type_241;

impl Mutator for Replace_Enum_Variant_With_Incompatible_Type_241 {
    fn name(&self) -> &str {
        "Replace_Enum_Variant_With_Incompatible_Type_241"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(enum_item) = item {
                for variant in &mut enum_item.variants {
                    if let Some((_, ref mut fields)) = variant.fields {
                        for field in fields {
                            if let Type::Reference(ref_type) = *field.ty {
                                let incompatible_type = if thread_rng().gen_bool(0.5) {
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("String", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    })
                                } else {
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("i32", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    })
                                };
                                field.ty = Box::new(incompatible_type);
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces enum variants with incompatible types to trigger potential bugs in the Rust compiler's pattern matching and type checking mechanisms."
    }
}
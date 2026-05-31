use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemEnum, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Enum_Lifetime_Mismatch_364;

impl Mutator for Enum_Lifetime_Mismatch_364 {
    fn name(&self) -> &str {
        "Enum_Lifetime_Mismatch_364"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_lifetime_variant = false;
                for variant in &item_enum.variants {
                    if let Fields::Named(fields_named) = &variant.fields {
                        for field in &fields_named.named {
                            if let Type::Ptr(type_ptr) = &field.ty {
                                if let Type::Reference(type_ref) = &*type_ptr.elem {
                                    if type_ref.lifetime.is_some() {
                                        has_lifetime_variant = true;
                                    }
                                }
                            }
                        }
                    }
                }

                if has_lifetime_variant {
                    for item in &mut file.items {
                        if let Item::Fn(item_fn) = item {
                            for stmt in &mut item_fn.block.stmts {
                                if let Stmt::Expr(Expr::Match(expr_match), _) = stmt {
                                    for arm in &mut expr_match.arms {
                                        if let Pat::TupleStruct(pat_tuple_struct) = &mut arm.pat {
                                            if pat_tuple_struct.path.segments.len() == 1 {
                                                let segment = &mut pat_tuple_struct.path.segments[0];
                                                if segment.ident == item_enum.ident {
                                                    segment.ident = Ident::new("NonExistentVariant", segment.ident.span());
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enum variants with lifetimes. It identifies enums with variants that include pointer or reference types with lifetimes. It then mutates pattern matching statements to introduce a type or lifetime mismatch by changing the matched pattern to a non-existent variant. This exploits potential mismanagement of lifetimes and pattern matching in Rust, which can lead to internal compiler errors or unexpected behavior."
    }
}
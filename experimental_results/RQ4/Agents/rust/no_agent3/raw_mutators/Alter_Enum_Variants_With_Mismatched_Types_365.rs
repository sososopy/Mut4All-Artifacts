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

pub struct Alter_Enum_Variants_With_Mismatched_Types_365;

impl Mutator for Alter_Enum_Variants_With_Mismatched_Types_365 {
    fn name(&self) -> &str {
        "Alter_Enum_Variants_With_Mismatched_Types_365"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut has_pattern_match = false;
                let mut new_variant_name = None;

                for variant in &item_enum.variants {
                    if let Some((_, fields)) = &variant.fields {
                        if fields.len() > 0 {
                            has_pattern_match = true;
                        }
                    }
                }

                if has_pattern_match {
                    let new_variant_ident = Ident::new("MismatchedVariant", Span::call_site());
                    item_enum.variants.push(parse_quote! {
                        #new_variant_ident(isize, isize)
                    });
                    new_variant_name = Some(new_variant_ident);
                }

                if let Some(new_variant_name) = new_variant_name {
                    for item in &mut file.items {
                        if let Item::Fn(item_fn) = item {
                            for stmt in &mut item_fn.block.stmts {
                                if let Stmt::Expr(Expr::Match(expr_match)) = stmt {
                                    for arm in &mut expr_match.arms {
                                        if let Pat::Path(pat_path) = &arm.pat {
                                            if pat_path.path.segments.last().unwrap().ident == item_enum.ident {
                                                arm.pat = parse_quote! { #new_variant_name(x) };
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
        "The mutation operator identifies enum definitions with multiple variants used in pattern matching. It introduces a new variant with a deliberately mismatched arity and modifies a pattern match arm to use this new variant, creating an arity mismatch."
    }
}
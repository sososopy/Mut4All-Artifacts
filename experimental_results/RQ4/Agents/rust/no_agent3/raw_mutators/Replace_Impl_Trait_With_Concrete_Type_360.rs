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

pub struct Replace_Impl_Trait_With_Concrete_Type_360;

impl Mutator for Replace_Impl_Trait_With_Concrete_Type_360 {
    fn name(&self) -> &str {
        "Replace_Impl_Trait_With_Concrete_Type_360"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = Vec::new();

        for item in &mut file.items {
            if let Item::Type(type_item) = item {
                if let Type::ImplTrait(_) = *type_item.ty {
                    type_aliases.push(type_item.ident.clone());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, ty)) = &local.ty {
                            if let Type::Path(type_path) = &**ty {
                                if type_aliases.contains(&type_path.path.segments[0].ident) {
                                    *local.ty.as_mut().unwrap().1 = Box::new(parse_quote!(Option<(u32, u32)>));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies type alias declarations using `impl Trait` and replaces their usage in variable declarations with a concrete type. This ensures that any match expressions using the previous variable correctly pattern against the new concrete type, revealing discrepancies in pattern matching and type handling."
    }
}
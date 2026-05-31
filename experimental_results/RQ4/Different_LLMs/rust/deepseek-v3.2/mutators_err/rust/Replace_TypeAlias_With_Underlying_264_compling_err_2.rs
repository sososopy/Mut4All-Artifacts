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

pub struct Replace_TypeAlias_With_Underlying_264;

impl Mutator for Replace_TypeAlias_With_Underlying_264 {
    fn name(&self) -> &str {
        "Replace_TypeAlias_With_Underlying_264"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = Vec::new();
        for item in &file.items {
            if let Item::Type(item_type) = item {
                type_aliases.push((item_type.ident.clone(), item_type.ty.clone()));
            }
        }
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Path(type_path) = &*field.ty {
                        let path_segments = &type_path.path.segments;
                        if let Some(first_seg) = path_segments.first() {
                            let alias_name = &first_seg.ident;
                            for (alias_ident, alias_ty) in &type_aliases {
                                if alias_name == alias_ident {
                                    if let Type::Path(alias_type_path) = alias_ty {
                                        let mut new_path = alias_type_path.path.clone();
                                        if let PathArguments::AngleBracketed(args) = &first_seg.arguments {
                                            if let PathArguments::AngleBracketed(alias_args) = &new_path.segments.last().unwrap().arguments {
                                                let mut substituted_args = alias_args.clone();
                                                for (i, arg) in substituted_args.args.iter_mut().enumerate() {
                                                    if let GenericArgument::Type(Type::Path(arg_path)) = arg {
                                                        if arg_path.path.segments.len() == 1 && arg_path.path.segments[0].ident == "U" {
                                                            *arg = args.args[i].clone();
                                                        }
                                                    }
                                                }
                                                new_path.segments.last_mut().unwrap().arguments = PathArguments::AngleBracketed(substituted_args);
                                            }
                                        }
                                        field.ty = Box::new(Type::Path(TypePath { qself: None, path: new_path }));
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
        "The mutation operator replaces a type alias used in a struct field within a generic context with its underlying type definition. It targets struct definitions where a field's type is a type alias that expands to a parameterized type involving associated types from a trait bound. Specifically, it applies when a struct has a generic parameter with a trait bound that includes an associated type, and a field in that struct uses a type alias that references that associated type indirectly through nested generic type expansions. The mutation substitutes the alias with its full expanded type, preserving all generic arguments and associated type projections. For example, if the seed contains a struct like `struct Container<T: Trait> { field: Alias<T::Assoc> }` where `type Alias<U> = Outer<Inner<U>>;`, then mutate to `struct Container<T: Trait> { field: Outer<Inner<T::Assoc>> }`. This must be done only if the alias is directly used in a field type and the underlying type is a valid substitution without introducing new names. The goal is to create complex nested type projections that may stress the trait solver during borrow checking or type resolution. Ensure that any generic parameters from the alias definition are properly substituted with the actual arguments from the alias usage. Avoid creating undeclared types; only use types already present in the seed. This mutation aims to expose bugs in the trait system's handling of associated types within deeply nested generic structures, particularly under borrow checking."
    }
}
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

pub struct InherentAssociatedTypeSubstitution_416;

impl Mutator for InherentAssociatedTypeSubstitution_416 {
    fn name(&self) -> &str {
        "InherentAssociatedTypeSubstitution_416"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_associated_types = Vec::new();

        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Type::Path(type_path) = &*impl_item.self_ty {
                    if let Some(struct_name) = type_path.path.get_ident().cloned() {
                        for impl_item in &mut impl_item.items {
                            if let syn::ImplItem::Type(type_alias) = impl_item {
                                let associated_type_name = type_alias.ident.clone();
                                struct_associated_types.push((struct_name.clone(), associated_type_name));
                            }
                        }
                    }
                }
            }
        }

        if let Some((struct_name, associated_type_name)) = struct_associated_types.first() {
            for item in &mut file.items {
                if let Item::Fn(func) = item {
                    for arg in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = arg {
                            if let Type::Reference(ref_type) = &*pat_type.ty {
                                let referenced_type = &*ref_type.elem;
                                let new_type = build_function_pointer_type(
                                    &struct_name,
                                    &associated_type_name,
                                    referenced_type,
                                );
                                *pat_type.ty = new_type;
                            }
                        }
                    }

                    if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                        if let Type::Reference(ref_type) = &**return_type {
                            let referenced_type = &*ref_type.elem;
                            let new_type = build_function_pointer_type(
                                &struct_name,
                                &associated_type_name,
                                referenced_type,
                            );
                            *return_type = new_type;
                        }
                    }
                }

                if let Item::Impl(impl_item) = item {
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(method) = impl_item {
                            for arg in &mut method.sig.inputs {
                                if let FnArg::Typed(pat_type) = arg {
                                    if let Type::Reference(ref_type) = &*pat_type.ty {
                                        let referenced_type = &*ref_type.elem;
                                        let new_type = build_function_pointer_type(
                                            &struct_name,
                                            &associated_type_name,
                                            referenced_type,
                                        );
                                        *pat_type.ty = new_type;
                                    }
                                }
                            }

                            if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                                if let Type::Reference(ref_type) = &**return_type {
                                    let referenced_type = &*ref_type.elem;
                                    let new_type = build_function_pointer_type(
                                        &struct_name,
                                        &associated_type_name,
                                        referenced_type,
                                    );
                                    *return_type = new_type;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces reference types in function parameters and return positions with higher-ranked function pointers that return an associated type of a struct. This transformation introduces complex lifetime relationships and forces the compiler to resolve associated types through function pointers, potentially exposing bugs in trait resolution, lifetime inference, or higher-ranked trait bounds handling."
    }
}

fn build_function_pointer_type(
    struct_name: &Ident,
    associated_type_name: &Ident,
    referenced_type: &Type,
) -> Type {
    parse_quote! {
        for<'a> fn(&'a #referenced_type) -> #struct_name<'a>::#associated_type_name
    }
}
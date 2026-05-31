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

pub struct Convert_Struct_To_Union_With_Trait_Ptr_332;

impl Mutator for Convert_Struct_To_Union_With_Trait_Ptr_332 {
    fn name(&self) -> &str {
        "Convert_Struct_To_Union_With_Trait_Ptr_332"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut target_struct_name: Option<Ident> = None;
        let mut struct_found = false;

        // Step 1: Convert the first struct to a union
        for item in &mut file.items {
            if let Item::Struct(s) = item && !struct_found {
                let original_name = s.ident.clone();
                target_struct_name = Some(original_name);

                // Convert to union
                let token = syn::token::Union::new1(s.struct_token.span());
                let union = ItemUnion {
                    attrs: s.attrs.clone(),
                    vis: s.vis.clone(),
                    union_token: Some(token),
                    ident: s.ident.clone(),
                    generics: s.generics.clone(),
                    fields: match s.fields.clone() {
                        syn::Fields::Named(named) => named,
                        _ => panic!("Expected named fields in struct"),
                    },
                };
                *item = Item::Union(union);
                struct_found = true;
                break;
            }
        }

        // Step 2: Add impl ToString for the union
        if let Some(name) = &target_struct_name {
            let impl_block = parse_quote! {
                impl ToString for #name {
                    fn to_string(&self) -> String {
                        unimplemented!()
                    }
                }
            };
            file.items.push(Item::Impl(impl_block));
        }

        // Step 3: Replace function parameters of the original struct with *const dyn ToString
        if let Some(name) = &target_struct_name {
            for item in &mut file.items {
                if let Item::Fn(func) = item {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                if type_path.path.is_ident(name) {
                                    // Replace with *const dyn ToString
                                    let new_type = parse_quote!(*const dyn ToString);
                                    pat_type.ty = Box::new(new_type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator converts a struct into a union, implements a trait for the union, and replaces function parameters expecting the struct with raw trait pointers. This transformation introduces unions and trait object pointers, challenging the compiler's handling of union types, trait resolution, and raw pointer interactions, potentially leading to ICEs in complex type inference or trait object scenarios."
    }
}
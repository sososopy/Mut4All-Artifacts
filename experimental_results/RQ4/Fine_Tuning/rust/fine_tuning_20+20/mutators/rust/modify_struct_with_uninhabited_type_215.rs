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

pub struct Modify_Struct_With_Uninhabited_Type_215;

impl Mutator for Modify_Struct_With_Uninhabited_Type_215 {
    fn name(&self) -> &str {
        "Modify_Struct_With_Uninhabited_Type_215"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut additional_items = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_uninhabited = false;
                for field in &item_struct.fields {
                    if let Type::Never(_) = &field.ty {
                        has_uninhabited = true;
                        break;
                    }
                }
                if has_uninhabited {
                    let new_fields = item_struct
                        .fields
                        .iter()
                        .map(|field| {
                            if let Type::Never(_) = &field.ty {
                                let new_field: syn::Field = parse_quote! { y: u8 };
                                new_field
                            } else {
                                field.clone()
                            }
                        })
                        .collect::<Vec<_>>();
                    item_struct.fields = syn::Fields::Named(syn::FieldsNamed {
                        brace_token: syn::token::Brace::default(),
                        named: syn::punctuated::Punctuated::from_iter(new_fields),
                    });
                    let struct_name = &item_struct.ident;
                    let use_function: syn::ItemFn = parse_quote! {
                        fn use_struct() {
                            let instance = #struct_name { x: 42, y: 1 };
                            println!("{:?}", instance);
                        }
                    };
                    additional_items.push(syn::Item::Fn(use_function));
                }
            }
        }
        file.items.extend(additional_items);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with uninhabited types (e.g., `!`) and replaces them with a concrete type (`u8`). It then adds a function to the file that creates an instance of the modified struct and prints it, ensuring the struct is actively used in the program. This transformation increases the likelihood of triggering meaningful compiler feedback by converting previously unconstructible types into concrete, usable forms."
    }
}
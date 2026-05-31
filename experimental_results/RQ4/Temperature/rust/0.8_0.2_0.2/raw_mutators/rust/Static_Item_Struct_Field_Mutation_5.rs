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

pub struct Static_Item_Struct_Field_Mutation_5;

impl Mutator for Static_Item_Struct_Field_Mutation_5 {
    fn name(&self) -> &str {
        "Static_Item_Struct_Field_Mutation_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Type::Path(TypePath { path, .. }) = &*item_static.ty {
                    if let Some(last_segment) = path.segments.last() {
                        if let Some(Item::Struct(item_struct)) = file.items.iter_mut().find(|i| {
                            if let Item::Struct(s) = i {
                                s.ident == last_segment.ident
                            } else {
                                false
                            }
                        }) {
                            // Swap field types
                            if item_struct.fields.len() >= 2 {
                                let mut fields = item_struct.fields.iter_mut().collect::<Vec<_>>();
                                fields.swap(0, 1);
                            }

                            // Remove a field from static initialization
                            if let Expr::Struct(expr_struct) = &mut *item_static.expr {
                                if !expr_struct.fields.is_empty() {
                                    expr_struct.fields.pop();
                                }
                            }

                            // Introduce a new field in struct definition
                            let new_field: syn::Field = parse_quote! {
                                pub new_field: String
                            };
                            item_struct.fields.push(new_field);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets static items initialized with structs. It modifies struct field initialization by swapping field types, removing a field from the static initialization, and introducing a new field in the struct definition. This aims to induce type mismatches and incomplete initializations, testing the compiler's handling of struct field changes and const evaluation."
    }
}
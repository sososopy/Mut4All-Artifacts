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

pub struct Swap_Const_Generic_Struct_Fields_88;

impl Mutator for Swap_Const_Generic_Struct_Fields_88 {
    fn name(&self) -> &str {
        "Swap_Const_Generic_Struct_Fields_88"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_def) = item {
                // Check if struct has const generics
                let has_const_generics = struct_def.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Const(_))
                });
                if !has_const_generics {
                    continue;
                }

                // Check if struct is in an unsafe context (simplified to check if inside a union)
                let in_unsafe_context = false; // Placeholder for actual context check

                // Process the fields
                let mut swapped = false;
                if let syn::Fields::Named(ref mut named) = struct_def.fields {
                    let fields = &mut named.named;
                    for i in 0..fields.len() - 1 {
                        let field1 = &fields[i];
                        let field2 = &fields[i + 1];

                        // Check if both are arrays
                        if let Type::Array(array1) = &field1.ty {
                            if let Type::Array(array2) = &field2.ty {
                                // Compare the array lengths
                                let expr1_str = array1.len.to_token_stream().to_string();
                                let expr2_str = array2.len.to_token_stream().to_string();
                                if expr1_str != expr2_str {
                                    // Swap the fields
                                    fields.swap(i, i + 1);
                                    swapped = true;
                                    break; // Swap only the first pair found
                                }
                            }
                        }
                    }

                    // If no swap happened, check for non-ZST types
                    if !swapped {
                        for i in 0..fields.len() - 1 {
                            let field1 = &fields[i];
                            let field2 = &fields[i + 1];

                            // Check if both are non-ZST types (not unit)
                            let is_unit = |ty: &Type| {
                                matches!(ty, Type::Tuple(tuple) if tuple.elems.is_empty())
                            };
                            if !is_unit(&field1.ty) && !is_unit(&field2.ty) {
                                // Swap them
                                fields.swap(i, i + 1);
                                swapped = true;
                                break;
                            }
                        }
                    }
                } else if let syn::Fields::Unnamed(ref mut unnamed) = struct_def.fields {
                    let fields = &mut unnamed.unnamed;
                    for i in 0..fields.len() - 1 {
                        let field1 = &fields[i];
                        let field2 = &fields[i + 1];

                        // Check if both are arrays
                        if let Type::Array(array1) = &field1.ty {
                            if let Type::Array(array2) = &field2.ty {
                                // Compare the array lengths
                                let expr1_str = array1.len.to_token_stream().to_string();
                                let expr2_str = array2.len.to_token_stream().to_string();
                                if expr1_str != expr2_str {
                                    // Swap the fields
                                    fields.swap(i, i + 1);
                                    swapped = true;
                                    break; // Swap only the first pair found
                                }
                            }
                        }
                    }

                    // If no swap happened, check for non-ZST types
                    if !swapped {
                        for i in 0..fields.len() - 1 {
                            let field1 = &fields[i];
                            let field2 = &fields[i + 1];

                            // Check if both are non-ZST types (not unit)
                            let is_unit = |ty: &Type| {
                                matches!(ty, Type::Tuple(tuple) if tuple.elems.is_empty())
                            };
                            if !is_unit(&field1.ty) && !is_unit(&field2.ty) {
                                // Swap them
                                fields.swap(i, i + 1);
                                swapped = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}
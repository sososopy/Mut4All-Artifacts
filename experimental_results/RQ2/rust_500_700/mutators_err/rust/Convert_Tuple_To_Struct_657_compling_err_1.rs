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

pub struct Convert_Tuple_To_Struct_657;

impl Mutator for Convert_Tuple_To_Struct_657 {
    fn name(&self) -> &str {
        "Convert_Tuple_To_Struct_657"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_structs = Vec::new();
        let mut replacements = Vec::new();

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut input_struct_name = None;
                let mut output_struct_name = None;

                // Process function parameters
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Tuple(type_tuple) = &*pat_type.ty {
                            let struct_name = Ident::new(&format!("InputData{}", new_structs.len()), Span::call_site());
                            input_struct_name = Some(struct_name.clone());
                            let fields: Punctuated<_, Comma> = type_tuple.elems.iter().enumerate().map(|(i, ty)| {
                                let field_name = Ident::new(&format!("field_{}", i), Span::call_site());
                                parse_quote!(#field_name: #ty)
                            }).collect();

                            let new_struct: ItemStruct = parse_quote! {
                                struct #struct_name {
                                    #fields
                                }
                            };
                            new_structs.push(new_struct);
                            pat_type.ty = Box::new(parse_quote!(#struct_name));
                        }
                    }
                }

                // Process function return type
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::Tuple(type_tuple) = &**return_type {
                        let struct_name = Ident::new(&format!("OutputData{}", new_structs.len()), Span::call_site());
                        output_struct_name = Some(struct_name.clone());
                        let fields: Punctuated<_, Comma> = type_tuple.elems.iter().enumerate().map(|(i, ty)| {
                            let field_name = Ident::new(&format!("field_{}", i), Span::call_site());
                            parse_quote!(#field_name: #ty)
                        }).collect();

                        let new_struct: ItemStruct = parse_quote! {
                            struct #struct_name {
                                #fields
                            }
                        };
                        new_structs.push(new_struct);
                        *return_type = Box::new(parse_quote!(#struct_name));
                    }
                }

                // Prepare replacements for function calls
                if input_struct_name.is_some() || output_struct_name.is_some() {
                    replacements.push((item_fn.sig.ident.clone(), input_struct_name, output_struct_name));
                }
            }
        }

        // Insert new structs at the beginning of the file
        file.items.splice(0..0, new_structs.into_iter().map(Item::Struct));

        // Update function calls
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let visitor = |expr: &mut Expr| {
                    if let Expr::Call(expr_call) = expr {
                        if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                            for (func_ident, input_struct_name, output_struct_name) in &replacements {
                                if path.is_ident(func_ident) {
                                    if let Some(input_struct_name) = input_struct_name {
                                        if let Some(first_arg) = expr_call.args.first_mut() {
                                            if let Expr::Tuple(expr_tuple) = first_arg {
                                                let fields: Punctuated<_, Comma> = expr_tuple.elems.iter().enumerate().map(|(i, expr)| {
                                                    let field_name = Ident::new(&format!("field_{}", i), Span::call_site());
                                                    parse_quote!(#field_name: #expr)
                                                }).collect();

                                                *first_arg = parse_quote!(#input_struct_name { #fields });
                                            }
                                        }
                                    }
                                    if let Some(output_struct_name) = output_struct_name {
                                        if let Some(last_stmt) = item_fn.block.stmts.last_mut() {
                                            if let Stmt::Expr(expr) = last_stmt {
                                                if let Expr::Tuple(expr_tuple) = expr {
                                                    let fields: Punctuated<_, Comma> = expr_tuple.elems.iter().enumerate().map(|(i, expr)| {
                                                        let field_name = Ident::new(&format!("field_{}", i), Span::call_site());
                                                        parse_quote!(#field_name: #expr)
                                                    }).collect();

                                                    *expr = parse_quote!(#output_struct_name { #fields });
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                };
                syn::visit_mut::visit_expr_mut(&mut visitor, &mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions with tuple parameters or return types, converts them into structs, and updates the function signatures and calls accordingly. It tests the compiler's handling of struct initialization and tuple-struct compatibility, potentially revealing issues with type resolution and data structure transformations."
    }
}
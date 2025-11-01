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

pub struct Array_Size_Mismatch_In_Struct_346;

impl Mutator for Array_Size_Mismatch_In_Struct_346 {
    fn name(&self) -> &str {
        "Array_Size_Mismatch_In_Struct_346"
    }

    fn mutate(&self, file: &mut syn::File) {
        struct StructVisitor {
            changes: Vec<(Span, usize)>,
        }

        impl<'ast> Visit<'ast> for StructVisitor {
            fn visit_item_struct(&mut self, i: &'ast ItemStruct) {
                if let Fields::Unnamed(fields) = &i.fields {
                    for field in &fields.unnamed {
                        if let Type::Array(type_array) = &field.ty {
                            let new_size = match &type_array.len {
                                Expr::Lit(expr_lit) => {
                                    if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                        lit_int.base10_parse::<usize>().unwrap_or(0) + 1
                                    } else {
                                        continue;
                                    }
                                }
                                _ => continue,
                            };
                            self.changes.push((i.span(), new_size));
                        }
                    }
                }
                syn::visit::visit_item_struct(self, i);
            }
        }

        let mut visitor = StructVisitor { changes: Vec::new() };
        visitor.visit_file(file);

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for (span, new_size) in &visitor.changes {
                    if item_struct.span().eq(span) {
                        if let Fields::Unnamed(fields) = &mut item_struct.fields {
                            for field in &mut fields.unnamed {
                                if let Type::Array(type_array) = &mut field.ty {
                                    type_array.len = parse_quote!(#new_size);
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.is_ident("Default") {
                        for item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = item {
                                if method.sig.ident == "default" {
                                    for stmt in &mut method.block.stmts {
                                        if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                                            if let Expr::Path(expr_path) = &*expr_call.func {
                                                if expr_path.path.segments.last().unwrap().ident
                                                    == "Foo"
                                                {
                                                    for arg in &mut expr_call.args {
                                                        if let Expr::Array(expr_array) = arg {
                                                            let new_size = visitor.changes.iter().find_map(|(span, size)| {
                                                                if item_impl.self_ty.span().eq(span) {
                                                                    Some(size)
                                                                } else {
                                                                    None
                                                                }
                                                            });

                                                            if let Some(&new_size) = new_size {
                                                                expr_array.elems = expr_array.elems.iter().cloned().take(new_size).collect();
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
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with array fields, altering the array size in the struct definition. It then checks for implementations of the Default trait and modifies the array initialization within the default method, creating a mismatch between the struct definition and the method implementation. This inconsistency is designed to trigger potential compiler bugs during type checking or MIR optimizations."
    }
}
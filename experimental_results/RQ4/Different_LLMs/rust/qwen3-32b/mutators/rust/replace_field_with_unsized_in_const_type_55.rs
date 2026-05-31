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

pub struct Replace_Field_With_Unsized_In_Const_Type_55;

impl Mutator for Replace_Field_With_Unsized_In_Const_Type_55 {
    fn name(&self) -> &str {
        "Replace_Field_With_Unsized_In_Const_Type_55"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut type_set = HashSet::new();
        
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(call_expr) = expr {
                            if let Expr::Path(path_expr) = &*call_expr.func {
                                let path = &path_expr.path;
                                let mut segments = path.segments.iter();
                                if let Some(seg) = segments.next() {
                                    if seg.ident == "core" {
                                        if let Some(seg) = segments.next() {
                                            if seg.ident == "mem" {
                                                if let Some(seg) = segments.next() {
                                                    if seg.ident == "size_of" || seg.ident == "align_of" {
                                                        if let PathArguments::AngleBracketed(args) = &seg.arguments {
                                                            if let Some(arg) = args.args.first() {
                                                                if let GenericArgument::Type(ty) = arg {
                                                                    if let Type::Path(type_path) = &*ty {
                                                                        if let Some(segment) = type_path.path.segments.last() {
                                                                            let type_name = segment.ident.to_string();
                                                                            type_set.insert(type_name);
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
            }
        }

        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if type_set.contains(&struct_item.ident.to_string()) {
                    if let Fields::Named(fields) = &mut struct_item.fields {
                        if let Some(first_field) = fields.named.iter_mut().next() {
                            first_field.ty = Type::Path(TypePath {
                                qself: None,
                                path: parse_quote! { str },
                            });
                        }
                    }
                }
            } else if let Item::Enum(enum_item) = item {
                if type_set.contains(&enum_item.ident.to_string()) {
                    if let Some(variant) = enum_item.variants.iter_mut().next() {
                        if let Fields::Named(fields) = &mut variant.fields {
                            if let Some(first_field) = fields.named.iter_mut().next() {
                                first_field.ty = Type::Path(TypePath {
                                    qself: None,
                                    path: parse_quote! { str },
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs or enums used in const contexts like size_of/align_of and replaces one of their fields with an unsized type (e.g., str). This creates a scenario where the compiler's const evaluator may encounter an ICE when processing unsized types in generic arguments, testing its ability to handle type unsizing in const contexts."
    }
}
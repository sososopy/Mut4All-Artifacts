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

pub struct Replace_Constant_Expression_With_Non_Constant_Field_Access_51;

impl Mutator for Replace_Constant_Expression_With_Non_Constant_Field_Access_51 {
    fn name(&self) -> &str {
        "Replace_Constant_Expression_With_Non_Constant_Field_Access_51"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let syn::Field::Named(field_named) = field {
                        if let Some(expr) = get_constant_expression(&field_named.ty) {
                            if let Some(non_constant_field_access) = get_non_constant_field_access(&struct_item, &field_named) {
                                field_named.ty = non_constant_field_access;
                            }
                        }
                    }
                }
            }
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Array(array) = &**expr {
                            for element in &mut array.elems {
                                if let Expr::Lit(lit) = &**element {
                                    if let Some(non_constant_field_access) = get_non_constant_field_access_for_lit(&func, lit) {
                                        *element = non_constant_field_access;
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
        "The mutation operator replaces constant expressions with non-constant field access. It targets constant expressions used in array or struct initializations and replaces them with non-constant field access on a variable of the same type. This transformation aims to test the compiler's handling of non-constant expressions and field access."
    }
}

fn get_constant_expression(ty: &syn::Type) -> Option<syn::Expr> {
    if let syn::Type::Path(type_path) = ty {
        if let Some(segment) = type_path.path.segments.last() {
            if segment.ident == "u32" || segment.ident == "i32" || segment.ident == "f32" || segment.ident == "f64" {
                return Some(parse_quote! { 0 });
            }
        }
    }
    None
}

fn get_non_constant_field_access(struct_item: &syn::ItemStruct, field_named: &syn::FieldNamed) -> Option<syn::Type> {
    if let Some(field) = struct_item.fields.iter().find(|f| f.ident == field_named.ident) {
        if let syn::Field::Named(field_named) = field {
            return Some(parse_quote! { self.#field_named.ident });
        }
    }
    None
}

fn get_non_constant_field_access_for_lit(func: &syn::ItemFn, lit: &syn::Lit) -> Option<syn::Expr> {
    if let syn::Lit::Int(int) = lit {
        let var_name = Ident::new(&format!("var_{}", int.base10_digits()), Span::call_site());
        let var_decl = parse_quote! { let #var_name = 0; };
        func.block.stmts.insert(0, Stmt::Local(parse_quote! { #var_decl }));
        return Some(parse_quote! { #var_name });
    }
    None
}
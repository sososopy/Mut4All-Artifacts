use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprStruct, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct NestedConstADTInsertion_290;

impl Mutator for NestedConstADTInsertion_290 {
    fn name(&self) -> &str {
        "NestedConstADTInsertion_290"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_info = Vec::new();
        for item in &file.items {
            if let Item::Struct(s) = item {
                let is_tuple = match &s.fields {
                    syn::Fields::Named(_) => false,
                    syn::Fields::Unnamed(_) => true,
                    syn::Fields::Unit => false,
                };
                let fields = match &s.fields {
                    syn::Fields::Named(named) => named.named.iter().map(|f| f.ty.clone()).collect(),
                    syn::Fields::Unnamed(unnamed) => unnamed.unnamed.iter().map(|f| f.ty.clone()).collect(),
                    syn::Fields::Unit => vec![],
                };
                let field_names = match &s.fields {
                    syn::Fields::Named(named) => Some(named.named.iter().map(|f| f.ident.as_ref().unwrap().clone()).collect()),
                    _ => None,
                };
                struct_info.push(StructInfo { name: s.ident.clone(), is_tuple, fields, field_names });
            }
            if let Item::Enum(e) = item {
                for variant in &e.variants {
                    let is_tuple = match &variant.fields {
                        syn::Fields::Named(_) => false,
                        syn::Fields::Unnamed(_) => true,
                        syn::Fields::Unit => false,
                    };
                    let fields = match &variant.fields {
                        syn::Fields::Named(named) => named.named.iter().map(|f| f.ty.clone()).collect(),
                        syn::Fields::Unnamed(unnamed) => unnamed.unnamed.iter().map(|f| f.ty.clone()).collect(),
                        syn::Fields::Unit => vec![],
                    };
                    let field_names = match &variant.fields {
                        syn::Fields::Named(named) => Some(named.named.iter().map(|f| f.ident.as_ref().unwrap().clone()).collect()),
                        _ => None,
                    };
                    struct_info.push(StructInfo { name: variant.ident.clone(), is_tuple, fields, field_names });
                }
            }
        }

        let mut visitor = NestedADTVisitor { struct_info: &struct_info };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces simple const ADT expressions in struct/enum instantiations with deeply nested ADT values. It selects existing structs/enums from the program and constructs nested instances, forcing the compiler to evaluate complex type hierarchies in const parameters. This stresses the interaction between ADT layout computation and const param validation, potentially exposing bugs in type resolution and const evaluation."
    }
}

struct StructInfo {
    name: Ident,
    is_tuple: bool,
    fields: Vec<Type>,
    field_names: Option<Vec<Ident>>,
}

struct NestedADTVisitor<'a> {
    struct_info: &'a Vec<StructInfo>,
}

impl<'a> VisitMut for NestedADTVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        syn::visit_mut::visit_expr_mut(self, expr);

        if let Expr::Struct(expr_struct) = expr {
            for segment in &mut expr_struct.path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(const_arg) = arg {
                            self.replace_expr(const_arg);
                        }
                    }
                }
            }
        }
    }
}

impl<'a> NestedADTVisitor<'a> {
    fn replace_expr(&self, expr: &mut Expr) {
        let mut rng = thread_rng();
        if let Some(struct_info) = self.struct_info.choose(&mut rng) {
            let new_expr = self.create_struct_instance(struct_info);
            *expr = new_expr;
        }
    }

    fn create_struct_instance(&self, struct_info: &StructInfo) -> Expr {
        if struct_info.is_tuple {
            let mut fields: Punctuated<Expr, Comma> = Punctuated::new();
            for field_type in &struct_info.fields {
                let default_expr = self.generate_default_expr(field_type);
                fields.push(Expr::from(default_expr));
            }
            parse_quote! { #struct_info.name(#fields) }
        } else {
            let mut fields: Punctuated<Expr, Comma> = Punctuated::new();
            if let Some(field_names) = &struct_info.field_names {
                for (field_name, field_type) in field_names.iter().zip(&struct_info.fields) {
                    let default_expr = self.generate_default_expr(field_type);
                    fields.push(parse_quote! { #field_name: #default_expr });
                }
            }
            parse_quote! { #struct_info.name { #fields } }
        }
    }

    fn generate_default_expr(&self, field_type: &Type) -> Expr {
        match field_type {
            Type::Path(type_path) => {
                if let Some(segment) = type_path.path.segments.last() {
                    match segment.ident.to_string().as_str() {
                        "i32" | "isize" | "u32" | "usize" => parse_quote! { 0 },
                        "bool" => parse_quote! { true },
                        "char" => parse_quote! { 'a' },
                        "str" => parse_quote! { "default" },
                        _ => {
                            if let Some(struct_info) = self.struct_info.iter().find(|s| s.name == segment.ident) {
                                self.create_struct_instance(struct_info)
                            } else {
                                parse_quote! { () }
                            }
                        }
                    }
                } else {
                    parse_quote! { () }
                }
            },
            _ => parse_quote! { () },
        }
    }
}
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

pub struct Struct_Field_Type_Change_68;

impl Mutator for Struct_Field_Type_Change_68 {
    fn name(&self) -> &str {
        "Struct_Field_Type_Change_68"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_names = Vec::new();
        let mut field_map = HashMap::new();
        let mut available_types = HashSet::new();
        for item in &file.items {
            match item {
                Item::Struct(item_struct) => {
                    struct_names.push(item_struct.ident.clone());
                    let mut fields = Vec::new();
                    for field in &item_struct.fields {
                        if let Some(ident) = &field.ident {
                            fields.push(ident.clone());
                        }
                        available_types.insert(field.ty.clone());
                    }
                    field_map.insert(item_struct.ident.clone(), fields);
                }
                Item::Enum(item_enum) => {
                    available_types.insert(parse_quote! { #item_enum.ident });
                }
                Item::Type(item_type) => {
                    available_types.insert(parse_quote! { #item_type.ident });
                }
                Item::Fn(item_fn) => {
                    if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                        available_types.insert(*return_type.clone());
                    }
                }
                _ => {}
            }
        }
        if struct_names.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let struct_name = struct_names.choose(&mut rng).unwrap();
        let field_names = field_map.get(struct_name).unwrap();
        if field_names.is_empty() {
            return;
        }
        let field_name = field_names.choose(&mut rng).unwrap();
        let mut target_type = None;
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == *struct_name {
                    for field in &item_struct.fields {
                        if let Some(ident) = &field.ident {
                            if ident == field_name {
                                target_type = Some(field.ty.clone());
                            }
                        }
                    }
                }
            }
        }
        if target_type.is_none() {
            return;
        }
        let target_type = target_type.unwrap();
        let mut candidate_types: Vec<_> = available_types
            .iter()
            .filter(|ty| !ty.to_token_stream().to_string().contains("fn"))
            .cloned()
            .collect();
        candidate_types.sort_by_key(|ty| {
            if ty.to_token_stream().to_string().contains("Vec") {
                0
            } else if ty.to_token_stream().to_string().contains("Option") {
                1
            } else {
                2
            }
        });
        let new_type = candidate_types
            .into_iter()
            .find(|ty| ty.to_token_stream().to_string() != target_type.to_token_stream().to_string())
            .unwrap_or_else(|| parse_quote! { i32 });
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == *struct_name {
                    for field in &mut item_struct.fields {
                        if let Some(ident) = &field.ident {
                            if ident == field_name {
                                field.ty = new_type.clone();
                            }
                        }
                    }
                }
            }
        }
        struct FieldTypeReplacer {
            struct_name: Ident,
            field_name: Ident,
            old_type: Type,
            new_type: Type,
        }
        impl VisitMut for FieldTypeReplacer {
            fn visit_expr_mut(&mut self, node: &mut Expr) {
                if let Expr::Struct(expr_struct) = node {
                    if expr_struct.path.is_ident(&self.struct_name) {
                        for field in &mut expr_struct.fields {
                            if field.member == self.field_name {
                                field.expr = parse_quote! { #field.expr as #new_type };
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, node);
            }
            fn visit_local_mut(&mut self, node: &mut Local) {
                if let Some((_, expr)) = &mut node.init {
                    if let Expr::Struct(expr_struct) = &**expr {
                        if expr_struct.path.is_ident(&self.struct_name) {
                            for field in &expr_struct.fields {
                                if field.member == self.field_name {
                                    node.ty = Some((token::Colon::default(), self.new_type.clone()));
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_local_mut(self, node);
            }
            fn visit_expr_field_mut(&mut self, node: &mut ExprField) {
                if let Expr::Field(expr_field) = node {
                    if let Expr::Path(expr_path) = &*expr_field.base {
                        if expr_path.path.segments.len() == 1 {
                            let segment = &expr_path.path.segments[0];
                            if segment.ident == self.struct_name {
                                if expr_field.member == self.field_name {
                                    expr_field.expr = parse_quote! { #expr_field.expr as #new_type };
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_field_mut(self, node);
            }
            fn visit_stmt_mut(&mut self, node: &mut Stmt) {
                if let Stmt::Local(local) = node {
                    if let Some((_, expr)) = &local.init {
                        if let Expr::Struct(expr_struct) = &**expr {
                            if expr_struct.path.is_ident(&self.struct_name) {
                                for field in &expr_struct.fields {
                                    if field.member == self.field_name {
                                        let new_local: Local = parse_quote! {
                                            let #local.pat : #new_type = #expr;
                                        };
                                        *node = Stmt::Local(new_local);
                                    }
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_stmt_mut(self, node);
            }
        }
        let mut replacer = FieldTypeReplacer {
            struct_name: struct_name.clone(),
            field_name: field_name.clone(),
            old_type: target_type,
            new_type,
        };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator selects a random struct and field, then changes the field's type to another available type in the program, prioritizing Vec and Option types. It updates all initializations and accesses of the field to use the new type, aiming to stress the compiler's type checking and inference systems by introducing potential type mismatches and conversion challenges."
    }
}
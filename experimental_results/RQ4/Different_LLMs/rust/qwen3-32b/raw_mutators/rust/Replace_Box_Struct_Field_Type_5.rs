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

pub struct Replace_Box_Struct_Field_Type_5;

impl Mutator for Replace_Box_Struct_Field_Type_5 {
    fn name(&self) -> &str {
        "Replace_Box_Struct_Field_Type_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut box_new_in_calls = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let block = &func.block;
                let mut visitor = BoxNewInVisitor::new();
                visitor.visit_block(block);
                box_new_in_calls.extend(visitor.calls);
            }
        }

        let struct_names: HashSet<Ident> = box_new_in_calls
            .iter()
            .filter_map(|(arg, _span)| {
                if let Expr::Struct(expr_struct) = &**arg {
                    if let Some(segment) = expr_struct.path.segments.iter().next() {
                        Some(segment.ident.clone())
                    } else {
                        None
                    }
                } else {
                    None
                }
            })
            .collect();

        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if struct_names.contains(&item_struct.ident) {
                    self.modify_struct_field(item_struct);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies structs allocated via Box::new_in with a custom allocator and alters one of their field types to a semantically incompatible but syntactically valid type (e.g., replacing a reference with a primitive). This disrupts type metadata and layout assumptions, potentially exposing LTO type-checking errors when the compiler merges code across crates."
    }
}

struct BoxNewInVisitor {
    calls: Vec<(Box<Expr>, Span)>,
}

impl BoxNewInVisitor {
    fn new() -> Self {
        Self { calls: Vec::new() }
    }
}

impl<'ast> Visit<'ast> for BoxNewInVisitor {
    fn visit_expr_call(&mut self, expr_call: &'ast ExprCall) {
        if let Expr::Path(expr_path) = &*expr_call.func {
            let path = &expr_path.path;
            if let Some(last_segment) = path.segments.iter().last() {
                if last_segment.ident == "new_in" && path.segments.len() >= 2 {
                    let first_segment = path.segments.iter().next().unwrap();
                    if first_segment.ident == "Box" {
                        if let Some(first_arg) = expr_call.args.get(0) {
                            self.calls.push((Box::new(first_arg.clone()), expr_call.span()));
                        }
                    }
                }
            }
        }
        syn::visit::visit_expr_call(self, expr_call);
    }
}

impl Replace_Box_Struct_Field_Type_5 {
    fn modify_struct_field(&self, item_struct: &mut ItemStruct) {
        if let syn::Body::Struct(struct_variant) = &mut item_struct.data {
            match struct_variant {
                syn::DataStruct::Struct(struct_data) => {
                    if let syn::Fields::Named(fields_named) = &mut struct_data.fields {
                        let field_count = fields_named.named.len();
                        if field_count == 0 {
                            return;
                        }
                        let mut rng = thread_rng();
                        let index = rng.gen_range(0..field_count - 1);
                        if let Some(field) = fields_named.named.get_mut(index) {
                            self.replace_field_type(field);
                        }
                    }
                }
                _ => {}
            }
        }
    }

    fn replace_field_type(&self, field: &mut Field) {
        let replacement_type = self.get_replacement_type();
        field.ty = Box::new(replacement_type);
    }

    fn get_replacement_type(&self) -> Type {
        let types = vec![
            parse_quote!(u32),
            parse_quote!(i32),
            parse_quote!(bool),
            parse_quote!(u8),
            parse_quote!(String),
            parse_quote!(Vec<u8>),
            parse_quote!(Option<i64>),
        ];
        let mut rng = thread_rng();
        let index = rng.gen_range(0..types.len());
        types[index].clone()
    }
}
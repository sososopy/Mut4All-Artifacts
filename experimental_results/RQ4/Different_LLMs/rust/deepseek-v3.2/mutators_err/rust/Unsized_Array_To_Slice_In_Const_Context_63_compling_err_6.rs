use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemStatic, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Unsized_Array_To_Slice_In_Const_Context_63;

impl Mutator for Unsized_Array_To_Slice_In_Const_Context_63 {
    fn name(&self) -> &str {
        "Unsized_Array_To_Slice_In_Const_Context_63"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_defs = Vec::new();
        let mut const_contexts = HashSet::new();

        // First pass: collect struct definitions with unsized array fields
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for field in &struct_item.fields {
                    if let Type::Slice(type_slice) = &field.ty {
                        // Check if it's an unsized array (slice without size)
                        struct_defs.push((struct_item.clone(), field.clone()));
                    }
                }
            }
        }

        // Second pass: identify const contexts (const fn and static items)
        for item in &mut file.items {
            if let Item::Fn(func_item) = item {
                if func_item.sig.constness.is_some() {
                    const_contexts.insert(func_item.sig.ident.to_string());
                }
            }
            if let Item::Static(static_item) = item {
                const_contexts.insert(static_item.ident.to_string());
            }
        }

        // Third pass: mutate struct fields and adjust constructor expressions
        for (struct_item, field) in struct_defs {
            // Change unsized array field type to slice reference
            if let Type::Slice(type_slice) = &field.ty {
                let new_type = Type::Reference(syn::TypeReference {
                    and_token: token::And::default(),
                    lifetime: None,
                    mutability: None,
                    elem: Box::new(Type::Slice(type_slice.clone())),
                });
                
                // Update the struct field in the original struct definition
                for item in &mut file.items {
                    if let Item::Struct(s) = item {
                        if s.ident == struct_item.ident {
                            for f in &mut s.fields {
                                if f.ident == field.ident {
                                    f.ty = new_type.clone();
                                }
                            }
                        }
                    }
                }
            }

            // Adjust constructor expressions in const contexts
            for item in &mut file.items {
                if let Item::Fn(func_item) = item {
                    if const_contexts.contains(&func_item.sig.ident.to_string()) {
                        // Visit the function body to find struct constructor expressions
                        let mut visitor = ConstructorVisitor {
                            struct_name: struct_item.ident.clone(),
                            field_name: field.ident.clone(),
                        };
                        visitor.visit_block_mut(&mut func_item.block);
                    }
                }
                if let Item::Static(static_item) = item {
                    if const_contexts.contains(&static_item.ident.to_string()) {
                        // Visit the static initializer expression
                        let mut visitor = ConstructorVisitor {
                            struct_name: struct_item.ident.clone(),
                            field_name: field.ident.clone(),
                        };
                        visitor.visit_expr_mut(&mut static_item.expr);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions containing unsized array fields ([T]) and changes them to slice references (&[T]). When these structs are used in const contexts (const fn or static items), corresponding constructor expressions are adjusted to use empty slices (&[]) as default initializers. This transformation stresses layout computation for unsized types in const-eval, potentially triggering assertions like layout.is_sized() failures and testing the compiler's handling of unsized types in constant evaluation contexts."
    }
}

struct ConstructorVisitor {
    struct_name: Ident,
    field_name: Option<Ident>,
}

impl VisitMut for ConstructorVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Struct(struct_expr) = expr {
            if struct_expr.path.segments.last().unwrap().ident == self.struct_name {
                // Check if the field is being initialized
                for field in &mut struct_expr.fields {
                    if let Some(field_ident) = &self.field_name {
                        if let syn::Member::Named(ident) = &field.member {
                            if ident == field_ident {
                                // Replace the field expression with an empty slice
                                field.expr = parse_quote!(&[]);
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}
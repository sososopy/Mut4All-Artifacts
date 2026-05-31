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

pub struct Modify_Match_Arm_Binding_To_Wildcard_281;

impl Mutator for Modify_Match_Arm_Binding_To_Wildcard_281 {
    fn name(&self) -> &str {
        "Modify_Match_Arm_Binding_To_Wildcard_281"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ModifyMatchArmVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match expressions where a mutable variable is matched and at least one arm modifies that variable while another arm uses a pattern that binds a variable (e.g., Some(x)). It transforms the binding pattern to a wildcard (Some(_)) and introduces a local variable declaration with a default value to maintain compilation. This transformation alters control flow and type-checking assumptions, potentially exposing compiler bugs in pattern matching, variable binding, and type inference."
    }
}

struct ModifyMatchArmVisitor;

impl ModifyMatchArmVisitor {
    fn new() -> Self {
        Self
    }

    fn get_default_expr_for_type(&self, ty: &Type) -> Option<Expr> {
        match ty {
            Type::Path(type_path) => {
                let path = &type_path.path;
                if let Some(segment) = path.segments.last() {
                    match segment.ident.to_string().as_str() {
                        "i32" | "i64" | "i128" | "isize" => Some(parse_quote! { 0 }),
                        "u32" | "u64" | "u128" | "usize" => Some(parse_quote! { 0 }),
                        "f32" => Some(parse_quote! { 0.0 }),
                        "f64" => Some(parse_quote! { 0.0 }),
                        "bool" => Some(parse_quote! { false }),
                        "char" => Some(parse_quote! { '\0' }),
                        "String" => Some(parse_quote! { String::new() }),
                        _ => None,
                    }
                } else {
                    None
                }
            }
            _ => None,
        }
    }
}

impl VisitMut for ModifyMatchArmVisitor {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        let mut has_mutation_arm = false;
        let mut binding_arms = Vec::new();

        for arm in &expr.arms {
            if let Pat::Ident(pat_ident) = &arm.pat {
                if pat_ident.ident == "mut" {
                    continue;
                }
            }

            let mut visitor = MatchArmVisitor::new();
            visitor.visit_expr(&arm.body);
            if visitor.modifies_matched_variable {
                has_mutation_arm = true;
            }

            if let Pat::TupleStruct(pat_tuple_struct) = &arm.pat {
                if let Some(segment) = pat_tuple_struct.path.segments.last() {
                    if segment.ident == "Some" {
                        if let Some(elem) = pat_tuple_struct.elems.first() {
                            if let Pat::Ident(binding_ident) = elem {
                                binding_arms.push((arm.clone(), binding_ident.ident.clone()));
                            }
                        }
                    }
                }
            } else if let Pat::Struct(pat_struct) = &arm1.pat {
                if let Some(segment) = pat_struct.path.segments.last() {
                    if segment.ident == "Some" {
                        for field in &pat_struct.fields {
                            if let Pat::Ident(binding_ident) = &*field.pat {
                                binding_arms.push((arm.clone(), binding_ident.ident.clone()));
                            }
                        }
                    }
                }
            }
        }

        if has_mutation_arm && !binding_arms.is_empty() {
            for (arm, binding_ident) in binding_arms {
                let arm_index = expr.arms.iter().position(|a| a == &arm).unwrap();
                let mut new_arm = expr.arms[arm_index].clone();

                if let Pat::TupleStruct(pat_tuple_struct) = &mut new_arm.pat {
                    pat_tuple_struct.elems = Punctuated::new();
                    pat_tuple_struct.elems.push(Pat::Wild(PatWild {
                        attrs: Vec::new(),
                        underscore_token: token::Underscore::default(),
                    }));
                } else if let Pat::Struct(pat_struct) = &mut new_arm.pat {
                    pat_struct.fields = Punctuated::new();
                    pat_struct.fields.push(FieldPat {
                        attrs: Vec::new(),
                        member: Member::Named(Ident::new("_", Span::call_site())),
                        colon_token: None,
                        pat: Box::new(Pat::Wild(PatWild {
                            attrs: Vec::new(),
                            underscore_token: token::Underscore::default(),
                        })),
                    });
                }

                let mut type_inference_visitor = TypeInferenceVisitor::new();
                type_inference_visitor.visit_expr(&new_arm.body);

                let default_expr = if let Some(ty) = type_inference_1visitor.get_type_for_ident(&binding_ident) {
                    self.get_default_expr_for_type(&ty).unwrap_or_else(|| parse_quote! { panic!("default value not available") })
                } else {
                    parse_quote! { panic!("type inference failed") }
                };

                let new_stmt: Stmt = parse_quote! {
                    let #binding_ident = #default_expr;
                };

                if let Expr::Block(expr_block) = &*new_arm.body {
                    let mut expr_block = expr_block.clone();
                    expr_block.block.stmts.insert(0, new_stmt);
                    new_arm.body = Box::new(Expr::Block(expr_block));
                } else {
                    let block: ExprBlock = parse_quote! {{
                        let #binding_ident = #default_expr;
                        #new_arm.body
                    }};
                    new_arm.body = Box::new(Expr::Block(block));
                }

                expr.arms[arm_index] = new_arm;
            }
        }

        visit_mut::visit_expr_match_mut(self, expr);
    }
}

struct MatchArmVisitor {
    modifies_matched_variable: bool,
}

impl MatchArmVisitor {
    fn new() -> Self {
        Self {
            modifies_matched_variable: false,
        }
    }
}

impl Visit<'_> for MatchArmVisitor {
    fn visit_expr_assign(&mut self, expr: &ExprAssign) {
        if let Expr::Path(expr_path) = &*expr.left {
            if let Some(segment) = expr_path.path.segments.last() {
                if segment.ident == "mut" {
                    self.modifies_matched_variable = true;
                }
            }
        }
        syn::visit::visit_expr_assign(self, expr);
    }
}

struct TypeInferenceVisitor {
    ident_types: std::collections::HashMap<Ident, Type>,
}

impl TypeInferenceVisitor {
    fn new() -> Self {
        Self {
            ident_types: std::collections::HashMap::new(),
        }
    }

    fn get_type_for_ident(&self, ident: &Ident) -> Option<Type> {
        self.ident_types.get(ident).cloned()
    }
}

impl Visit<'_> for TypeInferenceVisitor {
    fn visit_local(&mut self, local: &Local) {
        if let Pat::Ident(pat_ident) = &local.pat {
            if let Some(init) = &local.init {
                if let Expr::Type(expr_type) = &*init.expr {
                    self.ident_types.insert(pat_ident.ident.clone(), expr_type.ty.clone());
                }
            }
        }
        syn::visit::visit_local(self, local);
    }

    fn visit_stmt(&mut self, stmt: &Stmt) {
        if let Stmt::Local(local) = stmt {
            self.visit_local(local);
        } else {
            syn::visit::visit_stmt(self, stmt);
        }
    }
}
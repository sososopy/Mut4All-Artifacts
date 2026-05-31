use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemUnion, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_DynStar_With_Union_Implementing_Trait_332;

impl Mutator for Replace_DynStar_With_Union_Implementing_Trait_332 {
    fn name(&self) -> &str {
        "Replace_DynStar_With_Union_Implementing_Trait_332"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceDynStarVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function calls where arguments are passed to parameters of type `dyn* Trait`. It searches for union types in scope that implement the trait, then replaces the argument expression with a union constructor using default values for fields. This transformation tests the compiler's handling of dyn-star trait objects and union type conversions, potentially triggering ICEs in trait object coercion and union initialization."
    }
}

struct ReplaceDynStarVisitor {
    unions: Vec<(Ident, Vec<Field>, Vec<SynPath>)>,
    trait_impls: Vec<(SynPath, SynPath)>,
}

impl ReplaceDynStarVisitor {
    fn new() -> Self {
        Self {
            unions: Vec::new(),
            trait_impls: Vec::new(),
        }
    }
    
    fn find_union_fields(union: &ItemUnion) -> Vec<Field> {
        union.fields.named.iter().map(|f| Field {
            ident: f.ident.clone(),
            ty: f.ty.clone(),
        }).collect()
    }
    
    fn collect_unions_and_impls(&mut self, file: &File) {
        for item in &file.items {
            if let Item::Union(union) = item {
                let fields = Self::find_union_fields(union);
                self.unions.push((union.ident.clone(), fields, Vec::new()));
            }
            if let Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &impl_item.trait_ {
                    if let Type::Path(type_path) = &*impl_item.self_ty {
                        self.trait_impls.push((trait_path.clone(), type_path.path.clone()));
                    }
                }
            }
        }
    }
    
    fn get_union_for_trait(&self, trait_path: &SynPath) -> Option<(Ident, Vec<Field>, Vec<SynPath>)> {
        for (union_ident, fields, _) in &self.unions {
            for (impl_trait, impl_type) in &self.trait_impls {
                if impl_trait == trait_path {
                    if let Some(seg) = impl_type.segments.last() {
                        if seg.ident == *union_ident {
                            return Some((union_ident.clone(), fields.clone(), Vec::new()));
                        }
                    }
                }
            }
        }
        None
    }
    
    fn create_union_constructor(&self, union_ident: &Ident, fields: &[Field]) -> Expr {
        let field_inits: Vec<Expr> = fields.iter().map(|field| {
            let field_ident = field.ident.as_ref().unwrap();
            let default_expr: Expr = match &field.ty {
                Type::Path(type_path) => {
                    if let Some(seg) = type_path.path.segments.last() {
                        match seg.ident.to_string().as_str() {
                            "i32" | "i64" | "isize" | "u32" | "u64" | "usize" => parse_quote! { 0 },
                            "f32" | "f64" => parse_quote! { 0.0 },
                            "bool" => parse_quote! { false },
                            "char" => parse_quote! { '\0' },
                            "String" => parse_quote! { String::new() },
                            _ => parse_quote! { Default::default() },
                        }
                    } else {
                        parse_quote! { Default::default() }
                    }
                }
                _ => parse_quote! { Default::default() },
            };
            parse_quote! { #field_ident: #default_expr }
        }).collect();
        
        parse_quote! { #union_ident { #(#field_inits),* } }
    }
    
    fn find_dyn_star_trait_in_arg(&self, expr: &Expr) -> Option<SynPath> {
        if let Expr::Cast(cast) = expr {
            if let Type::TraitObject(trait_obj) = &*cast.ty {
                if trait_obj.dyn_token.is_some() {
                    for bound in &trait_obj.bounds {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            return Some(trait_bound.path.clone());
                        }
                    }
                }
            }
        }
        None
    }
    
    fn extract_dyn_star_trait(&self, ty: &Box<Type>) -> Option<SynPath> {
        if let Type::ImplTrait(type_impl) = &**ty {
            for bound in &type_impl.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    return Some(trait_bound.path.clone());
                }
            }
        }
        if let Type::TraitObject(trait_obj) = &**ty {
            if trait_obj.dyn_token.is_some() {
                for bound in &trait_obj.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        return Some(trait_bound.path.clone());
                    }
                }
            }
        }
        None
    }
}

impl VisitMut for ReplaceDynStarVisitor {
    fn visit_file_mut(&mut self, file: &mut File) {
        self.collect_unions_and_impls(file);
        syn::visit_mut::visit_file_mut(self, file);
    }
    
    fn visit_expr_call_mut(&mut self, expr: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*expr.func {
            for arg in &mut expr.args {
                if let Some(trait_path) = self.find_dyn_star_trait_in_arg(arg) {
                    if let Some((union_ident, fields, _)) = self.get_union_for_trait(&trait_path) {
                        let union_expr = self.create_union_constructor(&union_ident, &fields);
                        *arg = union_expr;
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, expr);
    }
    
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        for input in &mut i.sig.inputs {
            if let FnArg::Typed(PatType { ty, .. }) = input {
                if let Some(trait_path) = self.extract_dyn_star_trait(ty) {
                    if let Some((union_ident, fields, _)) = self.get_union_for_trait(&trait_path) {
                        let union_expr = self.create_union_constructor(&union_ident, &fields);
                        if let Some(stmt) = i.block.stmts.first_mut() {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Assign(assign) = expr {
                                    if let Expr::Path(_) = *assign.left {
                                        assign.right = Box::new(union_expr);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_item_fn_mut(self, i);
    }
}

#[derive(Clone)]
struct Field {
    ident: Option<Ident>,
    ty: Type,
}
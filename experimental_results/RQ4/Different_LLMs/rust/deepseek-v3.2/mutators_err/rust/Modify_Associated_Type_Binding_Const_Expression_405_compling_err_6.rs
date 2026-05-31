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

pub struct Modify_Associated_Type_Binding_Const_Expression_405;

impl Mutator for Modify_Associated_Type_Binding_Const_Expression_405 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_Binding_Const_Expression_405"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_items = Vec::new();
        let mut trait_object_uses = Vec::new();
        
        struct ConstCollector<'a> {
            const_items: &'a mut Vec<syn::ItemConst>,
        }
        impl<'ast> Visit<'ast> for ConstCollector<'_> {
            fn visit_item_const(&mut self, node: &'ast syn::ItemConst) {
                self.const_items.push(node.clone());
                syn::visit::visit_item_const(self, node);
            }
        }
        let mut collector = ConstCollector {
            const_items: &mut const_items,
        };
        collector.visit_file(file);
        
        struct TraitObjectFinder<'a> {
            trait_object_uses: &'a mut Vec<(syn::TypeTraitObject, proc_macro2::Span)>,
        }
        impl<'ast> Visit<'ast> for TraitObjectFinder<'_> {
            fn visit_type_trait_object(&mut self, node: &'ast syn::TypeTraitObject) {
                self.trait_object_uses.push((node.clone(), node.span()));
                syn::visit::visit_type_trait_object(self, node);
            }
        }
        let mut finder = TraitObjectFinder {
            trait_object_uses: &mut trait_object_uses,
        };
        finder.visit_file(file);
        
        let mut rng = thread_rng();
        if trait_object_uses.is_empty() {
            return;
        }
        
        let (selected_trait_object, span) = trait_object_uses.choose(&mut rng).unwrap();
        
        let mut visitor = ModifyAssociatedTypeBindingVisitor {
            target_span: span,
            const_items: &const_items,
            rng: &mut rng,
            modified: false,
            new_const_name: None,
        };
        visitor.visit_file_mut(file);
        
        if !visitor.modified {
            let new_const_name = Ident::new("MUTATED_CONST", Span::call_site());
            let new_const_value: syn::Expr = parse_quote! { 0 };
            let new_const_type: syn::Type = parse_quote! { i32 };
            
            let new_const_item: syn::Item = parse_quote! {
                const #new_const_name: #new_const_type = #new_const_value;
            };
            
            let mut visitor2 = ModifyAssociatedTypeBindingVisitor {
                target_span: span,
                const_items: &const_items,
                rng: &mut rng,
                modified: false,
                new_const_name: Some(new_const_name.clone()),
            };
            visitor2.visit_file_mut(file);
            
            if visitor2.modified {
                file.items.insert(0, new_const_item);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets dyn Trait<Assoc<const_expr> = Type> patterns in trait object types. It modifies the const expression within associated type bindings by either replacing it with an existing const item of matching type, altering integer literals (e.g., 1 to 0, 2, or 1+1), or introducing a new const item in scope. This transformation stresses the compiler's handling of generic const parameters in associated type bindings, testing const expression evaluation, type resolution, and trait object compatibility."
    }
}

struct ModifyAssociatedTypeBindingVisitor<'a> {
    target_span: &'a proc_macro2::Span,
    const_items: &'a Vec<syn::ItemConst>,
    rng: &'a mut rand::rngs::ThreadRng,
    modified: bool,
    new_const_name: Option<syn::Ident>,
}

impl<'a> VisitMut for ModifyAssociatedTypeBindingVisitor<'a> {
    fn visit_type_trait_object_mut(&mut self, node: &mut syn::TypeTraitObject) {
        if node.span() != self.target_span {
            return;
        }
        
        for bound in &mut node.bounds {
            if let TypeParamBound::Trait(trait_bound) = bound {
                let mut new_args = Punctuated::new();
                for arg in &mut trait_bound.path.segments {
                    if let PathArguments::AngleBracketed(angle_args) = &mut arg.arguments {
                        for generic_arg in &mut angle_args.args {
                            if let GenericArgument::AssocType(assoc_type) = generic_arg {
                                if let Some(angle_args_inner) = &mut assoc_type.generics {
                                    for generic_arg_inner in &mut angle_args_inner.args {
                                        if let GenericArgument::Const(const_expr) = generic_arg_inner {
                                            self.modify_const_expression(const_expr);
                                            self.modified = true;
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

impl<'a> ModifyAssociatedTypeBindingVisitor<'a> {
    fn modify_const_expression(&mut self, const_expr: &mut syn::Expr) {
        let mut candidate_consts = Vec::new();
        for const_item in self.const_items {
            candidate_consts.push(syn::Expr::Path(syn::ExprPath {
                attrs: Vec::new(),
                qself: None,
                path: syn::Path::from(const_item.ident.clone()),
            }));
        }
        
        if !candidate_consts.is_empty() && self.rng.gen_bool(0.5) {
            let selected_const = candidate_consts.choose(self.rng).unwrap().clone();
            *const_expr = selected_const;
            return;
        }
        
        if let syn::Expr::Lit(expr_lit) = const_expr {
            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                let value = lit_int.base10_parse::<i64>().unwrap_or(0);
                let new_value = match self.rng.gen_range(0..3) {
                    0 => value.wrapping_sub(1),
                    1 => value.wrapping_add(1),
                    _ => 0,
                };
                let new_lit = syn::LitInt::new(&new_value.to_string(), lit_int.span());
                *const_expr = syn::Expr::Lit(syn::ExprLit {
                    attrs: Vec::new(),
                    lit: syn::Lit::Int(new_lit),
                });
                return;
            }
        }
        
        if let Some(ref new_const_name) = self.new_const_name {
            *const_expr = syn::Expr::Path(syn::ExprPath {
                attrs: Vec::new(),
                qself: None,
                path: syn::Path::from(new_const_name.clone()),
            });
        } else {
            let new_expr: syn::Expr = parse_quote! { 0 };
            *const_expr = new_expr;
        }
    }
}
use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprCast, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Introduce_Const_Trait_Bound_On_Trait_Object_311;

impl Mutator for Introduce_Const_Trait_Bound_On_Trait_Object_311 {
    fn name(&self) -> &str {
        "Introduce_Const_Trait_Bound_On_Trait_Object_311"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstTraitBoundVisitor::new();
        visitor.visit_file_mut(file);
        
        if !visitor.feature_added {
            let feature = parse_quote! { #![feature(const_trait_impl)] };
            file.items.insert(0, syn::Item::Verbatim(feature));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces ~const trait bounds on trait objects within const contexts. It traverses the AST to find trait object types (both dyn Trait and bare Trait) in const functions, parameters, return types, and variable declarations, then adds the ~const modifier before the trait name. This creates syntactically valid but semantically invalid code that should trigger compiler assertions about ~const bounds on trait objects. The operator also ensures the necessary feature gates are present."
    }
}

struct ConstTraitBoundVisitor {
    feature_added: bool,
}

impl ConstTraitBoundVisitor {
    fn new() -> Self {
        Self { feature_added: false }
    }
    
    fn add_const_modifier(&mut self, bounds: &mut Punctuated<TypeParamBound, token::Add>) {
        for bound in bounds.iter_mut() {
            if let TypeParamBound::Trait(trait_bound) = bound {
                if trait_bound.modifier != TraitBoundModifier::MaybeConst {
                    trait_bound.modifier = TraitBoundModifier::MaybeConst;
                    self.feature_added = true;
                }
            }
        }
    }
}

impl VisitMut for ConstTraitBoundVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        match ty {
            Type::TraitObject(type_trait_object) => {
                self.add_const_modifier(&mut type_trait_object.bounds);
            }
            Type::Path(type_path) => {
                if let Some(last_segment) = type_path.path.segments.last_mut() {
                    if last_segment.ident == "dyn" {
                        if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                            for arg in args.args.iter_mut() {
                                if let GenericArgument::Type(Type::TraitObject(trait_obj)) = arg {
                                    self.add_const_modifier(&mut trait_obj.bounds);
                                }
                            }
                        }
                    }
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
    
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        let is_const = i.sig.constness.is_some();
        
        for input in &mut i.sig.inputs {
            if let FnArg::Typed(pat_type) = input {
                self.visit_type_mut(&mut pat_type.ty);
            }
        }
        
        if let ReturnType::Type(_, return_type) = &mut i.sig.output {
            self.visit_type_mut(return_type);
        }
        
        if is_const {
            syn::visit_mut::visit_block_mut(self, &mut i.block);
        }
        
        syn::visit_mut::visit_item_fn_mut(self, i);
    }
    
    fn visit_local_mut(&mut self, local: &mut Local) {
        if let Pat::Type(pat_type) = &mut local.pat {
            self.visit_type_mut(&mut pat_type.ty);
        }
        syn::visit_mut::visit_local_mut(self, local);
    }
    
    fn visit_expr_cast_mut(&mut self, expr_cast: &mut ExprCast) {
        self.visit_type_mut(&mut expr_cast.ty);
        syn::visit_mut::visit_expr_cast_mut(self, expr_cast);
    }
}
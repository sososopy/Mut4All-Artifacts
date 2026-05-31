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

pub struct Replace_Const_Generic_In_Default_Type_403;

impl Mutator for Replace_Const_Generic_In_Default_Type_403 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_In_Default_Type_403"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut const_param_name = None;
                let mut const_param_type = None;
                
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param_name = Some(const_param.ident.clone());
                        const_param_type = Some(const_param.ty.clone());
                        break;
                    }
                }
                
                if let (Some(const_name), Some(const_type)) = (const_param_name, const_param_type) {
                    let mut visitor = DefaultTypeVisitor {
                        const_name: &const_name,
                        const_type: &const_type,
                        mutated: false,
                    };
                    visitor.visit_item_struct_mut(item_struct);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with generic const parameters and associated type projections in default types. It replaces the const argument within the default type expression with a mismatched constant (e.g., replacing a boolean const with a unit type `()` literal). This creates a type mismatch that may lead to const evaluation errors or ICEs during well-formedness checking when the compiler attempts to instantiate generic parameters with erroneous const arguments."
    }
}

struct DefaultTypeVisitor<'a> {
    const_name: &'a Ident,
    const_type: &'a Type,
    mutated: bool,
}

impl<'a> VisitMut for DefaultTypeVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if self.mutated {
            return;
        }
        
        if let Type::Path(type_path) = ty {
            if let Some(last_segment) = type_path.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(expr) = arg {
                            if let Expr::Path(expr_path) = expr {
                                if expr_path.path.segments.len() == 1 {
                                    if expr_path.path.segments[0].ident == *self.const_name {
                                        let replacement_expr: Expr = parse_quote! { {} };
                                        *expr = replacement_expr;
                                        self.mutated = true;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        syn::visit_mut::visit_type_mut(self, ty);
    }
}
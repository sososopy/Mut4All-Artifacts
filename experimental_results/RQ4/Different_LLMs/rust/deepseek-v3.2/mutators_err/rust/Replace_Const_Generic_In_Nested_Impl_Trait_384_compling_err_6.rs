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

pub struct Replace_Const_Generic_In_Nested_Impl_Trait_384;

impl Mutator for Replace_Const_Generic_In_Nested_Impl_Trait_384 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_In_Nested_Impl_Trait_384"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let const_params: Vec<Ident> = item_impl.generics.params.iter()
                    .filter_map(|param| {
                        if let GenericParam::Const(const_param) = param {
                            Some(const_param.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if const_params.len() < 2 {
                    continue;
                }
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                            let mut visitor = ConstGenericReplacer {
                                const_params: &const_params,
                                rng: &mut rng,
                                changed: false,
                            };
                            visitor.visit_type_mut(&mut **return_type);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets impl blocks with at least two const generic parameters. It traverses function return types, specifically looking for nested impl Trait structures that contain const generic parameters in array lengths or similar const expressions. When found, it replaces the const parameter with a new const expression using a different const parameter from the same impl block, performing simple arithmetic (addition or multiplication). This mutation aims to trigger bugs in const evaluation within nested impl trait contexts by altering the const dependency graph and potentially creating invalid or unexpected const expressions."
    }
}

struct ConstGenericReplacer<'a, R: Rng> {
    const_params: &'a Vec<Ident>,
    rng: &'a mut R,
    changed: bool,
}

impl<'a, R: Rng> VisitMut for ConstGenericReplacer<'a, R> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::ImplTrait(type_impl_trait) = ty {
            for bound in &mut type_impl_trait.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    self.visit_path_mut(&mut trait_bound.path);
                }
            }
        }
        if let Type::Array(type_array) = ty {
            if let Expr::Path(expr_path) = &*type_array.len {
                if expr_path.path.segments.len() == 1 {
                    let ident = &expr_path.path.segments[0].ident;
                    if self.const_params.iter().any(|c| c == ident) && !self.changed {
                        let new_param = self.const_params.choose(self.rng).unwrap();
                        while new_param == ident {
                            let new_param = self.const_params.choose(self.rng).unwrap();
                        }
                        let new_param = self.const_params.choose(self.rng).unwrap();
                        let op = if self.rng.gen_bool(0.5) { "+" } else { "*" };
                        let num = self.rng.gen_range(1..=5);
                        let new_expr: Expr = match op {
                            "+" => parse_quote! { #new_param + #num },
                            "*" => parse_quote! { #new_param * #num },
                            _ => unreachable!(),
                        };
                        type_array.len = new_expr;
                        self.changed = true;
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}